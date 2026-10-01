# Catálogo de agentes especializados

Este catálogo serve como ponto de roteamento. A especificação operacional de cada papel fica em um arquivo próprio em [`docs/agentes/`](agentes/README.md), evitando carregar instruções que não pertencem à tarefa atual.

## Coordenação e processo

| Agente | Acione quando |
|---|---|
| [Coordenador](agentes/coordenador.md) | for necessário delimitar, delegar e integrar um incremento |
| [Explorador](agentes/explorador.md) | faltarem fatos sobre arquivos, dependências, testes ou decisões |
| [Facilitador de requisitos](agentes/facilitador-requisitos.md) | houver ambiguidade ou alternativas que mudem o resultado |
| [Planejador](agentes/planejador.md) | uma decisão entendida precisar virar sequência verificável |
| [Arquiteto](agentes/arquiteto.md) | contratos, fronteiras ou responsabilidades forem alterados |
| [Implementador](agentes/implementador.md) | existir um incremento aprovado e pronto para execução |
| [Revisor](agentes/revisor.md) | uma mudança relevante precisar de avaliação independente |

## Domínio e dados

| Agente | Acione quando |
|---|---|
| [Validador de concursos](agentes/validador-concursos.md) | entradas da Mega-Sena precisarem ser validadas |
| [Processador histórico](agentes/processador-historico.md) | concursos válidos precisarem virar ocorrências acumuladas |
| [Analista histórico](agentes/analista-historico.md) | dados processados precisarem de análise descritiva |
| [Especialista em estruturas](agentes/especialista-estruturas.md) | comparadores, BST, AVL, métricas ou invariantes forem afetados |

## GUI desktop

| Agente | Acione quando |
|---|---|
| [Arquiteto de apresentação](agentes/arquiteto-apresentacao.md) | contratos e estados da GUI forem definidos ou alterados |
| [Especialista em experiência e acessibilidade](agentes/especialista-experiencia-acessibilidade.md) | fluxo, terminologia ou acessibilidade precisarem de revisão |
| [Especialista em execução assíncrona](agentes/especialista-execucao-assincrona.md) | operações demoradas afetarem a responsividade da GUI |
| [Especialista em testes de interface](agentes/especialista-testes-interface.md) | estados e fluxos críticos da GUI precisarem de testes |

## Regras de composição

- Use o menor conjunto de agentes capaz de concluir a tarefa.
- Prefira um único agente para trabalho curto, sequencial ou concentrado no mesmo arquivo.
- Delegue somente unidades independentes, com entrada, entrega e limites explícitos.
- O coordenador integra os resultados e permanece responsável pela resposta final.
- Implementação e aprovação independente não devem ficar com o mesmo papel quando houver impacto em regra de negócio, arquitetura ou algoritmo.
- Skills fornecem workflows; agentes fornecem propriedade temporária. O arquivo de cada agente indica as skills aplicáveis.
