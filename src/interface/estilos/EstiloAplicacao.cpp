#include "interface/estilos/EstiloAplicacao.hpp"

#include <QApplication>
#include <QFile>

#include <stdexcept>

void EstiloAplicacao::aplicar(QApplication& aplicacao)
{
    Q_INIT_RESOURCE(recursos);

    QFile arquivo(QStringLiteral(":/estilos/tema-padrao.qss"));
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Nao foi possivel carregar o estilo visual da aplicacao.");
    }

    const QString folhaEstilos = QString::fromUtf8(arquivo.readAll());
    if (folhaEstilos.trimmed().isEmpty()) {
        throw std::runtime_error("O arquivo de estilo visual da aplicacao esta vazio.");
    }

    aplicacao.setStyleSheet(folhaEstilos);
}
