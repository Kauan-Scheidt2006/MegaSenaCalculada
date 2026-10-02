#include "aplicacao/Aplicacao.hpp"

#include "interface/ambiente/Ambiente.hpp"
#include "interface/ambiente/AreaPaginas.hpp"
#include "interface/paginas/arvore/PaginaArvore.hpp"

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
    paginaArvore_ = new PaginaArvore(&ambiente_->areaPaginas());
    ambiente_->areaPaginas().adicionarPagina(*paginaArvore_);
    ambiente_->areaPaginas().exibirPagina(*paginaArvore_);
}
