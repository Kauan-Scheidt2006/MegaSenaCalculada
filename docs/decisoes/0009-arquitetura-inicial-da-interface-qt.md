# 0009 — Arquitetura inicial da interface Qt

- Estado: Aceita
- Data: 2026-10-01
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

O projeto possui um protótipo isolado da visualização da árvore, mas ainda não possui o executável definitivo nem uma
arquitetura de interface capaz de sustentar navegação, estados de carregamento, componentes reutilizáveis e apresentação
dos detalhes dos concursos. A interface precisa evoluir sem acoplar widgets às regras da Mega-Sena, às implementações de
BST e AVL ou aos detalhes físicos da leitura do arquivo JSON.

O protótipo `docs/prototypes/qt-gui-tree-explorer-v4.png` estabelece a primeira referência visual: menu contextual de
estruturas, cabeçalho, árvore navegável, zoom, enquadramento, minimapa e barra de estado. Os nós também deverão abrir um
popover com os dados detalhados do registro e do concurso associado.

## Decisão

A interface será implementada em C++20 com Qt 6 Widgets. O ponto de entrada `principal.cpp` construirá `QApplication`,
criará `Aplicacao`, chamará `Aplicacao::iniciar()` e executará o event loop. `Aplicacao` será a raiz de composição e
manterá vivos os serviços, modelos de apresentação e a interface de alto nível.

`Ambiente` será o componente visual superior e herdará diretamente de `QWidget`; `QMainWindow` não será utilizado. O
ambiente hospedará a área de páginas e a barra de estado permanente, mas não implementará leitura, processamento
histórico ou construção de árvores.

Nenhuma página ou fluxo da aplicação instanciará diretamente widgets visuais nativos do Qt. Todo elemento visível terá
uma especialização própria do projeto com contrato e responsabilidade definidos. Os widgets Qt permanecerão como
classes-base e detalhes internos dessas especializações. Layouts Qt são infraestrutura não visual e serão encapsulados
por containers próprios, sem exposição para os consumidores.

A composição será preferida para reunir elementos visuais. Herança será usada quando houver especialização real e um
contrato comum. A biblioteca será construída conforme surgirem consumidores reais, sem antecipar componentes
hipotéticos. Parâmetros recorrentes poderão ser agrupados em structs com padrões explícitos.

A primeira página funcional será `PaginaArvore`. Seu menu lateral será contextual à exploração de estruturas, e não um
menu global do `Ambiente`. A área gráfica receberá um modelo de apresentação independente das implementações concretas
de BST e AVL.

A sobreposição de controles, minimapa, popover e estados será coordenada localmente pelo painel de visualização da
árvore. O gestor correspondente terá uma instância por painel, não será singleton, não criará os componentes e não
conhecerá dados do domínio.

O popover será um componente visual especializado e receberá `DadosDetalhesRegistro`, produzidos pelo modelo de
apresentação ou por um caso de uso. Ele não consultará JSON, árvores ou concursos diretamente. O modelo reunirá a dezena,
a frequência acumulada, o número, a data e as seis dezenas do concurso associado.

A interface modelará explicitamente os estados vazio, construindo, disponível, erro e cancelado. Operações demoradas
serão executadas fora da linha principal da interface, com progresso e cancelamento quando aplicáveis, sem contaminar as
métricas dos algoritmos.

Todos os fluxos críticos deverão admitir teclado, manter foco visível e fornecer informação acessível. Nós serão
acionáveis por clique, `Enter` ou `Espaço`; `Esc` fechará o popover; o foco retornará ao nó originador. Informações não
dependerão exclusivamente de cor ou representação gráfica.

Propriedade C++ e Dono Qt serão explícitos nos contratos. Objetos mantidos por `std::unique_ptr` terão propriedade C++;
widgets filhos seguirão o mecanismo pai-filho do Qt; referências observadoras não transferirão propriedade.

## Consequências

- Será criado um executável definitivo separado do protótipo temporário durante a migração incremental.
- A quantidade de classes visuais será maior, pois widgets visíveis exigirão especializações próprias.
- A biblioteca de componentes não deverá ser construída antecipadamente; cada classe precisará de responsabilidade e
  consumidor identificados.
- A GUI dependerá de modelos de apresentação e casos de uso, não de leitores JSON nem das árvores concretas.
- Os detalhes do concurso exigirão associação explícita entre `Registro` e o concurso importado.
- Geometria manual ficará restrita à cena da árvore e às sobreposições que realmente a exigem; a estrutura comum usará
  layouts.
- Estados e transições poderão ser testados sem abrir janelas; testes integrados da GUI ficarão restritos aos fluxos
  críticos.
- O protótipo `TesteVisualizacaoArvore` permanecerá temporariamente até que o executável final alcance equivalência
  funcional.
- A execução seguirá o roteiro mantido em `docs/planejamento.md`.

## Alternativas consideradas

- **Usar `QMainWindow` como janela superior:** rejeitada porque foi decidido que `Ambiente` terá `QWidget` como base
  direta e controlará explicitamente sua composição.
- **Instanciar widgets Qt diretamente nas páginas:** rejeitada para manter contratos visuais próprios e uma biblioteca
  reutilizável consistente.
- **Criar um gestor global de camadas:** rejeitada porque a sobreposição atual pertence somente ao painel da árvore e não
  justifica estado compartilhado em todo o processo.
- **Entregar a árvore concreta diretamente ao widget:** rejeitada para preservar a independência entre apresentação,
  domínio e experimentos com estruturas.
- **Posicionar toda a interface manualmente:** rejeitada porque layouts oferecem melhor adaptação a redimensionamento,
  fontes, escala e acessibilidade. O cálculo manual permanece adequado à cena gráfica e às sobreposições ancoradas.

