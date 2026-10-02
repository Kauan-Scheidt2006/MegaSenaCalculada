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

### [ ] Etapa 6 — Implementar o menu contextual de estruturas

Entregas:

- `MenuEstruturas` e `ItemNavegacaoEstrutura`;
- seleção única entre BST e AVL;
- opções futuras desabilitadas, se ainda fizerem parte da apresentação;
- acionamento por mouse e teclado.

Critério de conclusão: o menu comunica intenção sem construir ou conhecer a implementação interna das árvores.

### [ ] Etapa 7 — Definir o modelo de apresentação da árvore

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

### [ ] Etapa 8 — Adaptar BST e AVL para a apresentação

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

### [ ] Etapa 9 — Implementar a visualização gráfica

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

### [ ] Etapa 10 — Implementar controles e minimapa

Entregas:

- `BarraFerramentasArvore` e `ControleZoom`;
- zoom mínimo, máximo e percentual atual;
- enquadramento e modo de arrasto;
- `MapaArvore` sincronizado com o viewport.

Critério de conclusão: todos os controles são acessíveis por teclado e não ocultam conteúdo essencial.

### [ ] Etapa 11 — Implementar o popover de detalhes

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

### [ ] Etapa 12 — Consolidar as camadas locais da árvore

Entregas:

- `CamadaVisualizacaoArvore`;
- `GestorCamadasArvore`, com uma instância por painel;
- ordem e posicionamento de árvore, controles, detalhes e estado.

Critério de conclusão: o gestor é local, não é singleton, não cria componentes e não conhece dados de domínio.

### [ ] Etapa 13 — Implementar estados e execução assíncrona

Entregas:

- apresentações para vazio, construindo, disponível, erro e cancelado;
- coordenador de carregamento e construção fora da thread visual;
- progresso e cancelamento quando aplicáveis;
- descarte de resultados obsoletos.

Critérios de conclusão:

- a janela permanece responsiva;
- erro explica o problema e uma ação possível;
- métricas algorítmicas não incluem renderização nem agendamento da GUI.

### [ ] Etapa 14 — Consolidar estilo e recursos

Áreas previstas:

```text
src/interface/estilos/
recursos/estilos/
recursos/icones/
recursos/recursos.qrc
```

Entregas:

- estilo global inicial;
- recursos registrados no QRC;
- foco visível, contraste e ícones com alternativas textuais;
- nomes e propriedades semânticas estáveis para estilização.

### [ ] Etapa 15 — Validar comportamento e apresentação

Verificações:

- testes de modelos e transições sem abrir janelas;
- testes de cálculo de posições e limites de zoom;
- testes de seleção, popover, foco e teclado;
- fluxo integrado de BST e AVL;
- estados vazio, sucesso, erro e cancelamento;
- inspeção visual em tamanhos e escalas diferentes.

Gate: impacto arquitetural, domínio e algoritmos exigem revisão independente proporcional antes da conclusão.

### [ ] Etapa 16 — Substituir o protótipo temporário

Entregas:

- confirmar equivalência funcional com o protótipo;
- retirar o alvo `TesteVisualizacaoArvore`;
- remover ou arquivar os arquivos temporários de apresentação;
- preservar fixtures que permaneçam úteis;
- atualizar README e instruções de build.

Critério de conclusão: nenhuma funcionalidade exclusiva permanece no protótipo e o executável final passa por compilação,
testes e revisão.

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

## Próxima etapa

A próxima entrega é a **Etapa 6 — Implementar o menu contextual de estruturas**. Ela deverá introduzir a seleção entre
BST e AVL sem construir árvores nem conhecer suas implementações internas.
