"""Gera um gráfico de barras para a busca da chave extrema à direita."""

from pathlib import Path
from statistics import mean

import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


# Dados experimentais.
estruturas = ["BST", "AVL", "Rubro-negra"]
nos_visitados = [416, 16, 18]
comparacoes = [832, 32, 36]

tempos_busca_us = {
    "BST": [52.70, 50.30, 53.90, 52.00, 47.20, 51.20, 53.50, 52.10, 59.80, 60.00],
    "AVL": [3.20, 3.60, 3.00, 3.90, 4.10, 3.10, 3.30, 3.20, 3.30, 3.60],
    "Rubro-negra": [4.60, 4.30, 5.30, 4.60, 4.50, 5.30, 4.50, 3.90, 4.70, 4.60],
}


CORES = ["#D99B00", "#2478B7", "#C84444"]
COR_GRADE = "#C2C2C2"
COR_TEXTO = "#222222"

PASTA_SAIDA = Path(__file__).resolve().parent / "graficos_busca"


def decimal_br(valor, casas=2):
    return f"{valor:.{casas}f}".replace(".", ",")


def inteiro_br(valor, _pos=None):
    return f"{int(valor):,}".replace(",", ".")


def estilizar_eixo(ax):
    ax.set_facecolor("white")
    ax.grid(axis="y", color=COR_GRADE, linewidth=0.8, alpha=0.8)
    ax.set_axisbelow(True)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.tick_params(colors=COR_TEXTO, labelsize=10)


def adicionar_rotulos(ax, barras, textos, limite):
    for barra, texto in zip(barras, textos):
        altura = barra.get_height()
        ax.text(
            barra.get_x() + barra.get_width() / 2,
            altura + limite * 0.025,
            texto,
            ha="center",
            va="bottom",
            fontsize=10,
            fontweight="semibold",
            color=COR_TEXTO,
        )


def gerar_grafico():
    medias_tempo = [mean(tempos_busca_us[nome]) for nome in estruturas]

    fig, (ax_comparacoes, ax_tempo) = plt.subplots(
        1,
        2,
        figsize=(13, 6.5),
        facecolor="white",
        gridspec_kw={"wspace": 0.28},
    )

    barras_comparacoes = ax_comparacoes.bar(
        estruturas,
        comparacoes,
        color=CORES,
        width=0.62,
    )
    estilizar_eixo(ax_comparacoes)
    ax_comparacoes.set_title("Comparações realizadas", fontsize=14, pad=12)
    ax_comparacoes.set_ylabel("Quantidade de comparações", fontsize=11)
    ax_comparacoes.set_ylim(0, 930)
    ax_comparacoes.yaxis.set_major_formatter(FuncFormatter(inteiro_br))
    rotulos_comparacoes = [
        f"{comparacao} comparações\n{nos} nós visitados"
        for comparacao, nos in zip(comparacoes, nos_visitados)
    ]
    adicionar_rotulos(ax_comparacoes, barras_comparacoes, rotulos_comparacoes, 930)

    barras_tempo = ax_tempo.bar(
        estruturas,
        medias_tempo,
        color=CORES,
        width=0.62,
    )
    estilizar_eixo(ax_tempo)
    ax_tempo.set_title("Tempo médio de busca", fontsize=14, pad=12)
    ax_tempo.set_ylabel("Tempo médio (µs)", fontsize=11)
    ax_tempo.set_ylim(0, 60)
    rotulos_tempo = [f"{decimal_br(valor)} µs" for valor in medias_tempo]
    adicionar_rotulos(ax_tempo, barras_tempo, rotulos_tempo, 60)

    fig.suptitle(
        "Busca da chave extrema à direita",
        fontsize=19,
        y=0.98,
    )
    fig.text(
        0.5,
        0.02,
        "A altura da árvore determina quantos nós precisam ser percorridos até o elemento mais à direita.",
        ha="center",
        fontsize=11,
        color="#555555",
    )

    fig.subplots_adjust(top=0.84, bottom=0.14, left=0.08, right=0.97)
    PASTA_SAIDA.mkdir(parents=True, exist_ok=True)
    destino = PASTA_SAIDA / "01_busca_chave_extrema_direita.png"
    fig.savefig(destino, dpi=220, bbox_inches="tight")
    plt.close(fig)
    print(f"Gráfico gerado em: {destino}")


if __name__ == "__main__":
    gerar_grafico()
