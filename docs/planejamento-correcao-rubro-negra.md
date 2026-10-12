# Planejamento da correção da árvore rubro-negra

## Objetivo

Substituir a correção local atualmente usada na inserção por uma implementação
tradicional de árvore rubro-negra, baseada na relação entre nó inserido, pai,
avô e tio. A estrutura deve preservar o proprietário exclusivo dos filhos com
`std::unique_ptr`, continuar genérica e manter as métricas públicas existentes.

A implementação pode usar correção iterativa ou recursiva. A forma de controle
não define o algoritmo: o requisito é reproduzir os casos tradicionais de
recoloração, linha e triângulo, inclusive seus casos simétricos.

## Motivação

A implementação atual chama uma correção em cada ancestral durante o retorno da
inserção recursiva. Essa correção observa filhos e netos vermelhos, mas não
representa explicitamente o tio do nó inserido. Depois de uma rotação, ela
colore a nova raiz local de vermelho e os dois filhos de preto.

Esse resultado pode preservar as invariantes rubro-negras, porém não corresponde
à correção tradicional. Na sequência histórica atual, com 18.366 registros, o
comportamento observado foi:

| Estrutura | Rotações | Comparações | Altura final |
|---|---:|---:|---:|
| AVL atual | 13.524 | 405.536 | 16 |
| Rubro-negra atual | 18.261 | 455.852 | 20 |
| Rubro-negra tradicional de referência | 11.705 | 477.354 | 19 |

Os números da referência são diagnósticos para o arquivo atual, não uma regra
universal de desempenho nem um substituto para os testes de invariantes.

## Definição do comportamento tradicional

Todo nó novo nasce vermelho. Após a inserção de busca binária:

1. se o nó for a raiz, ele se torna preto;
2. se o pai for preto, nenhuma correção adicional é necessária;
3. se pai e tio forem vermelhos, pai e tio se tornam pretos, o avô se torna
   vermelho e a correção continua a partir do avô;
4. se o pai for vermelho, o tio for preto e a forma for um triângulo, uma
   rotação no pai converte o caso em linha;
5. no caso linha, o pai se torna preto, o avô se torna vermelho e uma rotação
   no avô conclui a correção local;
6. os casos com o pai à direita do avô são o espelho dos casos anteriores;
7. ao final, a raiz global é preta e não possui pai.

Ponteiros nulos são tratados como folhas pretas. Uma inserção tradicional
executa no máximo duas rotações primitivas; recolorações não incrementam o
contador de rotações.

## Escopo

### Incluído

- correção da inserção em `ArvoreRubroNegra`;
- adaptação dos testes que hoje consolidam a coloração não tradicional;
- testes explícitos para tio vermelho, linha, triângulo e casos simétricos;
- regressão da remoção, porque inserções diferentes produzem topologias e cores
  iniciais diferentes para o algoritmo de déficit preto;
- validação das métricas de rotações, comparações, altura e tempo;
- medição comparativa reproduzível entre AVL e rubro-negra.

### Fora do escopo

- copiar a busca prévia de duplicatas da classe Java, pois ela percorre a árvore
  duas vezes;
- alterar as regras de ordenação dos registros;
- prometer que a rubro-negra sempre terá menor tempo de parede que a AVL;
- alterar BST, AVL, apresentação visual ou contrato público sem necessidade
  demonstrada pelos testes;
- usar os valores exatos do arquivo histórico como prova de correção isolada.

## Arquivos previstos

```text
src/estruturas/ArvoreRubroNegra.hpp
testes/testes_estrutura_inicial.cpp
testes/testes_processamento_apresentacao.cpp
docs/decisoes/README.md
docs/decisoes/0015-insercao-tradicional-rubro-negra.md
```

O novo registro de decisão deve documentar a substituição da estratégia de
correção local pela estratégia tradicional. Ele não deve registrar resultados
temporários de benchmark como regra arquitetural.

## Etapas de implementação

### 1. Fixar testes de caracterização corretos

Antes de substituir o algoritmo, acrescentar testes que expressem a referência
tradicional e que, portanto, falhem com a implementação atual.

Casos mínimos:

| Sequência | Caso | Resultado esperado |
|---|---|---|
| `30, 20, 10` | linha esquerda | raiz 20 preta; filhos 10 e 30 vermelhos; 1 rotação |
| `10, 20, 30` | linha direita | raiz 20 preta; filhos 10 e 30 vermelhos; 1 rotação |
| `30, 10, 20` | triângulo esquerdo | raiz 20 preta; filhos vermelhos; 2 rotações |
| `10, 30, 20` | triângulo direito | raiz 20 preta; filhos vermelhos; 2 rotações |
| `20, 10, 30, 5` | tio vermelho | recoloração na quarta inserção; nenhuma rotação adicional |

Cada inserção deve ser seguida pela validação das invariantes. O teste existente
que espera os três nós pretos após `30, 20, 10` deve ser corrigido, pois ele
consolida a política atual em vez do algoritmo tradicional.

### 2. Separar inserção estrutural e correção de cores

Modificar `inserirNaSubarvore()` para realizar somente a busca da posição e a
criação do nó vermelho. A função deve informar ao chamador:

- se houve inserção;
- o endereço estável do nó criado, como ponteiro não proprietário.

A detecção de duplicata permanece no mesmo percurso. O valor original, a
quantidade, as cores, a topologia e o contador de rotações não podem mudar
quando a chave já existir.

### 3. Implementar a correção tradicional

Substituir `corrigir()` e `concluirCorrecao()` por uma correção que opere a
partir do nó inserido.

Uma implementação iterativa pode seguir diretamente pai, avô e tio. Uma versão
recursiva é aceitável se cada chamada receber ou reconstruir essas mesmas
relações, propagar o caso de recoloração para o avô e produzir exatamente as
mesmas transições de cor e rotações do algoritmo tradicional.

As rotações existentes podem ser reaproveitadas, desde que:

- a ligação proprietária do avô seja obtida antes de movê-la;
- filhos transferidos recebam o pai correto;
- a nova raiz da subárvore preserve o antigo pai externo;
- a raiz global seja atualizada quando a rotação ocorrer no topo;
- nenhum ponteiro não proprietário seja usado depois da destruição do nó a que
  apontava;
- cada chamada a uma rotação primitiva incremente o contador exatamente uma vez.

### 4. Preservar o contrato público e as métricas

Manter sem alteração de assinatura:

```cpp
bool inserir(const Valor& valor);
std::size_t quantidadeRotacoes() const noexcept;
bool invariantesPreservadas() const;
```

Após uma inserção bem-sucedida:

- incrementar a quantidade uma vez;
- garantir raiz preta e com pai nulo;
- contabilizar somente chamadas reais de rotação;
- permitir que o comparador contabilizado registre apenas as decisões do único
  percurso de inserção;
- não incluir validação de invariantes, altura ou construção visual no tempo de
  inserção.

### 5. Revalidar a remoção

A remoção não deve ser reescrita preventivamente. Executar sua suíte contra as
novas árvores produzidas pela inserção tradicional e corrigir somente falhas
demonstradas.

Acrescentar deltas do contador de rotações por remoção para verificar:

- remoção de nó vermelho;
- remoção de nó preto com filho vermelho;
- irmão vermelho;
- irmão preto com dois filhos pretos;
- sobrinho próximo vermelho;
- sobrinho distante vermelho;
- casos simétricos;
- remoção repetida ou ausente sem alteração estrutural.

Como referência do algoritmo tradicional, uma remoção executa no máximo três
rotações primitivas. Esse limite deve ser avaliado por operação usando a
diferença do contador acumulado.

### 6. Validar a sequência histórica

Com a ordem vigente do JSON:

- construir AVL e rubro-negra com os mesmos 18.366 registros;
- confirmar igualdade dos percursos em ordem e quantidade de nós;
- validar as invariantes após cada inserção em um teste estrutural dedicado ou
  em uma sequência determinística representativa;
- registrar altura, comparações e rotações finais;
- investigar qualquer divergência relevante em relação à referência de 11.705
  rotações e altura 19 antes de aceitá-la.

Para o arquivo atual e a mesma variante tradicional, a expectativa diagnóstica
é que a rubro-negra execute menos rotações que a AVL. Essa comparação não deve
ser transformada em invariante universal para outros arquivos ou ordens.

### 7. Medir desempenho sem confundir ruído com correção

Executar a comparação temporal separadamente dos testes funcionais:

- usar compilação otimizada;
- realizar aquecimento antes das amostras válidas;
- reconstruir uma árvore nova em cada repetição;
- alternar ou sortear a ordem de execução de AVL e rubro-negra;
- coletar várias repetições e apresentar mediana, além de uma medida de
  dispersão;
- manter o comparador instrumentado de forma idêntica nas duas estruturas.

O resultado esperado é uma tendência coerente com o número de operações, mas
`tempoRubroNegra < tempoAvl` não é critério funcional de aprovação. AVL pode
ser mais rápida em uma máquina ou sequência específica mesmo com mais
reorganizações, devido a altura, alocação, cache e detalhes da implementação.

## Verificações

Executar, nesta ordem:

1. testes mínimos de linha, triângulo e recoloração;
2. testes de duplicata e contador de rotações;
3. sequências ordenadas e permutações determinísticas, validando invariantes
   após cada inserção;
4. testes de remoção rubro-negra;
5. testes de construção do modelo e transporte das métricas;
6. suíte relacionada de estrutura e processamento;
7. diagnóstico sobre o JSON completo;
8. benchmark otimizado, sem limites rígidos de tempo nos testes automatizados.

Antes dessas verificações, resolver a divergência já existente das métricas
periódicas: as chamadas estão comentadas nas estruturas, enquanto os testes
esperam `metricasPeriodicasCalculadas() == true`.

## Critérios de aceitação

- todos os cinco casos mínimos apresentam cores, raiz e quantidade de rotações
  tradicionais;
- toda inserção individual executa zero, uma ou duas rotações;
- tio vermelho provoca recoloração sem rotação;
- raiz preta, ausência de pai na raiz, ausência de dois vermelhos consecutivos,
  mesma altura preta em todos os caminhos e ordem de busca são preservadas;
- duplicatas não alteram quantidade, cores, topologia ou rotações;
- percursos e métricas apresentados correspondem à árvore efetivamente criada;
- remoção continua preservando todas as invariantes e respeita o limite
  tradicional de rotações por operação;
- testes relacionados compilam e passam sem depender de limites temporais;
- no diagnóstico com o JSON atual, resultados diferentes da referência são
  explicados antes da aprovação;
- uma revisão independente examina transições de cor, propriedade dos
  `unique_ptr`, ponteiros de pai e contagem de rotações.

## Riscos e cuidados

- mover um `unique_ptr` não move o objeto apontado, mas pode destruir um nó se a
  propriedade não for religada corretamente;
- ponteiros de pai incorretos podem permanecer invisíveis em percurso em ordem e
  só falhar durante correção ou remoção;
- testes apenas de invariantes aceitam múltiplas colorações válidas e não
  comprovam que o algoritmo tradicional foi implementado;
- testar somente topologia também é insuficiente: cores e rotações fazem parte
  do comportamento requerido;
- a classe Java serve como referência da correção de inserção, mas sua busca
  prévia de duplicatas e a ausência de remoção não devem ser copiadas;
- comparação de desempenho entre Java e C++ não isola o algoritmo e não deve
  orientar a aceitação.

## Condição de conclusão

A correção estará concluída quando a inserção seguir os casos tradicionais,
seus testes de caracterização e invariantes estiverem aprovados, a remoção não
regredir, as métricas refletirem as operações reais e o conjunto histórico
mostrar resultados explicáveis e compatíveis com a nova política de
balanceamento.
