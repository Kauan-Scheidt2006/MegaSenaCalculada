#include "aplicacao/Aplicacao.hpp"

#include "interface/ambiente/Ambiente.hpp"

#include <QCoreApplication>

#include <memory>
#include <stdexcept>

Aplicacao::Aplicacao() = default;

Aplicacao::~Aplicacao() = default;

void Aplicacao::iniciar()
{
    if (QCoreApplication::instance() == nullptr) {
        throw std::logic_error("QApplication deve existir antes de iniciar Aplicacao.");
    }

    criarInterface();
    ambiente_->apresentar();
}

void Aplicacao::criarInterface()
{
    ambiente_ = std::make_unique<Ambiente>();
}
