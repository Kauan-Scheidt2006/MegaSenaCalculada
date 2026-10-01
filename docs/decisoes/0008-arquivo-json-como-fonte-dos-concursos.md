# 0008 — Arquivo JSON como fonte dos concursos

- Estado: Aceita
- Data: 2026-10-01
- Substitui: [0007](0007-encapsulamento-da-leitura-xlsx.md)
- Substituída por: não se aplica

## Contexto

O objetivo acadêmico do projeto é estudar o processamento histórico e o
comportamento das estruturas de árvore. A importação de planilhas acrescenta
complexidade técnica que não contribui para esses experimentos.

## Decisão

Os concursos serão fornecidos pelo arquivo `concursos.json`, localizado por
padrão na raiz de execução do projeto. O documento terá um array na raiz e
cada item usará o formato:

```json
{
  "numeroConcurso": 1,
  "data": "11/03/1996",
  "dezenas": [4, 5, 30, 33, 41, 52]
}
```

`LeitorConcursosJson` encapsulará a leitura e a interpretação do JSON. Seu
contrato público usará somente tipos próprios e da biblioteca padrão. A
implementação usará as classes JSON do Qt Core, que já é uma dependência do
projeto, sem expor tipos Qt aos consumidores.

O leitor verificará a estrutura e os tipos do documento, mas não aplicará
regras da Mega-Sena. A validação de quantidade, faixa e unicidade das dezenas,
a ordenação cronológica e a geração das frequências permanecerão em etapas
separadas.

## Consequências

- OpenXLSX deixa de ser dependência do projeto.
- Arquivos XLSX não serão aceitos como entrada.
- Erros de leitura informarão o arquivo e a posição lógica do valor no JSON.
- A política de tudo ou nada da decisão 0006 permanece vigente.
- As referências a linhas, colunas e XLSX na decisão 0006 passam a
  corresponder aos itens, campos e ao arquivo JSON definidos nesta decisão.
- A ordem dos itens e das dezenas no JSON não substitui a normalização
  posterior definida na decisão 0006.

## Alternativas consideradas

- **Manter suporte simultâneo a XLSX e JSON:** rejeitada porque conservaria a
  complexidade que esta decisão pretende remover.
- **Acoplar o parser diretamente ao processamento:** rejeitada para preservar
  testes e responsabilidades separados.
