# 0015 — Inserção tradicional da árvore rubro-negra

- Estado: Aceita
- Data: 2026-10-10
- Substitui: estratégia inicial de correção local da inserção rubro-negra
- Substituída por: não se aplica

## Contexto

A primeira implementação corrigia a árvore durante o retorno da inserção
recursiva, reconhecendo filhos e netos vermelhos. Ela preservava as invariantes,
mas não representava o tio do nó inserido e, depois de uma rotação, deixava a
nova raiz local vermelha e seus filhos pretos. Essa variante produzia mais
rotações que a AVL na sequência histórica usada pelo projeto.

## Decisão

A inserção rubro-negra usa os casos tradicionais definidos pela relação entre o
nó inserido, seu pai, avô e tio. O novo nó nasce vermelho e a correção começa a
partir dele:

- pai e tio vermelhos provocam somente recoloração e propagação para o avô;
- tio preto com formação triangular provoca uma rotação no pai;
- a formação em linha torna o pai preto, o avô vermelho e rotaciona o avô;
- os casos à direita são simétricos aos casos à esquerda;
- a raiz global termina preta e sem pai.

A busca da posição continua detectando duplicatas no mesmo percurso. Não será
feita uma busca prévia separada. Os filhos permanecem sob propriedade exclusiva
de `std::unique_ptr`, enquanto a ligação para o pai é não proprietária.

Cada chamada real a uma rotação primitiva incrementa o contador uma vez. Uma
inserção executa no máximo duas rotações; recolorações não contam como rotação.

## Consequências

- testes devem verificar cores e rotações dos casos de linha, triângulo e tio
  vermelho, além das invariantes gerais;
- após `30, 20, 10`, a raiz 20 é preta e os filhos 10 e 30 são vermelhos;
- uma coloração apenas válida não basta para caracterizar a implementação como
  a variante tradicional adotada;
- a remoção mantém sua correção de déficit preto, mas deve ser revalidada contra
  as topologias produzidas pela nova inserção;
- menos rotações são esperadas em cargas típicas, mas menor tempo que a AVL não
  é garantido para toda sequência, compilação ou máquina.
