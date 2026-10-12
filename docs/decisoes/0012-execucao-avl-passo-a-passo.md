# 0012 — Execução da AVL passo a passo

- Estado: Aceita
- Data: 2026-10-06
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

A exploração visual precisa demonstrar a construção da AVL uma transformação
por vez, inclusive as duas rotações dos casos LR e RL. Suspender chamadas
recursivas com espera, `sleep` ou um laço de eventos local manteria a pilha
aberta, bloquearia a interface e acoplaria a estrutura genérica ao Qt.

## Decisão

O projeto mantém a `ArvoreAvl` final existente e oferece uma
`ArvoreAvlPassoAPasso` separada, genérica e sem Qt. A versão didática representa
explicitamente as fases e o caminho da inserção. Cada chamada a `avancar()`
realiza no máximo uma transição observável, sem esperar pela interface.

Descidas, criação do nó, atualização de altura e cada rotação são eventos
individuais. Rotações duplas produzem dois eventos. A aplicação controla a
sequência de registros e converte a topologia corrente em modelos de
apresentação sem expor nós, ponteiros ou propriedade interna à GUI.

A execução manual ocorre por sinais do usuário. A reprodução automática usa
`QTimer` na thread visual para enfileirar avanços curtos. Não são permitidos
`sleep`, espera por condição ou acesso a widgets pela estrutura.

## Consequências

- A AVL didática deve terminar com a mesma topologia, altura, travessia e
  quantidade de rotações da AVL final para a mesma sequência.
- Casos LL, RR, LR e RL devem permanecer cobertos por testes de equivalência.
- A interface recebe somente modelos e valores estáveis, nunca nós ou ponteiros.
- Um estado intermediário de rotação dupla pode estar temporariamente
  desbalanceado, mas deve sempre formar uma árvore topologicamente válida.
- O modelo visual pode ser reconstruído após cada passo; zoom e posição devem
  ser preservados para evitar saltos durante a demonstração.

## Alternativas consideradas

- Pausar dentro de `inserirNaSubarvore` ou `rebalancear`: rejeitado porque
  bloquearia a thread visual e manteria estado implícito na pilha.
- Alterar a AVL final para sempre emitir eventos: rejeitado para não impor custo
  e complexidade didática ao experimento usado para métricas finais.
