# Planejamento da interface desktop

## Objetivo

Implementar incrementalmente a interface desktop do Mega Sena Calculada em Qt 6 Widgets, começando pela exploração de
árvores do protótipo `prototypes/qt-gui-tree-explorer-v4.png`. A interface deverá preservar o domínio e as estruturas de
dados independentes da apresentação, utilizar somente componentes visuais especializados pelo projeto e apresentar os
detalhes do registro e do concurso em um popover acessível.

Este documento é o acompanhamento duradouro da implementação. As decisões arquiteturais pertencem a
`docs/decisoes/`; contratos públicos pertencem aos headers; invariantes implementadas devem ser cobertas por testes.

## Legenda

- `[x]` concluída e verificada;
- `[ ]` ainda não concluída;
- uma etapa somente deve ser marcada como concluída quando todos os seus critérios forem atendidos;
- mudanças de escopo ou arquitetura devem atualizar a decisão correspondente antes deste roteiro.

## Escopo da primeira evolução visual

- executável final iniciado por `principal.cpp`;
- `Aplicacao` como raiz de composição e controle do ciclo de vida lógico;
- `Ambiente`, derivado diretamente de `QWidget`, como estrutura visual permanente;
- biblioteca de componentes visuais especializados;
- página de exploração de BST e AVL;
- zoom, enquadramento, arrasto, minimapa e barra de estado;
- popover com a dezena, frequência acumulada, número, data e dezenas do concurso;
- estados vazio, construindo, disponível, erro e cancelado;
- operação por mouse e teclado, foco visível e informações acessíveis.

## Fora do escopo inicial

- implementar antecipadamente todas as páginas do protótipo geral;
- adicionar estruturas além de BST e AVL;
- redesenhar regras de domínio, comparação ou balanceamento por causa da GUI;
- criar gestor global de camadas;
- criar sistema de temas múltiplos antes de existir essa necessidade;
- criar componentes sem consumidor real;
- apresentar frequência histórica como previsão de resultados futuros.

O limite acima descreve a primeira evolução visual já implementada. A integração da
árvore rubro-negra é uma evolução posterior, detalhada na Etapa 12, e não altera as
fronteiras entre estruturas, aplicação e interface.

## Etapas

### [x] Etapa 0 — Consolidar a arquitetura

Entregas:

- registrar a decisão `0009-arquitetura-inicial-da-interface-qt.md`;
- indexar a decisão em `docs/decisoes/README.md`;
- registrar este planejamento;
- fixar as fronteiras, propriedade, estados e requisitos de acessibilidade antes dos contratos de código.

Critério atendido: a arquitetura discutida foi aprovada e registrada como decisão aceita em 2026-10-01.

### [x] Etapa 1 — Criar a raiz do executável

Arquivos previstos:

```text
principal.cpp
src/aplicacao/Aplicacao.hpp
src/aplicacao/Aplicacao.cpp
```

Entregas:

- construir `QApplication` somente no ponto de entrada;
- criar `Aplicacao`, chamar `iniciar()` e entrar no event loop;
- converter falhas fatais de inicialização em código de saída controlado;
- criar no CMake o alvo final `MegaSenaCalculada`;
- preservar temporariamente o protótipo executável como referência.

Critérios de conclusão:

- `principal.cpp` não cria telas, árvores nem leitores;
- `Aplicacao` mantém vivos os componentes de alto nível;
- o executável final inicia e encerra corretamente;
- compilação do incremento concluída sem novos avisos relevantes.

Situação transitória encerrada pela Etapa 4: `Aplicacao` agora possui e apresenta `Ambiente`, que sustenta o event loop
até o fechamento da janela.

### [x] Etapa 2 — Criar os fundamentos visuais

Áreas previstas:

```text
src/interface/fundamentos/
src/interface/parametros/
```

Entregas:

- `BaseContainer`, `BasePagina`, `Painel` e `Barra`;
- parâmetros comuns de construção;
- disposições vertical, horizontal, em grade e empilhada;
- encapsulamento dos layouts Qt;
- propriedade e Dono Qt explícitos.

Critérios de conclusão:

- consumidores não instanciam layouts diretamente;
- componentes adicionados recebem propriedade coerente;
- somente abstrações com consumidor real são introduzidas.

### [x] Etapa 3 — Criar os componentes visuais básicos

Entregas iniciais:

- `Texto`, `Titulo`, `Subtitulo` e `Imagem`;
- `Botao`, `BotaoIcone` e `BotaoNavegacao`;
- `IndicadorProgresso`;
- padronização de foco, nome acessível, estado habilitado e estilo.

Critério de conclusão: páginas e composições não instanciam widgets visuais nativos diretamente.

### [x] Etapa 4 — Implementar o Ambiente

Áreas previstas:

```text
src/interface/ambiente/
```

Entregas:

- `Ambiente : QWidget`;
- `AreaPaginas` com disposição empilhada encapsulada;
- `BarraEstado` permanente;
- configuração da janela, apresentação e encerramento visual.

Critérios de conclusão:

- não utilizar `QMainWindow`;
- `Ambiente` não lê JSON nem cria árvores;
- redimensionamento estrutural ocorre por layouts, sem geometria manual geral.

### [x] Etapa 5 — Criar o esqueleto da página da árvore

Áreas previstas:

```text
src/interface/paginas/arvore/
```

Entregas:

- `PaginaArvore`, `ConteudoPaginaArvore` e `CabecalhoPaginaArvore`;
- `PainelVisualizacaoArvore` inicialmente no estado vazio;
- proporções e hierarquia correspondentes ao protótipo v4.

Critério de conclusão: menu, cabeçalho e área da árvore se adaptam ao tamanho da janela e mantêm ordem lógica de foco.

### [x] Etapa 6 — Implementar o menu contextual de estruturas

Entregas:

- `MenuEstruturas` e `ItemNavegacaoEstrutura`;
- seleção única entre BST e AVL;
- opções futuras desabilitadas, se ainda fizerem parte da apresentação;
- acionamento por mouse e teclado.

Critério de conclusão: o menu comunica intenção sem construir ou conhecer a implementação interna das árvores.

### [x] Etapa 7 — Definir o modelo de apresentação da árvore

Área prevista:

```text
src/interface/apresentacao/arvore/
```

Entregas:

- `EstadoVisualizacaoArvore`;
- modelos de nó, ligação e árvore;
- `DadosDetalhesRegistro`;
- identidade estável do registro, sem usar seu endereço de memória;
- `ModeloApresentacaoArvore` testável sem janelas.

Gate: os contratos do modelo visual e dos detalhes devem ser aprovados antes da adaptação das árvores e da GUI gráfica.

### [x] Etapa 8 — Adaptar BST e AVL para a apresentação

Entregas:

- caso de uso para construir o modelo visual de BST ou AVL;
- caso de uso para consultar os detalhes do registro e do concurso;
- associação entre registro, data e seis dezenas do concurso;
- separação entre processamento histórico e apresentação.

Critérios de conclusão:

- a GUI não depende diretamente das classes concretas BST e AVL;
- os detalhes correspondem ao concurso correto;
- comparadores e regras de frequência não são duplicados.

Dependência: caso o processamento histórico necessário ainda não exista, implementá-lo como incremento de domínio
separado antes de concluir esta etapa.

### [x] Etapa 9 — Implementar a visualização gráfica

Área prevista:

```text
src/interface/componentes/arvore/
```

Entregas:

- `VisualizacaoArvore`, `ItemNoArvore` e `ItemLigacaoArvore`;
- cálculo de disposição separado do widget;
- apresentação a partir de `ModeloVisualArvore`;
- seleção e ativação por identificador estável;
- enquadramento e navegação de árvores largas ou desbalanceadas.

Critério de conclusão: nenhuma leitura, regra de negócio ou construção da árvore acontece durante a pintura.

### [x] Etapa 10 — Implementar controles e minimapa

Entregas:

- `BarraFerramentasArvore` e `ControleZoom`;
- zoom mínimo, máximo e percentual atual;
- enquadramento e modo de arrasto;
- `MapaArvore` sincronizado com o viewport.

Critério de conclusão: todos os controles são acessíveis por teclado e não ocultam conteúdo essencial.

### [x] Etapa 11 — Implementar o popover de detalhes

Entregas:

- `PopoverDetalhesRegistro`;
- `CampoDetalhe`, `GrupoDezenasConcurso` e `MarcadorDezena`;
- apresentação da dezena, frequência, número, data e dezenas do concurso;
- destaque da dezena correspondente ao nó;
- posicionamento junto ao nó e limitação às bordas visíveis.

Critérios de conclusão:

- clique, `Enter` e `Espaço` abrem o popover;
- botão próprio e `Esc` fecham o popover;
- o foco entra no popover e retorna ao nó originador;
- zoom, rolagem e redimensionamento reposicionam o popover;
- troca de página, estrutura ou modelo fecha detalhes obsoletos.

### [ ] Etapa 12 — Integrar a árvore rubro-negra à interface

Objetivo: permitir selecionar e visualizar a estrutura final da árvore
rubro-negra com os mesmos registros, detalhes e controles já disponíveis para BST
e AVL. A primeira entrega não precisa diferenciar visualmente nós vermelhos e
pretos.

#### 12.1 — Completar o contrato de leitura da estrutura

Arquivos previstos:

```text
src/estruturas/ArvoreRubroNegra.hpp
testes/testes_estrutura_inicial.cpp
```

Entregas:

- expor `altura()` sem revelar nós ou ponteiros;
- expor visita em ordem com valor, pai, lado e profundidade, compatível com a
  construção dos modelos de BST e AVL;
- contabilizar e expor a quantidade de rotações executadas, para que a métrica
  apresentada não use um valor fictício;
- preservar as invariantes rubro-negras, a ordem e a rejeição de duplicatas.

Critérios de conclusão:

- a estrutura continua genérica e sem dependência de Qt ou da apresentação;
- uma sequência pequena calculada manualmente confirma topologia, altura, cores e
  rotações;
- os testes atuais de invariantes continuam aprovados.

#### 12.2 — Construir o modelo de apresentação rubro-negro

Arquivos previstos:

```text
src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.hpp
src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.cpp
src/aplicacao/arvore/DetalhesConstrucaoModelo.hpp
testes/testes_processamento_apresentacao.cpp
CMakeLists.txt
```

Entregas:

- inserir os registros processados, na ordem do arquivo JSON, em uma
  `ArvoreRubroNegra<Registro, ComparadorRegistroContabilizado>`;
- converter sua topologia para `ModeloVisualArvore`, reutilizando identificadores,
  ligações e métricas existentes;
- manter a consulta de detalhes independente da estrutura selecionada.

Critérios de conclusão:

- quantidade de nós, raiz, altura, ligações, comparações e rotações representam a
  árvore rubro-negra realmente construída;
- o modelo contém todos os registros e mantém a mesma travessia em ordem das
  demais estruturas;
- nenhuma classe da interface depende de `ArvoreRubroNegra`.

#### 12.3 — Propagar a seleção pela interface

Arquivos previstos:

```text
src/interface/paginas/arvore/TipoEstrutura.hpp
src/interface/paginas/arvore/MenuEstruturas.hpp
src/interface/paginas/arvore/MenuEstruturas.cpp
src/interface/paginas/arvore/CabecalhoPaginaArvore.cpp
testes/testes_interface_inicial.cpp
```

Entregas:

- adicionar `TipoEstrutura::RubroNegra`;
- criar o item navegável “Rubro-negra”, com nome acessível, ordem de foco e estado
  de seleção exclusiva;
- fazer `selecionar()` tratar explicitamente os quatro valores, sem usar a última
  opção como fallback implícito;
- continuar emitindo o sinal genérico `estruturaSelecionada(TipoEstrutura)` do
  menu para `PaginaArvore`, que o retransmite para `Aplicacao`;
- atualizar título e subtítulo quando a opção rubro-negra for selecionada.

Critérios de conclusão:

- mouse e teclado selecionam o novo item;
- apenas um item permanece selecionado;
- uma mudança efetiva emite o sinal uma vez e selecionar novamente o item atual
  não o reemite;
- `PaginaArvore` fecha detalhes obsoletos, atualiza o cabeçalho e retransmite a
  seleção como já faz para as demais estruturas.

#### 12.4 — Carregar e apresentar pela raiz de composição

Arquivos previstos:

```text
src/aplicacao/Aplicacao.hpp
src/aplicacao/Aplicacao.cpp
```

Entregas:

- reutilizar os registros já carregados e processados durante a inicialização;
- construir e manter vivo `modeloRubroNegra_` junto aos modelos BST e AVL;
- tratar explicitamente todos os valores de `TipoEstrutura` em
  `apresentarEstrutura()`;
- pausar a reprodução AVL e desativar seus controles antes de apresentar a
  rubro-negra;
- apresentar o modelo rubro-negro pelo mesmo fluxo de `PaginaArvore::apresentar()`.

Critérios de conclusão da primeira implementação:

- a aplicação inicia mostrando BST como hoje;
- selecionar “Rubro-negra” apresenta sua topologia, métricas e detalhes sem
  recriar nem reler o histórico;
- alternar entre BST, AVL, AVL passo a passo e rubro-negra não deixa temporizador,
  controles ou detalhes em estado incompatível;
- compilar o executável e aprovar os testes de estrutura, apresentação e
  interface relacionados ao incremento.

#### 12.5 — Diferenciar visualmente nós vermelhos e pretos

Esta é a última fase e não bloqueia a conclusão funcional das fases 12.1 a 12.4.

Arquivos previstos:

```text
src/interface/apresentacao/arvore/ModeloNoArvore.hpp
src/aplicacao/arvore/DetalhesConstrucaoModelo.hpp
src/aplicacao/arvore/ConstrutorApresentacaoRubroNegra.cpp
src/interface/componentes/arvore/VisualizacaoArvore.hpp
src/interface/componentes/arvore/VisualizacaoArvore.cpp
src/interface/componentes/arvore/ItemNoArvore.hpp
src/interface/componentes/arvore/ItemNoArvore.cpp
recursos/estilos/tema-padrao.qss
testes/testes_modelo_apresentacao_arvore.cpp
testes/testes_processamento_apresentacao.cpp
testes/testes_interface_inicial.cpp
```

Entregas:

- acrescentar ao modelo do nó uma semântica visual neutra, por exemplo
  `Padrao`, `RubroNegroPreto` e `RubroNegroVermelho`, sem importar o enum da
  estrutura para a camada de apresentação;
- manter BST e AVL no estilo padrão atual e mapear as cores internas da
  rubro-negra durante a construção do modelo;
- fazer a visualização escolher o fundo do item conforme essa semântica;
- manter a cor atual para nós pretos e declarar no QSS a nova cor de fundo dos
  nós vermelhos, incluindo contraste de texto, borda, foco e seleção;
- complementar a descrição acessível do nó com “vermelho” ou “preto”, pois a
  informação não pode depender exclusivamente da cor.

Critérios de conclusão:

- a cor do nó percorre o fluxo estrutura → caso de uso → modelo → item gráfico,
  sem a interface consultar a árvore concreta;
- BST e AVL não sofrem alteração visual;
- nós rubro-negros permanecem legíveis nos estados normal, foco, hover e seleção;
- testes validam o mapeamento semântico e uma inspeção visual confirma contraste
  e diferenciação.

Riscos e cuidados:

- a visita genérica usada por BST e AVL não conhece cor; a rubro-negra deve
  acrescentar essa informação sem tornar o helper comum dependente de uma
  estrutura concreta;
- cadeias condicionais que hoje assumem apenas três tipos devem ser substituídas
  por tratamento explícito para evitar que valores futuros caiam silenciosamente
  em AVL passo a passo;
- alterações em contagem de rotações e visita da estrutura exigem revisão
  independente, pois afetam um algoritmo e seu contrato público;
- toda cor nova pertence ao QSS, conforme a decisão 0010.


## Ordem de execução e propriedade

As etapas são sequenciais por padrão. Trabalhos independentes podem ser delegados depois que seus contratos estiverem
aprovados, mas `CMakeLists.txt`, `Aplicacao.cpp`, `PaginaArvore.cpp` e outros pontos de integração terão um único
proprietário por vez.

Papéis previstos conforme a necessidade:

| Área | Papel |
|---|---|
| fronteiras e contratos | arquiteto |
| páginas e componentes Qt | implementador |
| estados e modelos da GUI | arquiteto de apresentação |
| processamento histórico | processador histórico |
| BST, AVL e métricas | especialista em estruturas |
| tarefas fora da thread visual | especialista em execução assíncrona |
| foco, teclado e contraste | especialista em experiência e acessibilidade |
| testes da interface | especialista em testes de interface |
| integração | coordenador |
| avaliação final relevante | revisor independente |

## Evolução da exploração visual — layout compacto e contexto

Decisão associada: `0013-visualizacao-compacta-e-exploracao-contextual.md`.

- [x] substituir a disposição em ordem por disposição compacta por contornos;
- [x] reduzir progressivamente o conteúdo textual em escalas pequenas;
- [x] destacar o caminho do nó selecionado até a raiz;
- [x] oferecer modo de foco com ancestrais e contexto descendente;
- [x] permitir recolher, expandir e restaurar subárvores;
- [x] oferecer navegação estrutural por teclado;
- [x] recalcular os limites usados pelo minimapa após recolhimento;
- [ ] avaliar animação entre disposições somente após validação de usabilidade.

## Próxima etapa

A próxima entrega é a **Etapa 12.1 — Completar o contrato de leitura da estrutura rubro-negra**. Ela prepara altura,
topologia e métricas verificáveis antes de conectar a nova opção à interface. A diferenciação visual entre nós vermelhos
e pretos fica deliberadamente para a Etapa 12.5 e não bloqueia a primeira visualização funcional.
