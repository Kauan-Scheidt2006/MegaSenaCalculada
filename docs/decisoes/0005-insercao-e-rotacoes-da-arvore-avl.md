# 0005 — Inserção e rotações da árvore AVL

- Estado: Aceita
- Data: 2026-09-27
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

A arquitetura inicial da AVL definiu nós concretos com altura armazenada, mas
adiou a inserção até que atualização de altura, balanceamento e rotações
pudessem ser implementados em conjunto. Também é necessário preservar a mesma
política de comparação e rejeição de duplicatas adotada pelo domínio.

## Decisão

A inserção percorre recursivamente referências a `std::unique_ptr<NoAvl>` e
rebalanceia cada ancestral no retorno da recursão. Um valor é duplicado quando
nenhum dos sentidos do comparador é verdadeiro; nesse caso, o valor original,
a quantidade, as alturas e a topologia são preservados.

A altura de um ponteiro nulo é zero e a altura de uma folha é um. O fator de
balanceamento usa `int` e é calculado por `alturaEsquerda - alturaDireita`, com
cada altura convertida para `int` antes da subtração.

As operações primitivas são `rotacionarDireita` e `rotacionarEsquerda`. Os
casos são denominados conforme a sequência executada:

- RD: rotação à direita;
- RE: rotação à esquerda;
- ED: esquerda no filho esquerdo e direita na raiz;
- DE: direita no filho direito e esquerda na raiz.

ED e DE são composições das duas operações primitivas, não implementações de
rotação duplicadas.

## Consequências

- Toda inserção bem-sucedida preserva a ordem de busca, as alturas armazenadas
  e fatores de balanceamento entre menos um e um.
- RD e RE correspondem a uma rotação primitiva; ED e DE correspondem a duas.
- A quantidade aumenta somente quando um novo nó é criado.
- O percurso em ordem e a altura pública permitem verificar o comportamento
  sem expor os nós internos.
- Os casos RD, RE, ED e DE, duplicatas e uma sequência ordenada devem permanecer
  cobertos por testes.
