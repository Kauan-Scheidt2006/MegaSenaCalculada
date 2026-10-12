# Planejamento das métricas de busca e remoção

## Objetivo

Fazer BST, AVL e rubro-negra informarem separadamente, nas operações de busca e
remoção:

1. quantidade de nós visitados;
2. quantidade de chamadas reais ao comparador de chaves;
3. tempo da operação, mantido sob responsabilidade da camada de aplicação.

A alteração elimina a ambiguidade atual, na qual o campo chamado
`comparacoes` é incrementado uma vez por nó visitado, mesmo quando o comparador
é chamado duas vezes naquele nó.

## Definições

### Nó visitado

Um nó é visitado quando a operação inspeciona esse nó para navegar pela árvore.

Na busca, isso corresponde a cada nó não nulo do caminho entre a raiz e a chave
encontrada ou a posição nula que encerra uma busca malsucedida. O ponteiro nulo
final não conta como nó.

Na remoção, contam:

- os nós visitados para localizar a chave;
- quando o nó possui dois filhos, os nós percorridos para localizar o sucessor
  em ordem na subárvore direita, incluindo o próprio sucessor.

Não contam como novas visitas:

- ancestrais já percorridos que são processados novamente no retorno recursivo;
- nós acessados somente para atualizar altura ou fator de balanceamento;
- pai, irmão ou sobrinhos acessados durante a correção de cores;
- nós envolvidos em rotação ou recoloração.

Essas ações continuam representadas pelas métricas próprias de rotações e pelo
tempo total da operação. A definição de nós visitados permanece restrita à
navegação estrutural para encontrar o alvo ou seu substituto.

### Comparação

Cada chamada efetiva a `comparador_(a, b)` conta como uma comparação.

Com um comparador de ordem estrita, a navegação atual pode realizar:

- uma comparação quando `valor < no->valor` for verdadeiro;
- duas comparações quando o caminho seguir à direita;
- duas comparações para concluir que as chaves são equivalentes.

A procura estrutural do sucessor não chama o comparador e, portanto, acrescenta
nós visitados, mas não comparações de chave.

### Tempo

O tempo continua sendo medido com `std::chrono::steady_clock` ao redor da
chamada pública completa de busca ou remoção. Incrementar os dois contadores faz
parte da execução instrumentada.

O tempo de uma única busca permanece uma amostra, não um benchmark conclusivo.
Os contadores determinísticos devem ser a principal base de comparação entre
as estruturas.

## Contrato compartilhado

Criar um tipo independente das árvores concretas:

```cpp
namespace estruturas {

struct MetricasNavegacaoArvore {
    std::size_t nosVisitados = 0;
    std::size_t comparacoes = 0;
};

} // namespace estruturas
```

Arquivo previsto:

```text
src/estruturas/MetricasNavegacaoArvore.hpp
```

O tipo deve conter somente dados, sem depender de Qt, domínio da Mega-Sena ou
uma implementação concreta de árvore.

## Contratos públicos das árvores

BST, AVL e rubro-negra devem expor a mesma interface:

```cpp
const Valor* buscar(
    const Valor& valor,
    MetricasNavegacaoArvore& metricas) const noexcept;

bool remover(
    const Valor& valor,
    MetricasNavegacaoArvore& metricas);
```

Os overloads simples permanecem:

```cpp
const Valor* buscar(const Valor& valor) const noexcept;
bool remover(const Valor& valor);
```

Eles criam métricas locais e delegam aos overloads instrumentados.

Os overloads antigos que recebem apenas `std::size_t& comparacoes` devem ser
removidos depois da migração de todos os chamadores. Mantê-los perpetuaria a
ambiguidade porque o valor atualmente representa nós visitados, não chamadas
ao comparador.

Toda chamada instrumentada deve zerar ambos os campos antes de iniciar a
operação, mesmo que o objeto de métricas tenha sido reutilizado.

## Arquivos afetados

```text
src/estruturas/MetricasNavegacaoArvore.hpp
src/estruturas/ArvoreBinariaBusca.hpp
src/estruturas/ArvoreAvl.hpp
src/estruturas/ArvoreRubroNegra.hpp
src/aplicacao/arvore/ExecutorPercursosArvore.hpp
src/aplicacao/arvore/ExecutorPercursosArvore.cpp
src/interface/paginas/arvore/PainelVisualizacaoArvore.cpp
testes/testes_estrutura_inicial.cpp
testes/testes_processamento_apresentacao.cpp
docs/decisoes/README.md
docs/decisoes/0016-metricas-navegacao-arvores.md
```

Outros arquivos de propagação pela interface somente devem ser alterados se a
assinatura atual exigir transportar os campos separadamente.

## Etapas de implementação

### 1. Introduzir o tipo comum

- criar `MetricasNavegacaoArvore`;
- incluir `<cstddef>` no próprio cabeçalho;
- não adicionar comportamento ou conhecimento de árvore concreta;
- usar o mesmo tipo nas três estruturas.

### 2. Corrigir a busca nas três árvores

Em cada `buscarNaSubarvore()`:

1. retornar quando o ponteiro for nulo, sem incrementar contadores;
2. incrementar `nosVisitados` ao entrar em um nó não nulo;
3. incrementar `comparacoes` imediatamente antes de cada chamada ao comparador;
4. seguir somente o filho determinado pelo resultado;
5. retornar o valor quando as duas comparações de ordem forem falsas.

O algoritmo de navegação não muda. A alteração é apenas de instrumentação e
contrato.

### 3. Corrigir a localização do alvo na remoção

Na BST e AVL, substituir o contador isolado recebido por
`removerNaSubarvore()` pelas métricas compartilhadas. Cada nó usado para decidir
se a busca segue à esquerda, à direita ou encontrou a chave incrementa
`nosVisitados`; cada chamada ao comparador incrementa `comparacoes`.

Na rubro-negra, aplicar a mesma regra em `encontrarNo()`.

Uma remoção malsucedida deve informar exatamente o custo da navegação até o
ponteiro nulo e não alterar a árvore.

### 4. Contabilizar a procura do sucessor

Quando a remoção encontrar um nó com dois filhos:

- BST: instrumentar `extrairMinimo()`;
- AVL: instrumentar `extrairMinimo()`;
- rubro-negra: instrumentar `minimo()`.

Cada nó não nulo percorrido nessa procura acrescenta uma visita. Nenhuma
comparação de chave é acrescentada, pois o algoritmo apenas testa a existência
do filho esquerdo.

O nó alvo já contado durante sua localização não deve ser contado novamente.

### 5. Propagar as métricas pela aplicação

Atualizar os resultados:

```cpp
struct ResultadoBuscaArvore {
    std::optional<megasena::Registro> registro;
    std::size_t nosVisitados = 0;
    std::size_t comparacoes = 0;
    std::uint64_t tempoExecucaoNanossegundos = 0;
};

struct ResultadoRemocaoArvore {
    bool removido = false;
    std::size_t nosVisitados = 0;
    std::size_t comparacoes = 0;
    std::uint64_t tempoExecucaoNanossegundos = 0;
};
```

`medirBusca()` e `medirRemocao()` devem criar uma única instância de
`MetricasNavegacaoArvore`, passá-la à árvore e copiar os dois resultados depois
que o relógio for encerrado.

O valor encontrado pode continuar sendo copiado para o `optional` fora do
intervalo cronometrado.

### 6. Propagar até a interface

Atualizar as assinaturas entre `Aplicacao`, `PaginaArvore`,
`ConteudoPaginaArvore` e `PainelVisualizacaoArvore` para transportar ambos os
valores sem recálculo.

Formato sugerido:

```text
Busca pela chave (3, 27) • Encontrada • Nós visitados: 12 • Comparações: 19 • Tempo: 420 ns
```

```text
Remoção da chave (3, 27) • Removida • Nós visitados: 15 • Comparações: 19 • Tempo: 1,20 us
```

A descrição acessível deve conter os mesmos valores apresentados visualmente.

### 7. Atualizar os testes unitários

Usar sequências pequenas calculadas manualmente.

Para a árvore:

```text
       20
      /  \
    10    30
```

Resultados esperados da busca:

| Busca | Nós visitados | Comparações |
|---|---:|---:|
| `20` | 1 | 2 |
| `10` | 2 | 3 |
| `30` | 2 | 4 |
| `5` ausente | 2 | 2 |
| `40` ausente | 2 | 4 |

Esses valores decorrem do comparador de ordem estrita usado atualmente. Os
testes devem ser executados para BST, AVL e rubro-negra quando a topologia for a
mesma.

Também verificar:

- árvore vazia produz zero visitas e zero comparações;
- uma nova operação zera métricas previamente preenchidas;
- busca simples sem métricas continua funcionando;
- a chave completa `(frequenciaAcumulada, numero)` continua determinando a
  igualdade.

### 8. Testar remoções e sucessores

Casos mínimos para cada árvore:

- remoção ausente;
- remoção da raiz isolada;
- remoção de folha à esquerda e à direita;
- remoção de nó com um filho;
- remoção de nó com dois filhos cujo sucessor é o filho direito imediato;
- remoção com sucessor localizado após vários filhos esquerdos.

Para cada caso, calcular manualmente:

- visitas para localizar o alvo;
- visitas adicionais para localizar o sucessor;
- chamadas ao comparador;
- estado final da árvore.

AVL e rubro-negra devem continuar validando balanceamento e invariantes após a
remoção. Rotações não devem modificar os contadores de navegação.

### 9. Testar a camada de aplicação e a interface

- confirmar que `ResultadoBuscaArvore` e `ResultadoRemocaoArvore` preservam os
  dois contadores;
- confirmar que a interface não troca a ordem dos campos;
- verificar busca encontrada e ausente;
- verificar remoção bem-sucedida, ausente e repetida;
- confirmar que o tempo continua envolvendo somente a chamada pública da
  operação;
- atualizar testes que atualmente interpretam `comparacoes == 2` como duas
  visitas.

## Verificações quantitativas

Depois dos testes pequenos, executar todas as buscas bem-sucedidas sobre o
histórico atual e registrar, separadamente por estrutura:

- média, mediana e máximo de nós visitados;
- média, mediana e máximo de chamadas ao comparador;
- altura final;
- distribuição de chaves para as quais cada estrutura visita menos nós.

Não criar teste automatizado exigindo que toda busca AVL seja menor que a
rubro-negra. A AVL possui melhor limite de altura, mas uma chave específica
pode estar mais próxima da raiz rubro-negra.

Tempos devem ser avaliados em lotes, com compilação otimizada, aquecimento e
ordem alternada. Não adicionar limites rígidos de nanossegundos à suíte.

## Critérios de aceitação

- BST, AVL e rubro-negra usam exatamente o mesmo contrato de métricas;
- `nosVisitados` conta somente a navegação definida neste documento;
- `comparacoes` corresponde ao número real de chamadas ao comparador;
- busca e remoção zeram os dois contadores no início;
- remoção com dois filhos inclui o percurso até o sucessor nas visitas;
- rotações, recolorações e atualização de alturas não inflam os contadores de
  navegação;
- operações simples sem métricas preservam o comportamento atual;
- resultados percorrem aplicação e interface sem recálculo;
- testes manuais pequenos confirmam valores exatos;
- testes de invariantes, processamento, modelo e interface relacionados passam;
- nenhuma afirmação de desempenho depende do tempo de uma única operação.

## Ordem de execução recomendada

1. tipo compartilhado e instrumentação da busca;
2. testes exatos da busca;
3. instrumentação da localização na remoção;
4. contagem do percurso ao sucessor;
5. testes exatos da remoção e regressão de invariantes;
6. propagação pelos resultados da aplicação;
7. apresentação e acessibilidade;
8. análise quantitativa do histórico;
9. revisão independente do contrato e dos incrementos.

## Riscos

- incrementar comparações depois da chamada ao comparador pode perder a
  contagem quando houver retorno antecipado;
- contar novamente ancestrais no retorno recursivo mistura navegação com
  manutenção estrutural;
- omitir o percurso ao sucessor subestima remoções com dois filhos;
- manter o overload antigo com `std::size_t&` permite que chamadores continuem
  interpretando visitas como comparações;
- comparar tempo individual em nanossegundos pode contradizer os contadores por
  ruído de medição;
- remoções aplicadas somente a uma estrutura tornam comparações posteriores
  entre árvores inválidas se os conjuntos deixarem de ser iguais.

## Condição de conclusão

A alteração estará concluída quando busca e remoção das três árvores produzirem
contagens determinísticas e semanticamente distintas de nós visitados e
chamadas ao comparador, os valores chegarem corretamente à interface e as
invariantes estruturais continuarem aprovadas.
