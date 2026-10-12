# Planejamento das métricas de construção das árvores

## Objetivo

Medir a construção integral de cada árvore final com quatro resultados:

1. tempo para executar `inserir()` para todos os registros;
2. total de comparações feitas durante essas inserções;
3. altura da árvore ao final da construção;
4. total de rotações realmente executadas para balancear a árvore durante todas
   as inserções.

O contrato será aplicado à BST, à AVL final e à árvore rubro-negra quando ela
for integrada à interface. A AVL passo a passo permanece fora desta medição.

## Definições

### Tempo total de inserção

É o tempo decorrido entre o instante imediatamente anterior à primeira chamada
de `inserir()` e o instante imediatamente posterior ao retorno da última
chamada.

Será medido com `std::chrono::steady_clock` no helper `construirArvore()`, em
`src/aplicacao/arvore/DetalhesConstrucaoModelo.hpp`.

O intervalo inclui somente o laço que repassa os registros à árvore. Portanto,
inclui busca da posição, comparações, alocação de nós e correções próprias da
estrutura, mas exclui leitura do JSON, processamento histórico, cálculo
posterior da altura, geração do modelo visual e renderização da interface.

O valor será armazenado em nanossegundos inteiros e convertido para uma unidade
legível somente na interface.

### Total de comparações

Cada chamada ao comparador durante `inserir()` conta como uma comparação. O
contador atual de `ComparadorRegistroContabilizado`, em
`DetalhesConstrucaoModelo.hpp`, continuará sendo a fonte dessa métrica.

A contagem começa em zero antes da primeira inserção e termina junto com a
última. Percursos posteriores, validação de invariantes, cálculo de altura e
construção do modelo visual não podem alterar esse valor.

### Altura final

É a altura da árvore depois que todos os registros forem inseridos. Mantém-se a
convenção vigente: árvore vazia tem altura zero e folha tem altura um.

| Classe | Forma de cálculo |
|---|---|
| `ArvoreBinariaBusca` | percurso recursivo já existente em `altura()` |
| `ArvoreAvl` | altura armazenada e atualizada nos nós |
| `ArvoreRubroNegra` | percurso recursivo a ser acrescentado na integração |

A altura é uma fotografia do estado final; ela não será somada a cada inserção.

### Total de balanceamentos por rotações

Nesta etapa, “soma de todos os balanceamentos” significa **quantidade acumulada
de rotações realmente executadas durante a inserção de todos os nós**. O
contador não será incrementado ao entrar em `rebalancear()` ou `corrigir()`,
porque essas funções também podem concluir que nenhuma modificação é necessária.

| Estrutura | O que incrementa o contador |
|---|---|
| BST | não possui rotação; informa zero |
| AVL | cada execução de `rotacionarEsquerda()` ou `rotacionarDireita()` |
| Rubro-negra | cada execução de `rotacionarEsquerda()` ou `rotacionarDireita()` |

Cada rotação simples soma um. Uma correção dupla executa duas funções de rotação
e, portanto, soma dois. Verificações sem rotação não alteram a métrica;
recoloração sem rotação também não a altera.

Cada árvore final expõe:

```cpp
[[nodiscard]] std::size_t quantidadeRotacoes() const noexcept;
```

## Responsabilidade por classe

| Métrica | Responsável | Motivo |
|---|---|---|
| Tempo total | `construirArvore()` | delimita igualmente o laço das três árvores |
| Comparações | `ComparadorRegistroContabilizado` | aplica a mesma convenção a todas |
| Altura | classe concreta da árvore | possui acesso à raiz e aos nós internos |
| Balanceamentos | funções de rotação da árvore | incrementam somente quando a estrutura é modificada |

## Contrato do modelo

`apresentacao::arvore::MetricasArvore`, em
`src/interface/apresentacao/arvore/ModeloVisualArvore.hpp`, transportará:

```cpp
struct MetricasArvore {
    std::size_t quantidadeNos = 0;
    std::size_t altura = 0;
    std::size_t comparacoes = 0;
    std::size_t rotacoes = 0;
    std::uint64_t tempoInsercaoNanossegundos = 0;
};
```

O modelo apenas transporta valores. `quantidadeNos` permanece para
contextualizar as métricas.

## Construção e consolidação

O helper da aplicação produzirá:

```cpp
template<typename Arvore>
struct ResultadoConstrucaoArvore {
    Arvore arvore;
    std::uint64_t tempoInsercaoNanossegundos = 0;
    std::size_t comparacoes = 0;
};
```

Ordem obrigatória:

1. zerar o contador de comparações;
2. construir a árvore com o comparador instrumentado;
3. iniciar o relógio;
4. chamar `inserir()` para cada registro, na ordem recebida;
5. encerrar o relógio;
6. capturar tempo e comparações;
7. consultar quantidade, altura e rotações da árvore;
8. construir o modelo visual.

`ConstrutorApresentacaoBst`, `ConstrutorApresentacaoAvl` e o futuro
`ConstrutorApresentacaoRubroNegra` usarão esse mesmo fluxo.

## Fluxo até a interface

```text
Registros processados
        |
        v
construirArvore()
  tempo + comparações
        |
        v
árvore concreta
  quantidade + altura + rotações
        |
        v
criarModeloVisual()
        |
        v
ModeloVisualArvore::MetricasArvore
        |
        v
Aplicacao -> PaginaArvore -> PainelVisualizacaoArvore
```

`PainelVisualizacaoArvore` somente formata os valores:

```text
Nós: 18.000 • Tempo de inserção: 4,72 ms • Comparações: 286.410
Altura final: 18 • Balanceamentos (rotações): 9.304
```

## Etapas

### Etapa 1 — Contadores das estruturas

Arquivos:

```text
src/estruturas/ArvoreBinariaBusca.hpp
src/estruturas/ArvoreAvl.hpp
src/estruturas/ArvoreRubroNegra.hpp
testes/testes_estrutura_inicial.cpp
```

Entregas:

- expor `quantidadeRotacoes()` nas árvores finais;
- incrementar dentro de `rotacionarEsquerda()` e `rotacionarDireita()` na AVL;
- incrementar dentro de `rotacionarEsquerda()` e `rotacionarDireita()` na
  rubro-negra;
- manter zero na BST;
- completar `altura()` na rubro-negra;
- verificar que inserção duplicada não incrementa o contador.

### Etapa 2 — Tempo e consolidação

Arquivos:

```text
src/aplicacao/arvore/DetalhesConstrucaoModelo.hpp
src/aplicacao/arvore/ConstrutorApresentacaoBst.cpp
src/aplicacao/arvore/ConstrutorApresentacaoAvl.cpp
src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.cpp
testes/testes_processamento_apresentacao.cpp
```

Entregas:

- cronometrar somente o laço de inserção;
- manter a convenção única de comparação;
- consolidar as métricas no modelo;
- não testar limites rígidos de tempo.

A parte rubro-negra depende das fases 12.1 e 12.2 de
`docs/planejamento.md`.

### Etapa 3 — Modelo e interface

Arquivos:

```text
src/interface/apresentacao/arvore/ModeloVisualArvore.hpp
src/interface/paginas/arvore/PainelVisualizacaoArvore.cpp
src/interface/paginas/arvore/PainelVisualizacaoArvore.hpp
testes/testes_modelo_apresentacao_arvore.cpp
testes/testes_interface_inicial.cpp
```

Entregas:

- transportar os valores sem recálculo;
- apresentar a mesma lista para toda árvore final;
- formatar o tempo com unidade legível;
- atualizar a descrição acessível;
- preservar o comportamento da AVL passo a passo.

### Etapa 4 — Verificação

- compilar os alvos afetados;
- executar testes de estrutura, processamento, modelo e interface;
- inspecionar BST e AVL e, quando integrada, a rubro-negra;
- obter revisão independente dos contadores.

## Cuidados

- O tempo é da execução instrumentada: o contador de comparações é incrementado
  dentro do intervalo medido.
- O cálculo de altura ocorre depois do encerramento do relógio.
- Validar invariantes não pode alterar as comparações registradas.
- A interface deve explicar que o total de balanceamentos corresponde ao total
  de rotações efetivamente executadas.
- A ordem dos registros continua sendo a ordem do JSON, conforme a decisão
  0011.

## Condição de conclusão

Toda árvore de construção integral disponível na interface apresenta o tempo
para inserir todos os registros, o total de comparações dessas inserções, a
altura final e o total acumulado de rotações de balanceamento. A AVL passo a
passo permanece explicitamente excluída.
