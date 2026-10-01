# 0001 — Modelo de registro e ordenação das ocorrências

- Estado: Aceita
- Data: 2026-09-24
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

O projeto compara estruturas de dados usando ocorrências históricas da Mega-Sena. Para que as árvores recebam chaves estáveis e comparáveis, é necessário definir a unidade de registro, a rastreabilidade até o concurso de origem e a ordem entre registros sem acoplar o domínio à implementação das árvores.

## Decisão

Cada dezena de um concurso gera um `Registro` independente. A estrutura conceitual adotada é:

```cpp
struct Chave {
    int frequenciaAcumulada;
    int numero;
};

struct Registro {
    Chave chave;
    int numeroConcurso;
};
```

`numeroConcurso` preserva a origem e a rastreabilidade do registro, mas não participa da chave, da ordenação nem do desempate. A relação com o concurso usa esse identificador estável, e não ponteiros, referências C++ ou posições de contêiner.

A ordem das chaves é lexicográfica: primeiro por `frequenciaAcumulada` e, em caso de igualdade, por `numero`.

O processamento percorre concursos válidos em ordem cronológica. Para cada dezena, incrementa seu contador antes de criar a chave e emite exatamente um registro por ocorrência.

São invariantes do modelo:

- `1 <= numero <= 60`;
- `frequenciaAcumulada >= 1`;
- `numeroConcurso > 0`;
- cada concurso válido contém seis dezenas distintas;
- cada ocorrência válida é processada exatamente uma vez;
- para a mesma dezena, a frequência acumulada cresce de um em um.

Sob essas precondições, a chave é única. Uma chave duplicada representa violação dos dados ou do processamento e não deve ser resolvida silenciosamente usando `numeroConcurso` como desempate.

## Consequências

- As implementações de árvore podem permanecer genéricas e receber a política de comparação separadamente.
- Registros continuam rastreáveis sem depender do tempo de vida ou da posição em memória de outro objeto.
- Comparadores devem ser testados quanto a ordem, igualdade e transitividade.
- O processamento histórico deve ser testado com uma sequência pequena calculada manualmente.
- Filtros por período exigem uma decisão futura sobre frequência global ou reiniciada no começo do recorte.

## Alternativas consideradas

- **Um registro por concurso:** não representa cada ocorrência individual exigida pelos experimentos.
- **Incluir `numeroConcurso` na chave:** esconderia duplicidades causadas por dados ou processamento incorretos.
- **Guardar ponteiro, referência ou posição do concurso:** introduziria dependência de tempo de vida, realocação ou organização do contêiner.

