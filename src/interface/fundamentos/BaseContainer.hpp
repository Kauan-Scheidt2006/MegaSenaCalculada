#pragma once

#include "interface/parametros/ParametrosContainer.hpp"

#include <QWidget>

#include <optional>

class QGridLayout;
class QLayout;
class QStackedLayout;

/**
 * Fundamento visual que encapsula a criação e a manipulação dos layouts Qt.
 * O pai recebido no construtor é o Dono Qt do container.
 */
class BaseContainer : public QWidget
{
public:
    explicit BaseContainer(const ParametrosContainer& parametros, QWidget* pai = nullptr);
    ~BaseContainer() override = default;

    BaseContainer(const BaseContainer&) = delete;
    BaseContainer& operator=(const BaseContainer&) = delete;
    BaseContainer(BaseContainer&&) = delete;
    BaseContainer& operator=(BaseContainer&&) = delete;

protected:
    void adicionarComponente(QWidget& componente, int proporcao = 0,
                             std::optional<Qt::Alignment> alinhamento = std::nullopt);
    void adicionarComponenteNaGrade(QWidget& componente, int linha, int coluna,
                                    int quantidadeLinhas = 1, int quantidadeColunas = 1,
                                    std::optional<Qt::Alignment> alinhamento = std::nullopt);
    void adicionarEspacamentoExpansivel(int proporcao = 0);
    void exibirComponente(QWidget& componente);
    void removerComponente(QWidget& componente);

    [[nodiscard]] TipoDisposicao tipoDisposicao() const noexcept;
    [[nodiscard]] int quantidadeComponentes() const noexcept;

private:
    QLayout* criarDisposicao(const ParametrosContainer& parametros);
    [[nodiscard]] QGridLayout& obterGrade();
    [[nodiscard]] QStackedLayout& obterPilha();

    TipoDisposicao tipoDisposicao_;
    QLayout* disposicao_ = nullptr;
};
