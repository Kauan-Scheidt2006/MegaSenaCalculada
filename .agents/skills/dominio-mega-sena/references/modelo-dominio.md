# Contrato inicial do domínio

## Tipos

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

`numeroConcurso` é dado de origem e rastreabilidade. Ele não participa da chave, da ordenação ou do desempate. Guardar o identificador inteiro é preferível a guardar ponteiro, referência C++ ou índice de contêiner.

## Comparação

`a < b` quando:

```text
a.frequenciaAcumulada < b.frequenciaAcumulada
ou
(a.frequenciaAcumulada == b.frequenciaAcumulada e a.numero < b.numero)
```

## Invariantes

- `1 <= chave.numero <= 60`.
- `chave.frequenciaAcumulada >= 1`.
- `numeroConcurso > 0`.
- Cada concurso válido contém seis dezenas distintas.
- Cada ocorrência válida é processada exatamente uma vez, em ordem cronológica.
- Para uma mesma dezena, a frequência acumulada cresce de um em um.

Com essas precondições, duas ocorrências não compartilham a mesma chave. Uma chave duplicada indica entrada duplicada ou erro de processamento.

## Decisão adiada

Antes de implementar filtros por período, defina se a frequência de um recorte é global até aquele ponto ou reiniciada no começo do recorte.

