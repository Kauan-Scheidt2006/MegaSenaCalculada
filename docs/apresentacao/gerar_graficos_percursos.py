"""Gera alternativas visuais para comparar os três percursos das árvores."""

from pathlib import Path
from statistics import mean

import matplotlib.pyplot as plt
import numpy as np


ESTRUTURAS = ["BST", "AVL", "Rubro-negra"]
PERCURSOS = ["Pré-ordem", "Em ordem", "Pós-ordem"]

TEMPOS = {
    "Pré-ordem": {
        "BST": [2.30, 2.82, 2.75, 2.32, 2.28, 2.27, 2.32, 2.27, 2.03, 2.24],
        "AVL": [2.26, 2.17, 2.20, 2.66, 2.18, 2.22, 2.20, 2.17, 2.21, 2.09],
        "Rubro-negra": [1.83, 1.89, 1.80, 1.92, 1.91, 1.86, 1.82, 1.86, 1.84, 1.88],
    },
    "Em ordem": {
        "BST": [2.00, 2.10, 1.97, 1.95, 2.04, 2.08, 2.08, 2.04, 1.83, 2.03],
        "AVL": [1.85, 1.90, 1.75, 1.77, 1.94, 1.95, 1.93, 1.96, 1.67, 2.05],
        "Rubro-negra": [1.87, 1.80, 1.91, 1.89, 2.02, 2.04, 1.88, 1.95, 1.86, 1.83],
    },
    "Pós-ordem": {
        "BST": [2.09, 2.10, 2.14, 1.96, 2.00, 2.36, 2.05, 2.08, 2.06, 2.04],
        "AVL": [2.05, 1.94, 2.01, 2.04, 1.87, 1.99, 2.09, 2.04, 1.98, 1.76],
        "Rubro-negra": [2.04, 2.06, 2.00, 1.99, 2.02, 2.35, 1.95, 1.98, 2.01, 1.96],
    },
}

CORES = {"BST": "#D99B00", "AVL": "#2478B7", "Rubro-negra": "#C84444"}
GRADE = "#C4C4C4"
TEXTO = "#222222"
SAIDA = Path(__file__).resolve().parent / "graficos_percursos"


def medias():
    return {
        percurso: {estrutura: mean(TEMPOS[percurso][estrutura]) for estrutura in ESTRUTURAS}
        for percurso in PERCURSOS
    }


def decimal_br(valor):
    return f"{valor:.3f}".replace(".", ",")


def estilizar(ax, eixo_grade="y"):
    ax.set_facecolor("white")
    ax.grid(axis=eixo_grade, color=GRADE, linewidth=0.8, alpha=0.8)
    ax.set_axisbelow(True)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.tick_params(colors=TEXTO, labelsize=10)


def salvar(fig, nome):
    SAIDA.mkdir(parents=True, exist_ok=True)
    fig.savefig(SAIDA / nome, dpi=220, bbox_inches="tight", facecolor="white")
    plt.close(fig)


def barras_verticais(valores):
    fig, ax = plt.subplots(figsize=(11, 6), facecolor="white")
    x = np.arange(len(PERCURSOS))
    largura = 0.23

    for indice, estrutura in enumerate(ESTRUTURAS):
        serie = [valores[p][estrutura] for p in PERCURSOS]
        barras = ax.bar(
            x + (indice - 1) * largura,
            serie,
            largura,
            label=estrutura,
            color=CORES[estrutura],
        )
        for barra, valor in zip(barras, serie):
            ax.text(
                barra.get_x() + barra.get_width() / 2,
                valor + 0.045,
                decimal_br(valor),
                ha="center",
                fontsize=9,
                fontweight="semibold",
            )

    estilizar(ax)
    ax.set_title("Tempo médio dos percursos", fontsize=17, pad=15)
    ax.set_ylabel("Tempo médio (ms)", fontsize=11)
    ax.set_xticks(x, PERCURSOS)
    ax.set_ylim(0, 2.8)
    ax.legend(frameon=False, ncol=3, loc="upper right")
    fig.tight_layout()
    salvar(fig, "01_barras_verticais_agrupadas.png")


def barras_horizontais(valores):
    fig, ax = plt.subplots(figsize=(11, 6), facecolor="white")
    y = np.arange(len(PERCURSOS))
    altura = 0.23

    for indice, estrutura in enumerate(ESTRUTURAS):
        serie = [valores[p][estrutura] for p in PERCURSOS]
        barras = ax.barh(
            y + (indice - 1) * altura,
            serie,
            altura,
            label=estrutura,
            color=CORES[estrutura],
        )
        for barra, valor in zip(barras, serie):
            ax.text(
                valor + 0.025,
                barra.get_y() + barra.get_height() / 2,
                f"{decimal_br(valor)} ms",
                va="center",
                fontsize=9.5,
                fontweight="semibold",
            )

    estilizar(ax, "x")
    ax.set_title("Tempo médio dos percursos", fontsize=17, pad=15)
    ax.set_xlabel("Tempo médio (ms)", fontsize=11)
    ax.set_yticks(y, PERCURSOS)
    ax.invert_yaxis()
    ax.set_xlim(0, 2.8)
    ax.legend(frameon=False, ncol=3, loc="lower right")
    fig.tight_layout()
    salvar(fig, "02_barras_horizontais_agrupadas.png")


def barras_sobrepostas(valores):
    fig, ax = plt.subplots(figsize=(11, 6), facecolor="white")
    x = np.arange(len(PERCURSOS))
    larguras = {"BST": 0.72, "AVL": 0.50, "Rubro-negra": 0.28}

    # A barra mais larga fica atrás e as menores permanecem visíveis à frente.
    for estrutura in ESTRUTURAS:
        serie = [valores[p][estrutura] for p in PERCURSOS]
        ax.bar(
            x,
            serie,
            larguras[estrutura],
            label=estrutura,
            color=CORES[estrutura],
            alpha=0.92,
        )
        for posicao, valor in zip(x, serie):
            deslocamento = {"BST": -0.25, "AVL": 0.0, "Rubro-negra": 0.25}[estrutura]
            ax.text(
                posicao + deslocamento,
                valor + 0.045,
                decimal_br(valor),
                ha="center",
                fontsize=9,
                fontweight="semibold",
                color=CORES[estrutura],
            )

    estilizar(ax)
    ax.set_title("Percursos com barras sobrepostas", fontsize=17, pad=15)
    ax.set_ylabel("Tempo médio (ms)", fontsize=11)
    ax.set_xticks(x, PERCURSOS)
    ax.set_ylim(0, 2.8)
    ax.legend(frameon=False, ncol=3, loc="upper right")
    fig.tight_layout()
    salvar(fig, "03_barras_sobrepostas.png")


def pontos_comparativos(valores):
    fig, ax = plt.subplots(figsize=(11, 6), facecolor="white")
    y = np.arange(len(PERCURSOS))

    for linha, percurso in enumerate(PERCURSOS):
        serie = [valores[percurso][e] for e in ESTRUTURAS]
        ax.hlines(linha, min(serie), max(serie), color=GRADE, linewidth=3, zorder=1)
        for estrutura, valor in zip(ESTRUTURAS, serie):
            ax.scatter(valor, linha, s=130, color=CORES[estrutura], zorder=3, label=estrutura if linha == 0 else None)
            ax.text(valor, linha - 0.13, decimal_br(valor), ha="center", va="top", fontsize=9, color=CORES[estrutura], fontweight="semibold")

    estilizar(ax, "x")
    ax.set_title("Diferenças entre os tempos médios", fontsize=17, pad=15)
    ax.set_xlabel("Tempo médio (ms)", fontsize=11)
    ax.set_yticks(y, PERCURSOS)
    ax.invert_yaxis()
    ax.set_xlim(1.70, 2.50)
    ax.legend(frameon=False, ncol=3, loc="lower right")
    fig.tight_layout()
    salvar(fig, "04_pontos_comparativos.png")


def main():
    valores = medias()
    barras_verticais(valores)
    barras_horizontais(valores)
    barras_sobrepostas(valores)
    pontos_comparativos(valores)
    print(f"Alternativas geradas em: {SAIDA}")


if __name__ == "__main__":
    main()
