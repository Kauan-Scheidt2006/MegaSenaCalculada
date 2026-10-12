"""Gera os dois gráficos de evolução da altura das árvores.

A separação em duas figuras evita que a escala da BST esconda as variações
menores das árvores AVL e rubro-negra.
"""

from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter, MaxNLocator


# Dados experimentais: altura registrada a cada 600 inserções.
registros = [
    600, 1200, 1800, 2400, 3000, 3600, 4200, 4800, 5400, 6000,
    6600, 7200, 7800, 8400, 9000, 9600, 10200, 10800, 11400, 12000,
    12600, 13200, 13800, 14400, 15000, 15600, 16200, 16800, 17400,
    18000, 18366,
]

altura_bst = [
    30, 47, 61, 76, 90, 104, 116, 126, 136, 152, 172, 184, 202,
    215, 222, 233, 245, 253, 267, 277, 293, 304, 318, 334, 346,
    363, 375, 388, 400, 413, 416,
]

altura_avl = [
    11, 12, 13, 13, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
]

altura_rubro_negra = [
    12, 13, 14, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19,
]


PASTA_SAIDA = Path(__file__).resolve().parent / "graficos_altura"

COR_BST = "#BD8700"
COR_AVL = "#2478B7"
COR_RN = "#C84444"
COR_GRADE = "#B7B7B7"
COR_TEXTO = "#222222"


def formatar_milhar(valor, _pos=None):
    return f"{int(valor):,}".replace(",", ".")


def preparar_eixo(ax):
    ax.grid(True, color=COR_GRADE, linewidth=0.8, alpha=0.8)
    ax.set_axisbelow(True)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.tick_params(colors=COR_TEXTO, labelsize=10)
    ax.xaxis.set_major_formatter(FuncFormatter(formatar_milhar))
    ax.set_xlabel("Quantidade de registros inseridos", fontsize=11)
    ax.set_ylabel("Altura da árvore", fontsize=11)


def anotar_marcos(ax, alturas, indices, cor, prefixo=""):
    for indice in indices:
        x = registros[indice]
        y = alturas[indice]
        texto = f"{prefixo}{formatar_milhar(x)} registros\naltura {y}"
        deslocamento = (0, 35)
        if indice == len(registros) - 1:
            deslocamento = (-8, 8)
        ax.annotate(
            texto,
            (x, y),
            xytext=deslocamento,
            textcoords="offset points",
            ha="right" if indice == len(registros) - 1 else "left",
            va="bottom",
            fontsize=9,
            color=cor,
            fontweight="semibold",
        )


def grafico_bst():
    fig, ax = plt.subplots(figsize=(10, 5.8), facecolor="white")
    ax.set_facecolor("white")

    ax.plot(
        registros,
        altura_bst,
        color=COR_BST,
        marker="o",
        markersize=4.5,
        linewidth=2.2,
        label="BST",
    )

    anotar_marcos(ax, altura_bst, [0, 9, 19, 30], COR_BST)
    preparar_eixo(ax)
    ax.set_title("Evolução da altura da árvore BST", fontsize=15, pad=14)
    ax.set_xlim(0, 19_200)
    ax.set_ylim(0, 455)
    ax.legend(loc="upper left", frameon=False, fontsize=10)

    fig.tight_layout()
    fig.savefig(PASTA_SAIDA / "01_altura_bst.png", dpi=220, bbox_inches="tight")
    plt.close(fig)


def grafico_balanceadas():
    fig, ax = plt.subplots(figsize=(10, 5.8), facecolor="white")
    ax.set_facecolor("white")

    ax.plot(
        registros,
        altura_avl,
        color=COR_AVL,
        marker="o",
        markersize=4.5,
        linewidth=2.2,
        label="AVL",
    )
    ax.plot(
        registros,
        altura_rubro_negra,
        color=COR_RN,
        marker="o",
        markersize=4.5,
        linewidth=2.2,
        label="Rubro-negra",
    )

    # Três pontos de referência em cada curva evitam sobreposição entre os textos.
    marcos = [9, 19, 30]
    anotar_marcos(ax, altura_avl, marcos, COR_AVL, "AVL: ")

    # Os textos da rubro-negra ficam abaixo dos pontos para não se sobreporem.
    for indice in marcos:
        x = registros[indice]
        y = altura_rubro_negra[indice]
        ha = "right" if indice == len(registros) - 1 else "left"
        dx = -8 if indice == len(registros) - 1 else 7
        ax.annotate(
            f"RN: {formatar_milhar(x)} registros\naltura {y}",
            (x, y),
            xytext=(dx, -10),
            textcoords="offset points",
            ha=ha,
            va="top",
            fontsize=9,
            color=COR_RN,
            fontweight="semibold",
        )

    preparar_eixo(ax)
    ax.set_title("Evolução da altura das árvores balanceadas", fontsize=15, pad=14)
    ax.set_xlim(0, 19_200)
    ax.set_ylim(10, 20)
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))
    ax.legend(loc="lower right", frameon=False, fontsize=10, ncol=2)

    fig.tight_layout()
    fig.savefig(PASTA_SAIDA / "02_altura_arvores_balanceadas.png", dpi=220, bbox_inches="tight")
    plt.close(fig)


def main():
    PASTA_SAIDA.mkdir(parents=True, exist_ok=True)
    grafico_bst()
    grafico_balanceadas()
    print(f"Gráficos de altura gerados em: {PASTA_SAIDA}")


if __name__ == "__main__":
    main()
