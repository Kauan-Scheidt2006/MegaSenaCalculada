# MegaSenaCalculada

Projeto acadêmico para estudar estruturas de dados com ocorrências históricas da Mega-Sena.

## Idioma do projeto

- Siga a decisão vigente sobre idioma e nomenclatura indexada em `docs/decisoes/README.md`.

## Escopo atual

- Evolua o projeto em incrementos pequenos e verificáveis.
- Priorize o domínio, o processamento histórico e as comparações antes de GUI, XLSX ou otimizações.
- Não trate frequência histórica como previsão de sorteios futuros.

## Decisões do projeto

- Consulte `docs/decisoes/README.md` antes de alterar regras de negócio, contratos, arquitetura ou escolhas técnicas registradas.
- Registre em `docs/decisoes/` toda nova decisão duradoura ou substituição de uma decisão vigente.
- Não replique aqui o conteúdo dos registros de decisão.

## Limites arquiteturais

- Mantenha regras da Mega-Sena separadas das implementações genéricas de árvore.
- Não acople o domínio à GUI ou ao leitor XLSX.

## Verificação

- Mudanças em comparação exigem testes de ordem, igualdade e transitividade.
- Mudanças no processamento exigem testes com uma sequência pequena calculada manualmente.
- Execute apenas os testes relacionados ao incremento atual; amplie a suíte conforme o código crescer.
- Uma mudança não deve ser considerada aprovada somente por quem a implementou quando houver impacto em regras de negócio, arquitetura ou algoritmos.

## Governança de IA

- O agente coordenador mantém o escopo, seleciona especialistas e integra os resultados.
- Subagentes são temporários e acionados somente para tarefas delimitadas.
- Não delegue edições concorrentes sobre o mesmo arquivo.
- Registre decisões duradouras; não transforme toda conversa em documentação.
- Consulte `docs/governanca-ia.md` para o fluxo, os gates e a distinção entre agentes e skills.
- Consulte `docs/agentes-especializados.md` para selecionar o papel e leia somente o contrato correspondente em `docs/agentes/`.

## Skills locais

- Use `dominio-mega-sena` para modelo, regras de negócio e processamento histórico.
- Use `experimentos-arvores` para comparadores, BST/AVL, métricas e invariantes.
- Use `analise-historica-mega-sena` para análises descritivas de concursos e dezenas.
- Use `arquitetura-gui-desktop` para fronteiras e qualidade da futura interface desktop.
- Quando uma tarefa atravessar áreas, use primeiro a skill que define os dados de entrada e depois a skill que os consome.
