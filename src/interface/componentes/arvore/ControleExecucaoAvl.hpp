#pragma once

#include "interface/fundamentos/Barra.hpp"

#include <cstddef>

class Botao;
class Texto;

class ControleExecucaoAvl final : public Barra
{
    Q_OBJECT

public:
    explicit ControleExecucaoAvl(QWidget* pai = nullptr);

    void definirModoAtivo(bool ativo);
    void definirExecutando(bool executando);
    void definirConcluido(bool concluido);
    void definirInformacoes(const QString& descricao, std::size_t registroAtual,
                            std::size_t totalRegistros, std::size_t numeroPasso);

signals:
    void proximoPassoSolicitado();
    void reproducaoSolicitada();
    void pausaSolicitada();
    void reinicioSolicitado();

private:
    Botao* proximo_ = nullptr;
    Botao* reproduzir_ = nullptr;
    Botao* pausar_ = nullptr;
    Botao* reiniciar_ = nullptr;
    Texto* progresso_ = nullptr;
    Texto* descricao_ = nullptr;
    bool concluido_ = false;
};
