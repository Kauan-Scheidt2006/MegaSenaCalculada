#pragma once

struct ParametrosComponente;
class QWidget;

/** Aplica a configuração comum dentro das especializações visuais do projeto. */
class ConfiguradorComponente final
{
public:
    static void aplicar(QWidget& componente, const ParametrosComponente& parametros);

    ConfiguradorComponente() = delete;
};
