# Levantamento de melhorias da governança de IA

Este backlog registra ações organizacionais ainda não implementadas. Ele não autoriza mudanças de arquitetura por si só.

## Diagnóstico concluído

- O `AGENTS.md` está conciso e separa regras globais de decisões e skills.
- As quatro skills ativas têm escopos claros e complementares.
- Havia diretórios legados vazios em `.agents/skills/`; convém removê-los depois de confirmar que não são usados fora do repositório.
- O catálogo de agentes não possuía contratos uniformes nem handoffs explícitos; isso foi corrigido em `docs/agentes/`.
- Não há automação que verifique links, front matter das skills ou consistência do índice de decisões.
- Não há registro leve de métricas para saber se delegações melhoram tempo, qualidade ou retrabalho.

## Prioridade alta

1. **Adicionar validação documental no CI.** Verificar links locais, UTF-8, índice de decisões, existência dos arquivos apontados pelo catálogo e front matter de cada `SKILL.md`.
2. **Criar checklist de delegação executável.** Validar que tarefas multiagente informem objetivo, escopo, arquivos, restrições e entrega; começar como template Markdown antes de automatizar.
3. **Definir gate de revisão por risco.** Classificar mudanças como baixa, média ou alta e tornar explícito quando a revisão independente é obrigatória.
4. **Eliminar legado de skills.** Confirmar e remover os diretórios vazios `mega-sena-*` e `tree-experiments`, evitando ambiguidade de descoberta.

## Prioridade média

5. **Criar cenários de avaliação das skills.** Manter pequenos casos positivos e negativos para validar roteamento, limites e qualidade das quatro skills.
6. **Padronizar relatório de revisão.** Usar gravidade, evidência, impacto e correção sugerida, sem transformar toda revisão em documento persistente.
7. **Manter mapa de ownership por incremento.** Registrar no plano temporário quais arquivos pertencem a cada agente quando houver paralelismo.
8. **Medir a governança.** Acompanhar por amostragem: conflitos de edição, retrabalho após revisão, testes omitidos e delegações sem benefício.

## Prioridade baixa ou futura

9. **Adicionar `AGENTS.md` por subdiretório somente quando necessário.** Usar instruções hierárquicas se `src/dominio`, GUI ou infraestrutura adquirirem regras locais estáveis; não antecipar arquivos vazios.
10. **Avaliar saída estruturada para automação externa.** Se o projeto adotar Agents API ou SDK, definir schemas para achados de revisão e planos, além de guardrails e observabilidade.
11. **Versionar prompts somente quando houver runtime próprio.** Os papéis atuais são documentação; migrá-los para configurações executáveis apenas quando existir um consumidor real.
12. **Revisar a governança periodicamente.** Remover regras redundantes e unir papéis que não demonstrem uso distinto.

## Critérios para considerar a governança saudável

- o agente lê apenas o contexto necessário à tarefa;
- cada regra possui um ponto de verdade;
- delegações independentes não disputam arquivos;
- decisões duradouras são rastreáveis;
- skills podem ser testadas com exemplos;
- revisão e verificação são proporcionais ao risco.

