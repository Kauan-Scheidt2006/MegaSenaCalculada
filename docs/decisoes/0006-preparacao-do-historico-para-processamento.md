# 0006 — Preparação do histórico para processamento

- Estado: Aceita
- Data: 2026-09-28
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

Antes de calcular a frequência acumulada, o projeto precisa transformar os
concursos importados em uma sequência válida e determinística. A ordem física
das linhas e das colunas de um arquivo XLSX não deve alterar o resultado nem a
sequência usada nos experimentos com árvores.

Uma linha inválida também não pode ser ignorada silenciosamente. A ausência de
uma ocorrência modificaria a frequência acumulada daquela dezena em todos os
concursos posteriores.

## Decisão

Os concursos válidos serão processados em ordem crescente de
`numeroConcurso`, independentemente da ordem em que aparecem no arquivo de
origem.

Antes da emissão dos registros, as seis dezenas de cada concurso serão
normalizadas em ordem numérica crescente. Dessa forma, a sequência de registros
é reproduzível e não depende da disposição das colunas no XLSX.

A importação e a validação adotarão inicialmente uma política de tudo ou nada.
Ao encontrar qualquer erro em uma linha, a operação será interrompida por uma
exceção e nenhum conjunto parcial de registros será entregue ao consumidor. A
exceção deve preservar contexto suficiente para identificar o arquivo, a aba, a
linha e o motivo do erro quando essas informações estiverem disponíveis.

O tipo específico da exceção e uma estratégia futura de acumulação ou
apresentação de múltiplos erros serão definidos durante incrementos próprios,
sem alterar a regra de que dados inválidos não seguem para o processamento
histórico.

## Consequências

- A ordem física das linhas do XLSX não define a cronologia do processamento.
- A disposição das seis dezenas nas colunas do arquivo não altera a sequência
  emitida para um mesmo concurso.
- Importações com dados inválidos não produzem resultados parciais.
- A validação completa deve acontecer antes do cálculo das frequências, para
  evitar que uma exceção deixe uma sequência parcialmente processada disponível.
- Os testes devem cobrir concursos fora de ordem, dezenas fora de ordem e pelo
  menos uma linha inválida que interrompa a operação.
- Normalizar as dezenas não altera suas frequências, mas torna determinística a
  sequência posterior de inserções e, portanto, as métricas das árvores.

## Alternativas consideradas

- **Preservar a ordem das linhas do arquivo:** rejeitada porque faria a
  cronologia depender da organização física do XLSX.
- **Ignorar uma linha inválida e continuar:** rejeitada porque produziria
  frequências acumuladas incorretas nos concursos seguintes.
- **Preservar a ordem das colunas das dezenas:** rejeitada porque arquivos com
  os mesmos concursos poderiam gerar sequências de inserção diferentes.

