# 0011 — Ordem do JSON nos experimentos de árvores

- Estado: Aceita
- Data: 2026-10-06
- Substitui: [0006](0006-preparacao-do-historico-para-processamento.md)
- Substituída por: não se aplica

## Contexto

O formato final de uma BST depende da sequência de inserção. Para apresentar o
estado realmente produzido pela fonte de dados e compará-lo futuramente com a
AVL, o experimento precisa preservar a sequência observável no arquivo JSON.
A normalização anterior por número do concurso e por dezena escondia esse efeito.

## Decisão

O processamento percorre os concursos na ordem em que aparecem no array do
`concursos.json` e, dentro de cada concurso, percorre as dezenas na ordem em que
aparecem no respectivo array. Cada dezena tem sua frequência incrementada antes
da emissão do `Registro`, e os registros são inseridos na árvore nessa mesma
sequência.

Todos os concursos são validados antes que o resultado seja disponibilizado.
Cada concurso deve possuir identificador positivo e exatamente seis dezenas
distintas no intervalo de 1 a 60. Uma falha invalida o processamento completo.

BST e AVL devem receber a mesma sequência e usar o mesmo comparador. As métricas
de cada estrutura descrevem o estado final após todas as inserções; renderização
e leitura do JSON não fazem parte das medições algorítmicas.
Cada chamada ao comparador conta como uma comparação; na BST, rotações são
sempre zero.

## Consequências

- Alterar a ordem física dos concursos ou das dezenas no JSON pode alterar a
  topologia e as métricas das árvores.
- A raiz da BST corresponde ao primeiro registro emitido pelo arquivo.
- A comparação futura com a AVL terá uma entrada idêntica e reproduzível para
  uma versão específica do JSON.
- A apresentação deve informar explicitamente que usa a ordem do arquivo JSON.
