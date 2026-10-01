# Dívidas técnicas

Este documento reúne melhorias conhecidas que foram deliberadamente adiadas.
Uma dívida registrada aqui não representa uma funcionalidade aprovada para o
incremento atual. Antes de implementá-la, seu escopo, seus critérios de
aceitação e seus impactos devem ser novamente avaliados.

## Modelo para novos itens

Copie a seção abaixo para o final deste arquivo e substitua os campos entre
colchetes.

```markdown
## DT-[número] — [título]

- Estado: Pendente
- Registrada em: [AAAA-MM-DD]
- Área: [domínio, estruturas, apresentação, infraestrutura etc.]
- Origem: [decisão, protótipo, limitação ou incremento relacionado]

### Contexto

[Explique a situação atual e por que a necessidade existe.]

### Solução adiada

[Descreva o comportamento desejado sem antecipar detalhes desnecessários de
implementação.]

### Motivo do adiamento

[Explique por que o trabalho não faz parte do incremento atual.]

### Impacto enquanto permanecer pendente

[Descreva as limitações observáveis e os riscos conhecidos.]

### Condições para retomada

[Liste o conhecimento, as decisões ou os incrementos que devem existir antes
da retomada.]

### Critérios de aceitação iniciais

- [Resultado observável 1]
- [Resultado observável 2]

### Observações e riscos

[Registre dependências, cuidados de acessibilidade, desempenho, testes ou
compatibilidade.]
```

## DT-001 — Acompanhamento da construção das árvores

- Estado: Pendente
- Registrada em: 2026-10-01
- Área: Apresentação e experimentos com árvores
- Origem: Protótipo da visualização da BST e da AVL

### Contexto

O primeiro protótipo apresenta somente o estado final da estrutura escolhida.
Essa visualização permite explorar a topologia resultante, comparar alturas e
observar o desbalanceamento da árvore de busca binária em contraste com a AVL,
mas não mostra como cada inserção produziu esse resultado.

O acompanhamento temporal é particularmente relevante para o objetivo
acadêmico do projeto. Na árvore de busca binária, ele permite observar o
caminho percorrido pelo novo registro e o crescimento sem rebalanceamento. Na
AVL, permite identificar o primeiro ancestral desbalanceado, a rotação
aplicada e a nova topologia após o rebalanceamento.

### Solução adiada

A visualização deverá oferecer um modo chamado **Construção**, separado da
visão **Resultado final**. Esse modo deverá reproduzir a mesma sequência de
registros usada para construir a árvore, sem recalcular nem alterar as regras
do domínio na camada de apresentação.

O controle temporal previsto contém:

- indicador `passo atual / total de inserções`;
- ações **Primeiro**, **Anterior**, **Próximo** e **Último**;
- ação **Reproduzir/Pausar**, com velocidade configurável;
- linha do tempo que permita saltar para uma inserção específica;
- identificação persistente do registro que está sendo inserido;
- opção de retornar ao resultado final sem perder o tipo de árvore e o recorte
  selecionados.

Em cada passo, a interface deverá distinguir visualmente:

1. a árvore existente antes da inserção;
2. o caminho de comparação percorrido pelo novo registro;
3. a posição em que o nó foi criado;
4. a árvore resultante após a operação;
5. no caso da AVL, o ancestral desbalanceado, o tipo de rotação (`RD`, `RE`,
   `ED` ou `DE`) e os nós envolvidos antes e depois da rotação.

O painel de detalhes deverá explicar o evento corrente em texto, por exemplo:
“a chave (3, 17) é maior que (2, 42) e segue para o filho direito”. Essa
descrição será a alternativa textual às animações e também ajudará a entender
o comparador lexicográfico por frequência acumulada e dezena.

O salto pela linha do tempo deverá produzir exatamente o mesmo estado que a
navegação passo a passo. A reprodução automática será apenas uma forma de
navegar entre estados determinísticos; duração de animações e renderização não
deverão fazer parte das métricas algorítmicas.

### Motivo do adiamento

O incremento atual busca validar a organização da tela, a escolha entre tipos
de árvore e a navegação espacial de estruturas grandes. A construção temporal
introduz estados intermediários, controles de reprodução, explicações de
comparações e rotações, animações e requisitos adicionais de acessibilidade.
Projetá-los agora aumentaria o escopo antes da validação da visualização final.

### Impacto enquanto permanecer pendente

- O usuário poderá inspecionar apenas a topologia final.
- Não será possível relacionar visualmente uma inserção específica à posição
  que ela ocupou.
- As rotações AVL poderão ser apresentadas como métricas agregadas, mas não
  observadas no instante em que ocorreram.
- O projeto continuará permitindo comparar as estruturas finais, porém com
  menor apoio didático para explicar o processo de construção.

### Condições para retomada

- A visualização e a navegação espacial da árvore final devem estar validadas.
- A sequência de inserção precisa estar disponível de forma estável e
  rastreável.
- Os eventos observáveis das inserções e rotações devem ser definidos sem
  expor os nós internos nem acoplar as árvores à GUI.
- Deve ser decidido se cada passo representa uma inserção completa ou se
  comparações e rotações serão subpassos navegáveis.
- O comportamento para recortes deve ser definido: preservar o histórico
  global até o recorte ou reconstruir uma árvore apenas com o subconjunto.

### Critérios de aceitação iniciais

- Alternar entre **Resultado final** e **Construção** não altera a sequência de
  dados nem o tipo de árvore selecionado.
- Avançar até o último passo produz uma árvore idêntica ao resultado final.
- Voltar e avançar para o mesmo passo sempre reproduz a mesma topologia.
- BST e AVL utilizam a mesma sequência de inserção.
- Cada rotação AVL informa tipo e nós envolvidos, sem contaminar as métricas
  de tempo da estrutura.
- Todos os controles funcionam por teclado e informam nome, estado e ação às
  tecnologias assistivas.
- O evento corrente possui descrição textual equivalente ao destaque visual.
- Reprodução, pausa, salto e mudança de velocidade não bloqueiam a interface.

### Observações e riscos

Guardar uma cópia completa da árvore para cada inserção pode consumir memória
em excesso com o histórico integral. Na futura elaboração técnica deverão ser
comparadas estratégias como instantâneos periódicos mais reprodução de
eventos, uma trilha imutável de operações ou reconstrução sob demanda.

Animações devem respeitar a preferência do sistema por movimento reduzido. A
linha do tempo não poderá ser a única forma de escolher um passo, e cores não
deverão ser o único meio de identificar caminho, inserção ou rotação.
