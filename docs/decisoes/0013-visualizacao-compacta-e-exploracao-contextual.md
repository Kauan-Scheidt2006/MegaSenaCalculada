# 0013 — Visualização compacta e exploração contextual das árvores

- Estado: Aceita
- Data: 2026-10-10
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

A disposição baseada no índice do percurso em ordem reservava uma coluna para
cada nó. Em árvores grandes ou desbalanceadas, isso produzia cenas muito largas,
ligações extensas e uma visão enquadrada na qual os dados dos nós ficavam
ilegíveis. Ao ampliar, o usuário perdia a relação do nó examinado com a raiz.

## Decisão

A cena utilizará uma disposição hierárquica compacta. Cada pai será centralizado
em relação aos filhos e as subárvores serão separadas a partir de seus contornos,
preservando uma distância mínima entre nós. Cadeias com um único filho crescerão
principalmente na vertical.

A exploração oferecerá detalhamento progressivo, destaque do caminho entre o nó
selecionado e a raiz, modo de foco com contexto descendente e recolhimento de
subárvores. Seleção, foco, zoom e subárvores recolhidas são estado da interface e
não alteram `ModeloVisualArvore` nem as estruturas concretas.

Os recursos críticos continuarão disponíveis por teclado. Cor e opacidade não
serão a única fonte de informação: seleção e foco continuarão identificados pela
borda, pelo foco do item e pelas descrições acessíveis.

## Consequências

- o cálculo de disposição permanece separado de `QGraphicsView`;
- árvores degeneradas deixam de consumir largura proporcional à quantidade de nós;
- o texto dos nós é reduzido progressivamente quando não há escala para leitura;
- a visualização mantém índices de parentesco derivados das ligações do modelo;
- recolher um ramo altera apenas a visibilidade dos itens e os limites da cena;
- testes da disposição priorizam invariantes geométricas e não a pintura.

## Alternativas consideradas

- **Apenas reduzir o espaçamento:** rejeitada porque não evita colisões e mantém a
  largura proporcional ao número total de nós.
- **Layout radial como padrão:** rejeitado porque dificulta a leitura ordenada da
  árvore binária e apenas redistribui a necessidade de zoom.
- **Remover ramos fora do foco:** rejeitado como comportamento padrão porque faria
  o usuário perder a noção da estrutura completa. Eles são atenuados ou recolhidos
  somente por ação explícita.
