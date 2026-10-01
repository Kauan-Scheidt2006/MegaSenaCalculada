#include "ArvoreVisualizacao.h"
#include "concursosTestes.h"

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>

int main(int quantidadeArgumentos, char* argumentos[])
{
    QApplication aplicacao(quantidadeArgumentos, argumentos);
    aplicacao.setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget#conteudo { background: #eef3f8; }
        QLabel { color: #17324d; font-family: "Segoe UI"; }
        QFrame#popoverDetalhes { background: white; border: 1px solid #cad6e2;
                                border-radius: 12px; }
        QLabel#numeroDetalhe { font-size: 20px; font-weight: 700; }
        QPushButton#fecharPopover { border: none; border-radius: 15px;
                                   background: #edf2f6; font-size: 18px; }
        QPushButton#fecharPopover:hover { background: #dfe8ef; }
        QPushButton#fecharPopover:focus { border: 2px solid #ffb020; }
    )"));

    ArvoreVisualizacao::ArvoreRegistros arvore;
    for (const megasena::Registro& registro :
         testes_apresentacao::registrosConcursosTeste) {
        if (!arvore.inserir(registro)) {
            return 1;
        }
    }

    QMainWindow janela;
    janela.setWindowTitle(QStringLiteral("Teste de apresentação da árvore"));
    janela.resize(1100, 720);

    auto* conteudo = new QWidget;
    conteudo->setObjectName(QStringLiteral("conteudo"));
    auto* disposicao = new QVBoxLayout(conteudo);
    auto* titulo = new QLabel(QStringLiteral("Ocorrências de teste da Mega-Sena"));
    QFont fonte = titulo->font();
    fonte.setPointSize(17);
    fonte.setBold(true);
    titulo->setFont(fonte);
    auto* instrucao = new QLabel(QStringLiteral(
        "Clique em uma dezena — ou use Tab e Enter — para ver frequência e concurso."));
    auto* visualizacao = new ArvoreVisualizacao(arvore);
    disposicao->addWidget(titulo);
    disposicao->addWidget(instrucao);
    disposicao->addWidget(visualizacao);
    janela.setCentralWidget(conteudo);
    janela.show();

    QTimer::singleShot(0, visualizacao, &ArvoreVisualizacao::enquadrarArvore);
    return aplicacao.exec();
}
