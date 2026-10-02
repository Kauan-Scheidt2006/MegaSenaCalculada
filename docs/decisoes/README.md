# Decisões do projeto

Esta pasta registra decisões duradouras que orientam o programa, o código-fonte e sua evolução. Ela evita acumular detalhes e histórico no `AGENTS.md`.

## Quando registrar

Crie ou atualize um registro quando uma escolha:

- afetar regras de negócio, contratos, arquitetura ou uma convenção técnica relevante;
- tiver alternativas plausíveis;
- precisar ser conhecida em trabalhos futuros;
- mudar ou substituir uma decisão já vigente.

Não registre conversas, planos temporários, detalhes evidentes da implementação nem preferências sem consequência prática. Invariantes implementadas também devem permanecer cobertas por testes.

## Como registrar

1. Copie `modelo.md` para um arquivo numerado com nome descritivo, como `0002-nome-da-decisao.md`.
2. Use números sequenciais e não renumere arquivos existentes.
3. Registre contexto, decisão e consequências; inclua alternativas somente quando ajudarem a entender a escolha.
4. Depois de aceita, não reescreva a história da decisão. Para substituí-la, crie um novo registro e conecte os dois documentos.
5. Atualize o índice abaixo.

## Estados

- **Proposta**: ainda depende de aprovação.
- **Aceita**: regra vigente do projeto.
- **Substituída**: deixou de vigorar e aponta para a decisão sucessora.
- **Rejeitada**: foi considerada, mas não adotada.

## Índice

| Número | Decisão | Estado |
|---|---|---|
| [0001](0001-modelo-de-registro-e-ordenacao.md) | Modelo de registro e ordenação das ocorrências | Aceita |
| [0002](0002-idioma-e-convencoes-de-nomenclatura.md) | Idioma e convenções de nomenclatura do código | Aceita |
| [0003](0003-governanca-escalavel-de-agentes.md) | Governança escalável de agentes | Aceita |
| [0004](0004-arquitetura-inicial-da-arvore-avl.md) | Arquitetura inicial da árvore AVL | Aceita |
| [0005](0005-insercao-e-rotacoes-da-arvore-avl.md) | Inserção e rotações da árvore AVL | Aceita |
| [0006](0006-preparacao-do-historico-para-processamento.md) | Preparação do histórico para processamento | Aceita |
| [0007](0007-encapsulamento-da-leitura-xlsx.md) | Encapsulamento da leitura XLSX | Substituída |
| [0008](0008-arquivo-json-como-fonte-dos-concursos.md) | Arquivo JSON como fonte dos concursos | Aceita |
| [0009](0009-arquitetura-inicial-da-interface-qt.md) | Arquitetura inicial da interface Qt | Aceita |
| [0010](0010-estilos-visuais-exclusivamente-em-qss.md) | Estilos visuais exclusivamente em QSS | Aceita |
