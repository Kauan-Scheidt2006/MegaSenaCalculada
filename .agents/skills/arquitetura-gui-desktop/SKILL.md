---
name: arquitetura-gui-desktop
description: Projetar ou revisar arquitetura, estados, acessibilidade, assincronismo e testes da GUI desktop deste projeto. Não use para escolher regras da Mega-Sena nem para implementar BST/AVL.
---

# Arquitetura da GUI desktop

Leia `AGENTS.md`. A GUI é uma camada de apresentação: recebe ações, aciona casos de uso e exibe estados e resultados. Ela não calcula frequência acumulada, não implementa comparadores ou árvores e não lê diretamente detalhes físicos do XLSX.

Modele explicitamente estados como vazio, carregando, sucesso, erro e cancelado. Operações demoradas devem ocorrer fora da linha principal da interface, expor progresso quando útil e não contaminar medições algorítmicas.

Revise navegação por teclado, ordem de foco, contraste, legibilidade e alternativas textuais para gráficos. Mensagens devem explicar o problema e a ação possível.

Prefira testar modelos de apresentação e transições de estado sem abrir janelas; use poucos testes ponta a ponta para fluxos críticos. Não imponha framework enquanto a tecnologia não tiver sido decidida.

