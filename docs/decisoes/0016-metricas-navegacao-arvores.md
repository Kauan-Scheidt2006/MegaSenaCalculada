# 0016 — Métricas de navegação das árvores

- Estado: Aceita
- Data: 2026-10-11
- Substitui: contagem única de comparações em busca e remoção
- Substituída por: não se aplica

## Contexto

Busca e remoção informavam um campo chamado `comparacoes`, mas o contador era
incrementado uma vez por nó visitado. Como o comparador de ordem estrita pode
ser chamado uma ou duas vezes em cada nó, o nome não correspondia ao valor e
era incompatível com a convenção usada nas métricas de inserção.

## Decisão

BST, AVL e rubro-negra usam `MetricasNavegacaoArvore`, com contadores separados
para nós visitados e chamadas reais ao comparador.

Na busca, cada nó não nulo inspecionado conta uma visita. Na remoção, também
contam os nós percorridos para localizar o sucessor quando o alvo possui dois
filhos. O ponteiro nulo final, rotações, recolorações, atualizações de altura e
ancestrais processados novamente durante a manutenção não acrescentam visitas.

Cada execução de `comparador_(a, b)` incrementa `comparacoes`. Os overloads
instrumentados zeram ambos os campos no início. Os overloads simples continuam
disponíveis e descartam as métricas.

## Consequências

- os resultados de busca e remoção transportam `nosVisitados` e `comparacoes`;
- a interface apresenta os dois valores sem recálculo;
- uma remoção com dois filhos pode ter mais visitas que comparações adicionais,
  porque localizar o sucessor não exige comparar chaves;
- rotações permanecem em sua métrica própria;
- tempos individuais continuam sendo amostras e não substituem os contadores
  determinísticos na comparação das estruturas.
