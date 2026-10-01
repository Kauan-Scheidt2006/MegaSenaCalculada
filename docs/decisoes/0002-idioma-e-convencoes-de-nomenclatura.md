# 0002 — Idioma e convenções de nomenclatura do código

- Estado: Aceita
- Data: 2026-09-24
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

O projeto precisa manter uma linguagem uniforme e identificadores previsíveis. Sem uma convenção explícita, contribuições diferentes podem misturar português e inglês ou usar estilos incompatíveis, reduzindo a legibilidade do código-fonte.

## Decisão

Todo código próprio do projeto deve ser escrito em português. Isso inclui nomes de classes, estruturas, funções, métodos, variáveis, atributos, constantes, enumerações e arquivos, além de comentários, testes, mensagens e documentação.

Os identificadores seguem estas convenções:

- nomes compostos de classes usam `PascalCase`, por exemplo `ProcessadorHistorico`;
- arquivos que definem uma classe usam o mesmo nome da classe principal em
  `PascalCase`, preservando apenas a extensão apropriada, por exemplo
  `ProcessadorHistorico.hpp` para a classe `ProcessadorHistorico`;
- variáveis, atributos e métodos usam `camelCase`, por exemplo `frequenciaAcumulada`, `numeroConcurso` e `processarConcursos`;
- nomes devem ser completos e claros, evitando abreviações sem significado evidente.

Termos estrangeiros são permitidos quando impostos pela linguagem, por uma biblioteca, por um protocolo ou por uma ferramenta. Palavras-chave de C++, APIs externas, identificadores de dependências, `CMakeLists.txt` e pontos de entrada obrigatórios não devem ser traduzidos.

## Consequências

- Novos códigos e refatorações devem seguir o idioma e os estilos definidos nesta decisão.
- Revisões devem verificar a consistência dos identificadores alterados.
- Ao renomear uma classe, seu arquivo e todas as inclusões correspondentes
  também devem ser atualizados.
- Arquivos sem uma classe principal, como pontos de entrada, testes e arquivos
  de configuração, não são abrangidos pela regra de correspondência entre
  classe e arquivo.
- Identificadores exigidos por integrações externas permanecem inalterados, mesmo quando estiverem em outro idioma ou estilo.
- Esta decisão não exige renomear imediatamente código legado fora do incremento em andamento.
