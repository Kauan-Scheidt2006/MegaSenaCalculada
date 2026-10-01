---
name: experimentos-arvores
description: Implementar ou revisar comparadores, BST/AVL, métricas e testes de invariantes para os registros deste projeto. Não use para GUI, XLSX ou para redefinir como a frequência acumulada é gerada.
---

# Experimentos com árvores

Leia `AGENTS.md`. Quando precisar confirmar a chave, leia `../dominio-mega-sena/references/modelo-dominio.md`.

Mantenha BST e AVL genéricas: elas recebem um comparador e não conhecem regras da Mega-Sena. Para a chave vigente, compare primeiro `frequenciaAcumulada` e use `numero` somente quando as frequências forem iguais. `numeroConcurso` nunca é desempate.

Verifique irreflexividade, assimetria, transitividade, equivalência, travessia em ordem, política de duplicatas e invariantes da estrutura. Na AVL, inclua os casos de rotação LL, RR, LR e RL.

Não altere a ordenação para esconder duplicatas. Documente o que conta como comparação e rotação e aplique a mesma convenção às estruturas comparadas. Exclua importação XLSX e atualização de GUI dos experimentos de desempenho.

