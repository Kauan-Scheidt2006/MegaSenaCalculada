# 0003 — Governança escalável de agentes

- Estado: Aceita
- Data: 2026-09-27

## Contexto

O projeto já distinguia agente, skill, decisão e guideline, mas mantinha todos os papéis especializados em um catálogo monolítico. Isso dificultava fornecer a cada subagente apenas o contexto necessário e não padronizava entradas, limites, entregas e handoffs.

## Decisão

Adotar quatro camadas com pontos de verdade distintos:

1. `AGENTS.md` permanece curto e contém somente regras globais.
2. `docs/agentes-especializados.md` funciona como índice de roteamento.
3. Cada papel tem contrato próprio em `docs/agentes/`, consultado sob demanda.
4. Skills continuam representando workflows reutilizáveis, e não cargos.

Toda delegação deve ser delimitada por objetivo, contexto mínimo, escopo, propriedade de arquivos, restrições e entrega. O coordenador integra resultados e evita edições concorrentes. A execução multiagente é reservada a trabalhos independentes; tarefas pequenas ou sequenciais permanecem com um agente.

## Consequências

- Novos papéis podem ser adicionados sem aumentar o contexto global.
- A granularidade dos contratos reduz sobreposição e melhora handoffs.
- Existe mais documentação para manter; o catálogo e os contratos devem ser revisados juntos.
- Os arquivos de papel são convenções do repositório e não criam agentes automaticamente.

## Referências

- [Agent definitions](https://developers.openai.com/api/docs/guides/agents/define-agents)
- [Multi-agent](https://developers.openai.com/api/docs/guides/responses-multi-agent)
- [Skills](https://developers.openai.com/api/docs/guides/tools-skills)

