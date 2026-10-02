# 0010 — Estilos visuais exclusivamente em QSS

- Estado: Aceita
- Data: 2026-10-02
- Substitui: não se aplica
- Substituída por: não se aplica

## Contexto

A interface inicial declarou nomes de objeto e a intenção de pintar fundos, mas inseriu regras QSS diretamente nos
construtores de dois painéis. Essas regras ainda utilizavam referências à paleta corrente e, portanto, acompanhavam o
tema do sistema operacional. Os demais componentes continuavam dependentes do estilo nativo e não existia um ponto de
verdade que mostrasse a aparência completa da aplicação.

O projeto requer uma identidade visual fixa, auditável e independente do tema claro ou escuro do sistema. Também precisa
impedir que cores, fontes, bordas, raios, preenchimentos e estados visuais sejam distribuídos entre os arquivos C++.

## Decisão

Toda declaração visual própria do projeto será escrita em arquivo `.qss` registrado no sistema de recursos Qt. Código
C++ não declarará cores, fontes, bordas, imagens de fundo, raios, preenchimentos nem outras propriedades de aparência.

É proibido nos componentes:

- chamar `QWidget::setStyleSheet()`;
- armazenar fragmentos QSS em parâmetros, constantes ou literais C++;
- definir cores com `QPalette`, `setPalette()` ou métodos equivalentes;
- usar referências à paleta do sistema no QSS do tema fixo;
- depender de seletores universais ou de regras sem identificação explícita do componente.

A única chamada permitida a `setStyleSheet()` pertence a `EstiloAplicacao`. Ela carrega integralmente o recurso
`:/estilos/tema-padrao.qss` e o instala no `QApplication` antes da criação do primeiro widget. Essa chamada é apenas o
mecanismo de instalação: nenhuma regra visual fica no objeto global ou no código C++. Toda regra permanece no arquivo
QSS e contém seletor explícito.

Os seletores obedecerão aos seguintes critérios:

1. componentes concretos usam o seletor de ID `#nomeObjeto`;
2. famílias reutilizáveis podem usar propriedades semânticas, como `[papelVisual="titulo"]`, desde que o seletor seja
   limitado ao tipo ou à região consumidora;
3. estados interativos usam pseudoestados QSS, como `:hover`, `:focus`, `:pressed`, `:checked` e `:disabled`;
4. seletores universais (`*`) e regras genéricas como `QWidget { ... }` não são permitidos;
5. todo componente visual criado deve possuir `nomeObjeto` estável e uma regra explícita no QSS, mesmo quando a regra
   declarar `background-color: transparent`;
6. componentes com `fundoEstilizado = true` devem possuir no QSS uma regra de fundo não transparente;
7. componentes com `fundoEstilizado = false` ainda têm sua aparência textual ou transparente declarada no QSS.

`Qt::WA_StyledBackground`, políticas de tamanho, visibilidade, foco, acessibilidade e propriedades semânticas continuam
sendo configurados em C++, pois expressam comportamento e contrato do widget, não a aparência concreta do tema.
Geometria estrutural continua pertencendo aos layouts; dimensões estritamente visuais poderão ser declaradas no QSS
quando não representarem uma restrição funcional.

O QSS deve usar cores literais do tema, incluindo texto, fundo, borda, foco e estados interativos. Novos componentes
somente estarão concluídos quando seus seletores e todos os estados relevantes estiverem declarados no arquivo QSS.

### Fluxo de aplicação

```text
construir QApplication
  → carregar o recurso QSS
  → validar que o arquivo existe e não está vazio
  → instalar a folha no QApplication
  → criar Aplicacao, Ambiente e componentes
```

### Lista de verificação para novos componentes

- definir `nomeObjeto` único e estável;
- definir explicitamente `fundoEstilizado`;
- adicionar seletor correspondente ao QSS;
- declarar cores de primeiro plano e fundo necessárias;
- declarar borda, fonte e espaçamento visual aplicáveis;
- declarar `:focus`, `:hover`, `:pressed`, `:checked` e `:disabled` quando aplicáveis;
- verificar contraste e foco visível;
- testar que o componente não possui folha de estilos local;
- testar que o QSS global contém seu seletor.

## Consequências

- a aparência passa a ter um único ponto de verdade em `recursos/estilos/tema-padrao.qss`;
- a troca do tema do sistema não altera as cores declaradas pela aplicação;
- construtores ficam restritos à composição, comportamento, identidade e acessibilidade;
- componentes novos exigem alteração coordenada do C++ e do QSS;
- o carregamento do estilo torna-se parte obrigatória da inicialização e falha de forma controlada;
- testes devem validar tanto a ausência de QSS local quanto a presença dos seletores globais;
- a moldura e a barra de título nativas permanecem sob controle do sistema operacional enquanto a aplicação não adotar
  uma janela sem moldura própria;
- escala de tela e recursos de acessibilidade do sistema não são desativados por esta decisão.

## Alternativas consideradas

- **QSS inline em cada componente:** rejeitado porque espalha a identidade visual e dificulta auditoria.
- **Paleta global com `QPalette`:** rejeitada porque nem todos os estilos nativos respeitam a paleta igualmente e porque
  a declaração visual deixaria de estar concentrada no QSS.
- **Referências à paleta no QSS:** rejeitadas porque acompanham o tema do sistema.
- **Seletores universais:** rejeitados porque aplicam regras implicitamente a componentes futuros.
