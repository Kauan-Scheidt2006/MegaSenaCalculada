#include "aplicacao/Aplicacao.hpp"

#include <QApplication>
#include <QDebug>

#include <cstdlib>
#include <exception>

int main(int quantidadeArgumentos, char* argumentos[])
{
    QApplication aplicacaoQt(quantidadeArgumentos, argumentos);
    QApplication::setApplicationName(QStringLiteral("Mega Sena Calculada"));
    QApplication::setOrganizationName(QStringLiteral("Mega Sena Calculada"));

    try {
        Aplicacao aplicacao;
        aplicacao.iniciar();
        return aplicacaoQt.exec();
    } catch (const std::exception& erro) {
        qCritical() << "Falha na inicialização:" << erro.what();
        return EXIT_FAILURE;
    }
}
