#include "interface/fundamentos/HospedagemRaiz.hpp"

#include <QVBoxLayout>
#include <QWidget>

#include <stdexcept>

void HospedagemRaiz::aplicar(QWidget& hospedeiro, QWidget& conteudo)
{
    if (hospedeiro.layout() != nullptr) {
        throw std::logic_error("O hospedeiro ja possui uma disposicao raiz.");
    }

    auto* disposicao = new QVBoxLayout(&hospedeiro);
    disposicao->setContentsMargins(0, 0, 0, 0);
    disposicao->setSpacing(0);
    disposicao->addWidget(&conteudo);
}
