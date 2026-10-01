#pragma once

#include <QWidget>

class AreaPaginas;
class BarraEstado;
class EstruturaAmbiente;

/** Componente visual superior e Dono Qt da estrutura permanente da interface. */
class Ambiente final : public QWidget
{
public:
    explicit Ambiente(QWidget* pai = nullptr);

    void apresentar();
    [[nodiscard]] AreaPaginas& areaPaginas() const noexcept;
    [[nodiscard]] BarraEstado& barraEstado() const noexcept;

private:
    void configurarJanela();
    void criarEstrutura();

    EstruturaAmbiente* estrutura_ = nullptr;
};
