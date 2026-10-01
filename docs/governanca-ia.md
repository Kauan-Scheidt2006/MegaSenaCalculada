# Governança de IA — versão 2

## Objetivo

Esta governança define como agentes de IA colaboram no desenvolvimento incremental do projeto. Ela não exige que todos os papéis sejam usados em toda tarefa.

## Estruturas principais

### Guidelines permanentes

O `AGENTS.md` contém regras aplicáveis a todo trabalho: idioma, escopo, invariantes, limites arquiteturais, verificação e roteamento para skills.

### Agentes

Agentes são executores temporários com um papel, uma entrada e uma entrega delimitadas. O agente coordenador cria subagentes somente quando houver trabalho independente, especialização relevante ou necessidade de revisão sem viés.

O roteamento está em `docs/agentes-especializados.md` e cada contrato de papel está em `docs/agentes/`. Consulte apenas o papel selecionado para a tarefa. Esses papéis são uma convenção de governança do projeto; não são carregados automaticamente como skills.

### Skills

Skills descrevem procedimentos especializados e repetíveis:

- `dominio-mega-sena`: entidades, validação e processamento histórico;
- `experimentos-arvores`: comparadores, BST/AVL, métricas e invariantes;
- `analise-historica-mega-sena`: análise descritiva de concursos e dezenas;
- `arquitetura-gui-desktop`: arquitetura e qualidade da apresentação.

Não se cria uma skill apenas para representar um cargo. Novas skills exigem um workflow concreto, repetível e com limites diferentes das existentes.

### Decisões e planos

- Decisões duradouras devem ser registradas e indexadas em `docs/decisoes/` conforme as regras dessa pasta.
- O `AGENTS.md` aponta para as decisões, mas não reproduz seu conteúdo nem seu histórico.
- Planos só devem ser persistidos quando precisarem sobreviver à conversa.
- Testes são a especificação executável das invariantes implementadas.

## Fluxo de um incremento

```text
Pedido
  → exploração
  → discussão, se houver decisão aberta
  → planejamento
  → revisão arquitetural, se houver mudança de fronteira
  → implementação especializada
  → testes e verificação
  → revisão independente
  → integração pelo coordenador
```

Etapas sem utilidade concreta podem ser omitidas. Uma alteração pequena pode seguir diretamente de exploração para implementação, verificação e revisão.

## Critérios de delegação

Delegue quando houver tarefas independentes, conhecimento especializado, decisão de alto impacto ou necessidade de revisão independente.

Não delegue quando a alteração for local e evidente, quando especialistas precisariam editar o mesmo arquivo em paralelo ou quando ainda faltar uma decisão que bloqueia todo o trabalho.

O coordenador define propriedade temporária dos arquivos antes de qualquer edição paralela.

Cada delegação deve declarar objetivo, contexto mínimo, escopo, arquivos sob propriedade, restrições e formato da entrega. O coordenador continua responsável por integrar e verificar o resultado.

Como referência operacional, mantenha no máximo três subagentes ativos na maioria dos trabalhos. Esse número não é uma meta: use menos quando os trabalhos não forem independentes. Subagentes podem criar descendentes somente quando o coordenador autorizar a decomposição ou quando isso estiver explícito na tarefa.

## Matriz de artefatos

| Artefato | Conteúdo | Carregamento |
|---|---|---|
| `AGENTS.md` | regras globais, curtas e sempre relevantes | automático no escopo do repositório |
| `docs/agentes/*.md` | contrato de um papel especializado | sob demanda pelo coordenador |
| `.agents/skills/*/SKILL.md` | workflow repetível e recursos de apoio | descoberto pela descrição; instruções lidas quando aplicável |
| `docs/decisoes/*.md` | escolhas duradouras e suas consequências | sob demanda conforme o impacto |
| testes | invariantes executáveis | na verificação do incremento |

Evite copiar a mesma regra entre esses artefatos. Prefira um ponto de verdade e links contextuais.

## Gates

### Entendimento

- objetivo e itens fora do escopo explícitos;
- termos do domínio sem ambiguidade relevante;
- critérios de aceitação observáveis.

### Arquitetura

Aplicável quando contratos ou fronteiras mudam:

- domínio independente de GUI e XLSX;
- árvores genéricas e orientadas por comparador;
- impacto e alternativas relevantes registrados.

### Implementação

- incremento pequeno;
- arquivos e responsabilidades definidos;
- nenhum conflito de edição entre agentes.

### Verificação

- compilação sem novos avisos relevantes;
- testes do incremento aprovados;
- invariantes afetadas cobertas;
- exemplo manual validado quando houver processamento ou comparação.

### Revisão

- revisão independente quando proporcional ao risco;
- código próprio em português;
- nenhuma previsão lotérica sugerida;
- documentação atualizada quando uma decisão permanente mudar.

## Governança da própria governança

Revise estes documentos quando houver sobreposição recorrente, conflito entre agentes ou uma nova área estável. Remova regras que deixarem de mudar decisões. Use o levantamento em `docs/melhorias-governanca-ia.md` para evolução incremental e registre mudanças estruturais em `docs/decisoes/`.

## Referências oficiais

- [Definições de agentes](https://developers.openai.com/api/docs/guides/agents/define-agents): recomenda agentes focados, com instruções e handoffs claros.
- [Multi-agent](https://developers.openai.com/api/docs/guides/responses-multi-agent): recomenda delegação para trabalhos independentes e delimitados, com coordenação e síntese pelo agente raiz.
- [Skills](https://developers.openai.com/api/docs/guides/tools-skills): separa workflows reutilizáveis em skills descobertas por nome e descrição.
- [Orientação sobre prompts, skills e AGENTS.md](https://developers.openai.com/blog/rethinking-skills-and-prompts-for-gpt-6-astra): recomenda manter instruções globais enxutas e contextuais.
