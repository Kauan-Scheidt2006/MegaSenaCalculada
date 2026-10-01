---
name: dominio-mega-sena
description: Modelar ou revisar entidades, regras de negócio, validações e a transformação cronológica de concursos da Mega-Sena em ocorrências com frequência acumulada. Não use para GUI ou implementação interna de BST/AVL.
---

# Domínio da Mega-Sena

Leia `AGENTS.md`. Leia [references/modelo-dominio.md](references/modelo-dominio.md) quando a tarefa alterar entidades, invariantes ou a geração de registros.

Preserve as decisões vigentes enquanto o usuário não as substituir explicitamente:

- cada dezena sorteada gera um registro;
- a chave é `(frequenciaAcumulada, numero)`;
- `numeroConcurso` identifica a origem e não participa da comparação;
- relações persistentes usam identificadores estáveis, não ponteiros, referências C++ ou posições de contêiner.

No processamento histórico, valide os concursos, estabeleça a ordem cronológica, incremente o contador antes de criar a chave e emita um registro por ocorrência. Relate duplicatas ou inconsistências sem corrigi-las silenciosamente.

Separe leitura de arquivo, validação, processamento e inserção na árvore. Implemente somente o incremento pedido e teste regras alteradas com exemplos pequenos calculados manualmente.

