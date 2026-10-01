# 0007 — Encapsulamento da leitura XLSX

- Estado: Substituída
- Data: 2026-09-28
- Substitui: não se aplica
- Substituída por: [0008](0008-arquivo-json-como-fonte-dos-concursos.md)

## Contexto

O histórico da Mega-Sena será recebido em arquivos XLSX. O projeto precisa
ler esses arquivos sem acoplar o domínio, o processamento histórico, as
estruturas de árvore ou a interface gráfica a uma biblioteca específica.

## Decisão

O projeto usará OpenXLSX, em versão fixada pelo CMake, exclusivamente por
meio de `LeitorPlanilhaXlsx`. O contrato público do leitor usará somente tipos
próprios e da biblioteca padrão. Tipos, cabeçalhos e exceções do OpenXLSX
não integrarão esse contrato.

O leitor representará os valores das células sem interpretar regras da
Mega-Sena e preservará as coordenadas de origem. A interpretação das colunas,
a validação dos concursos e o processamento das frequências permanecerão em
etapas posteriores e separadas.

Uma operação de leitura somente retornará depois que toda a aba tiver sido
lida. Falhas serão traduzidas para `ErroLeituraPlanilha`, com o contexto
disponível de arquivo, aba, linha e coluna.

## Consequências

- A troca futura da biblioteca fica restrita à camada de infraestrutura.
- Consumidores não precisam conhecer a representação de valores do OpenXLSX.
- O leitor não valida dezenas nem corrige dados silenciosamente.
- Testes de integração devem usar arquivos XLSX reais e cobrir a tradução de
  falhas.
- A atualização do OpenXLSX deve ser explícita e acompanhada pelos testes do
  leitor.

## Alternativas consideradas

- **Usar OpenXLSX diretamente nos importadores:** rejeitada por espalhar a
  dependência e suas exceções pelas demais camadas.
- **Fazer o leitor devolver concursos:** rejeitada porque misturaria acesso ao
  arquivo com interpretação e validação do domínio.
