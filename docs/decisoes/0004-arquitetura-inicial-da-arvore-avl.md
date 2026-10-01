# 0004 — Arquitetura inicial da árvore AVL

- Estado: Aceita
- Data: 2026-09-27
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

O projeto iniciará uma implementação genérica de árvore AVL. Seus nós precisam
reutilizar o estado estrutural formado pelo valor e pelos filhos, sem armazenar
os filhos como ponteiros para uma classe-base nem recorrer a métodos virtuais,
conversões de tipo ou RTTI. O incremento inicial não inclui inserção,
balanceamento ou rotações.

## Decisão

`NoArvoreBase<Valor, TipoNo>` armazena o valor e os filhos esquerdo e direito.
O tipo concreto dos filhos é recebido como parâmetro de template.

`NoAvl<Valor>` deriva de `NoArvoreBase<Valor, NoAvl<Valor>>` e acrescenta a
altura, inicialmente igual a um. Assim, seus filhos têm o tipo estático
`std::unique_ptr<NoAvl<Valor>>`.

`ArvoreAvl<Valor, Comparador>` possui exclusivamente uma raiz do tipo
`NoAvl<Valor>`, a quantidade de elementos e o comparador. A classe não oferece
inserção até que atualização de altura, rotações e preservação das invariantes
AVL sejam implementadas em conjunto.

Os nós não usam métodos virtuais e permanecem independentes do domínio da
Mega-Sena e da apresentação Qt.

## Consequências

- O compilador garante que todos os descendentes de um nó AVL são nós AVL.
- Operações futuras não precisarão de `static_cast`, `dynamic_cast` ou
  destrutores virtuais nos nós.
- A árvore poderá ser instanciada com `Registro` e `ComparadorRegistro` sem
  conhecer regras da Mega-Sena.
- O próximo incremento deverá implementar inserção, atualização de alturas e
  os quatro casos de rotação como uma unidade capaz de preservar a invariante
  AVL.
- Enquanto esse incremento não existir, `ArvoreAvl` representa apenas o estado
  vazio e não deve ser usada nos experimentos de desempenho ou na GUI.
