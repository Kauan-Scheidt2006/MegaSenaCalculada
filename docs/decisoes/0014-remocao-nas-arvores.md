# 0014 — Remoção nas árvores e métricas da operação

- Estado: Aceita
- Data: 2026-10-10
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

As implementações de árvore binária de busca, AVL e rubro-negra precisam
remover chaves sem exigir atribuição de `Valor`. A interface também precisa
receber o resultado da operação, a quantidade de comparações e sua duração.

## Decisão

Os casos com dois filhos usam transplante estrutural: o sucessor em ordem é
extraído e seus `std::unique_ptr` são movidos para a posição removida. Valores
não são copiados entre nós.

A BST apenas religa os filhos. A AVL atualiza alturas e rebalanceia os
ancestrais afetados. A rubro-negra mantém uma ligação não proprietária para o
pai em cada nó, usa transplante, recoloração e rotações para corrigir o déficit
preto e conserva `std::unique_ptr` como único proprietário dos filhos.

Cada árvore oferece `remover(valor)` e `remover(valor, comparacoes)`. Uma
comparação corresponde à visita de um nó durante a localização da chave, a
mesma convenção usada pela busca. A camada de aplicação mede a chamada inteira
com `std::chrono::steady_clock` e devolve sucesso, comparações e nanossegundos.

## Consequências

- `Valor` não precisa oferecer operador de atribuição para ser removível.
- Quantidade só diminui em remoções bem-sucedidas.
- Rotações de remoção integram o contador acumulado de rotações.
- As métricas periódicas continuam descrevendo inserções e não são reescritas.
- A AVL passo a passo permanece dedicada à inserção; remoção didática exige
  eventos e fases próprios em um incremento separado.
- Testes de remoção devem cobrir os casos estruturais, balanceamento AVL,
  invariantes rubro-negras e o contrato de métricas da aplicação.
