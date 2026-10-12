"""Gera gráficos comparativos para a apresentação Mega-Sena Calculada.

Os dados foram transcritos do relatório de execuções. As imagens são exportadas
em PNG, proporção 16:9 e resolução adequada para slides.
"""

from pathlib import Path
from statistics import mean, stdev

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.lines import Line2D
from matplotlib.ticker import FuncFormatter


# ---------------------------------------------------------------------------
# Dados
# ---------------------------------------------------------------------------

ESTRUTURAS = ["BST", "AVL", "Rubro-negra"]

REGISTROS_ALTURA = [
    600, 1200, 1800, 2400, 3000, 3600, 4200, 4800, 5400, 6000,
    6600, 7200, 7800, 8400, 9000, 9600, 10200, 10800, 11400, 12000,
    12600, 13200, 13800, 14400, 15000, 15600, 16200, 16800, 17400,
    18000, 18366,
]

ALTURA_BST = [
    30, 47, 61, 76, 90, 104, 116, 126, 136, 152, 172, 184, 202,
    215, 222, 233, 245, 253, 267, 277, 293, 304, 318, 334, 346,
    363, 375, 388, 400, 413, 416,
]
ALTURA_AVL = [
    11, 12, 13, 13, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
]
ALTURA_RN = [
    12, 13, 14, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19,
]

ALTURAS_FINAIS = [416, 16, 19]
COMPARACOES_INSERCAO = [7_205_373, 405_536, 455_852]
ROTACOES = [0, 13_524, 11_705]

TEMPOS_INSERCAO_MS = {
    "BST": [180.14, 198.52, 185.66, 179.76, 178.19, 202.08, 178.00, 183.73, 180.60, 179.11],
    "AVL": [65.93, 65.91, 67.19, 82.93, 67.30, 67.15, 67.15, 66.64, 67.37, 66.76],
    "Rubro-negra": [26.04, 25.74, 26.16, 26.45, 26.68, 26.22, 26.08, 26.38, 27.13, 25.92],
}

TEMPOS_PRE_ORDEM_MS = {
    "BST": [2.30, 2.82, 2.75, 2.32, 2.28, 2.27, 2.32, 2.27, 2.03, 2.24],
    "AVL": [2.26, 2.17, 2.20, 2.66, 2.18, 2.22, 2.20, 2.17, 2.21, 2.09],
    "Rubro-negra": [1.83, 1.89, 1.80, 1.92, 1.91, 1.86, 1.82, 1.86, 1.84, 1.88],
}

TEMPOS_EM_ORDEM_MS = {
    "BST": [2.00, 2.10, 1.97, 1.95, 2.04, 2.08, 2.08, 2.04, 1.83, 2.03],
    "AVL": [1.85, 1.90, 1.75, 1.77, 1.94, 1.95, 1.93, 1.96, 1.67, 2.05],
    "Rubro-negra": [1.87, 1.80, 1.91, 1.89, 2.02, 2.04, 1.88, 1.95, 1.86, 1.83],
}

TEMPOS_POS_ORDEM_MS = {
    "BST": [2.09, 2.10, 2.14, 1.96, 2.00, 2.36, 2.05, 2.08, 2.06, 2.04],
    "AVL": [2.05, 1.94, 2.01, 2.04, 1.87, 1.99, 2.09, 2.04, 1.98, 1.76],
    "Rubro-negra": [2.04, 2.06, 2.00, 1.99, 2.02, 2.35, 1.95, 1.98, 2.01, 1.96],
}

NOS_VISITADOS_BUSCA = [416, 16, 18]
COMPARACOES_BUSCA = [832, 32, 36]

TEMPOS_BUSCA_US = {
    "BST": [52.70, 50.30, 53.90, 52.00, 47.20, 51.20, 53.50, 52.10, 59.80, 60.00],
    "AVL": [3.20, 3.60, 3.00, 3.90, 4.10, 3.10, 3.30, 3.20, 3.30, 3.60],
    "Rubro-negra": [4.60, 4.30, 5.30, 4.60, 4.50, 5.30, 4.50, 3.90, 4.70, 4.60],
}

TEMPOS_REMOCAO_US = {
    "BST": [66.80, 60.40, 67.60, 67.40, 68.00, 82.50, 59.80, 68.00, 70.90, 63.40],
    "AVL": [13.40, 16.10, 13.30, 13.60, 13.10, 13.30, 13.60, 20.10, 15.00, 12.40],
    "Rubro-negra": [8.00, 8.30, 7.90, 7.10, 5.20, 7.70, 7.00, 7.40, 7.60, 6.80],
}


# ---------------------------------------------------------------------------
# Identidade visual
# ---------------------------------------------------------------------------

CORES = {
    "BST": "#F5B700",
    "AVL": "#2D9CDB",
    "Rubro-negra": "#E45756",
}
FUNDO = "#0B0D10"
FUNDO_SECUNDARIO = "#15191E"
TEXTO = "#F5F7FA"
TEXTO_SECUNDARIO = "#B7BEC8"
GRADE = "#343A43"
DESTAQUE = "#F5B700"

PASTA_SAIDA = Path(__file__).resolve().parent / "graficos"


def formatar_inteiro(valor, _pos=None):
    return f"{int(valor):,}".replace(",", ".")


def formatar_decimal(valor, casas=2):
    return f"{valor:.{casas}f}".replace(".", ",")


def configurar_estilo():
    plt.rcParams.update({
        "figure.facecolor": FUNDO,
        "axes.facecolor": FUNDO,
        "savefig.facecolor": FUNDO,
        "axes.edgecolor": GRADE,
        "axes.labelcolor": TEXTO_SECUNDARIO,
        "xtick.color": TEXTO_SECUNDARIO,
        "ytick.color": TEXTO_SECUNDARIO,
        "text.color": TEXTO,
        "font.family": "DejaVu Sans",
        "font.size": 13,
        "axes.titleweight": "bold",
        "axes.titlesize": 24,
        "axes.labelsize": 14,
        "xtick.labelsize": 12,
        "ytick.labelsize": 12,
        "legend.frameon": False,
        "legend.fontsize": 12,
    })


def nova_figura():
    return plt.figure(figsize=(16, 9), dpi=120, facecolor=FUNDO)


def titulo(figura, principal, apoio):
    figura.text(0.055, 0.935, principal, fontsize=25, fontweight="bold", color=TEXTO)
    figura.text(0.055, 0.895, apoio, fontsize=13, color=TEXTO_SECUNDARIO)


def rodape(figura, texto="18.366 registros | 3.061 concursos | mesma sequência de entrada"):
    figura.text(0.055, 0.025, texto, fontsize=10, color="#7F8792")


def finalizar(figura, nome):
    PASTA_SAIDA.mkdir(parents=True, exist_ok=True)
    figura.savefig(PASTA_SAIDA / nome, dpi=180, bbox_inches="tight", pad_inches=0.25)
    plt.close(figura)


def estilizar_eixo(eixo, grade="y"):
    eixo.spines["top"].set_visible(False)
    eixo.spines["right"].set_visible(False)
    eixo.spines["left"].set_color(GRADE)
    eixo.spines["bottom"].set_color(GRADE)
    eixo.grid(axis=grade, color=GRADE, linewidth=0.8, alpha=0.65)
    eixo.set_axisbelow(True)


def grafico_altura():
    fig = nova_figura()
    titulo(
        fig,
        "A BST chegou a 416 níveis; as árvores balanceadas ficaram abaixo de 20",
        "Evolução da altura medida a cada 600 registros inseridos",
    )
    ax = fig.add_axes([0.07, 0.12, 0.88, 0.70])

    series = [
        ("BST", ALTURA_BST, "o"),
        ("AVL", ALTURA_AVL, "s"),
        ("Rubro-negra", ALTURA_RN, "D"),
    ]
    for nome, valores, marcador in series:
        ax.plot(
            REGISTROS_ALTURA,
            valores,
            color=CORES[nome],
            linewidth=3.0 if nome == "BST" else 2.5,
            marker=marcador,
            markersize=5,
            markevery=3,
            label=nome,
        )
        ax.scatter(REGISTROS_ALTURA[-1], valores[-1], s=90, color=CORES[nome], zorder=5)
        deslocamento = 13 if nome == "BST" else (7 if nome == "Rubro-negra" else -14)
        ax.annotate(
            str(valores[-1]),
            (REGISTROS_ALTURA[-1], valores[-1]),
            xytext=(-8, deslocamento),
            textcoords="offset points",
            ha="right",
            fontsize=15,
            fontweight="bold",
            color=CORES[nome],
        )

    ax.set_xlabel("Registros inseridos")
    ax.set_ylabel("Altura")
    ax.set_xlim(0, 19_100)
    ax.set_ylim(0, 450)
    ax.xaxis.set_major_formatter(FuncFormatter(formatar_inteiro))
    estilizar_eixo(ax)
    ax.legend(loc="upper left", ncol=3)
    rodape(fig)
    finalizar(fig, "01_evolucao_altura.png")


def grafico_insercao_estrutural():
    fig = nova_figura()
    titulo(
        fig,
        "O balanceamento reduziu as comparações, com custo explícito em rotações",
        "Métricas determinísticas após 18.366 inserções",
    )

    ax1 = fig.add_axes([0.07, 0.14, 0.51, 0.67])
    ax2 = fig.add_axes([0.66, 0.14, 0.29, 0.67])
    y = np.arange(len(ESTRUTURAS))
    cores = [CORES[n] for n in ESTRUTURAS]

    barras = ax1.barh(y, COMPARACOES_INSERCAO, color=cores, height=0.56)
    ax1.set_yticks(y, ESTRUTURAS)
    ax1.invert_yaxis()
    ax1.set_xlabel("Chamadas ao comparador")
    ax1.xaxis.set_major_formatter(FuncFormatter(formatar_inteiro))
    ax1.set_title("Comparações", loc="left", fontsize=17, pad=14)
    ax1.set_xlim(0, 7_800_000)
    estilizar_eixo(ax1, "x")
    for barra, valor in zip(barras, COMPARACOES_INSERCAO):
        ax1.text(
            barra.get_width() + 90_000,
            barra.get_y() + barra.get_height() / 2,
            formatar_inteiro(valor),
            va="center",
            fontsize=13,
            fontweight="bold",
        )

    barras2 = ax2.barh(y, ROTACOES, color=cores, height=0.56)
    ax2.set_yticks(y, ESTRUTURAS)
    ax2.invert_yaxis()
    ax2.set_xlabel("Rotações primitivas")
    ax2.xaxis.set_major_formatter(FuncFormatter(formatar_inteiro))
    ax2.set_title("Rotações", loc="left", fontsize=17, pad=14)
    ax2.set_xlim(0, 15_500)
    estilizar_eixo(ax2, "x")
    for barra, valor in zip(barras2, ROTACOES):
        posicao = valor + 300 if valor else 250
        ax2.text(
            posicao,
            barra.get_y() + barra.get_height() / 2,
            formatar_inteiro(valor),
            va="center",
            fontsize=13,
            fontweight="bold",
        )

    fig.text(0.075, 0.075, "AVL: 94,4% menos comparações que a BST", color=CORES["AVL"], fontsize=12)
    fig.text(0.36, 0.075, "Rubro-negra: 93,7% menos", color=CORES["Rubro-negra"], fontsize=12)
    rodape(fig)
    finalizar(fig, "02_comparacoes_rotacoes.png")


def grafico_pontos(dados, unidade, principal, apoio, nome_arquivo, limite=None):
    fig = nova_figura()
    titulo(fig, principal, apoio)
    ax = fig.add_axes([0.09, 0.15, 0.84, 0.66])
    rng = np.random.default_rng(42)

    for indice, estrutura in enumerate(ESTRUTURAS):
        valores = np.array(dados[estrutura])
        dispersao_x = rng.uniform(-0.10, 0.10, size=len(valores))
        ax.scatter(
            np.full(len(valores), indice) + dispersao_x,
            valores,
            s=75,
            color=CORES[estrutura],
            alpha=0.82,
            edgecolors=FUNDO,
            linewidths=0.8,
            zorder=3,
        )
        media = mean(valores)
        desvio = stdev(valores)
        ax.plot([indice - 0.22, indice + 0.22], [media, media], color=TEXTO, linewidth=3.2, zorder=4)
        ax.text(
            indice,
            media + (limite[1] - limite[0]) * 0.045 if limite else media * 1.06,
            f"média {formatar_decimal(media)} {unidade}",
            ha="center",
            fontsize=12,
            fontweight="bold",
            color=CORES[estrutura],
        )
        ax.text(
            indice,
            media - (limite[1] - limite[0]) * 0.07 if limite else media * 0.91,
            f"DP {formatar_decimal(desvio)}",
            ha="center",
            fontsize=10,
            color=TEXTO_SECUNDARIO,
        )

    ax.set_xticks(range(len(ESTRUTURAS)), ESTRUTURAS)
    ax.set_ylabel(f"Tempo ({unidade})")
    ax.set_xlim(-0.55, 2.55)
    if limite:
        ax.set_ylim(*limite)
    estilizar_eixo(ax)
    ax.text(0.99, -0.13, "Cada ponto representa uma execução | n = 10", transform=ax.transAxes,
            ha="right", fontsize=10, color="#7F8792")
    rodape(fig)
    finalizar(fig, nome_arquivo)


def grafico_busca_deterministica():
    fig = nova_figura()
    titulo(
        fig,
        "A busca extrema percorreu 416 nós na BST e apenas 16 na AVL",
        "Custo determinístico para localizar a chave mais à direita (356, 10)",
    )
    ax1 = fig.add_axes([0.08, 0.16, 0.39, 0.64])
    ax2 = fig.add_axes([0.56, 0.16, 0.39, 0.64])
    y = np.arange(len(ESTRUTURAS))
    cores = [CORES[n] for n in ESTRUTURAS]

    for ax, valores, nome, limite in [
        (ax1, NOS_VISITADOS_BUSCA, "Nós visitados", 470),
        (ax2, COMPARACOES_BUSCA, "Comparações", 930),
    ]:
        barras = ax.barh(y, valores, color=cores, height=0.56)
        ax.set_yticks(y, ESTRUTURAS)
        ax.invert_yaxis()
        ax.set_xlim(0, limite)
        ax.set_title(nome, loc="left", fontsize=17, pad=14)
        estilizar_eixo(ax, "x")
        for barra, valor in zip(barras, valores):
            ax.text(
                valor + limite * 0.025,
                barra.get_y() + barra.get_height() / 2,
                formatar_inteiro(valor),
                va="center",
                fontsize=14,
                fontweight="bold",
            )

    rodape(fig, "Busca da chave extrema (356, 10) | métricas independentes do tempo da máquina")
    finalizar(fig, "04_busca_custo_deterministico.png")


def grafico_percursos():
    fig = nova_figura()
    titulo(
        fig,
        "Os percursos completos permaneceram próximos de 2 ms",
        "Média de dez execuções; cada percurso visita todos os 18.366 nós",
    )
    ax = fig.add_axes([0.17, 0.18, 0.66, 0.58])

    percursos = ["Pré-ordem", "Em ordem", "Pós-ordem"]
    fontes = [TEMPOS_PRE_ORDEM_MS, TEMPOS_EM_ORDEM_MS, TEMPOS_POS_ORDEM_MS]
    matriz = np.array([[mean(fonte[e]) for e in ESTRUTURAS] for fonte in fontes])

    imagem = ax.imshow(matriz, cmap="YlOrBr", vmin=1.75, vmax=2.40, aspect="auto")
    ax.set_xticks(range(3), ESTRUTURAS)
    ax.set_yticks(range(3), percursos)
    ax.tick_params(length=0, pad=12)
    for linha in range(3):
        for coluna in range(3):
            valor = matriz[linha, coluna]
            cor = FUNDO if valor > 2.05 else TEXTO
            ax.text(
                coluna,
                linha,
                f"{formatar_decimal(valor, 3)} ms",
                ha="center",
                va="center",
                fontsize=18,
                fontweight="bold",
                color=cor,
            )

    for spine in ax.spines.values():
        spine.set_visible(False)
    cbar = fig.colorbar(imagem, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("Tempo médio (ms)", color=TEXTO_SECUNDARIO)
    cbar.ax.tick_params(colors=TEXTO_SECUNDARIO)
    cbar.outline.set_edgecolor(GRADE)

    fig.text(
        0.5,
        0.105,
        "A proximidade dos valores é coerente com o custo linear dos percursos completos",
        ha="center",
        fontsize=13,
        color=TEXTO_SECUNDARIO,
    )
    rodape(fig)
    finalizar(fig, "07_percursos_completos.png")


def grafico_sintese():
    fig = nova_figura()
    titulo(
        fig,
        "O balanceamento reduziu o caminho das operações",
        "Síntese dos principais resultados observados",
    )

    blocos = [
        ("416", "16", "níveis", "BST", "AVL"),
        ("832", "32", "comparações na busca", "BST", "AVL"),
        ("67,48", "7,30", "µs na remoção", "BST", "Rubro-negra"),
    ]
    centros = [0.20, 0.50, 0.80]
    for centro, (antes, depois, medida, origem, destino) in zip(centros, blocos):
        fig.text(centro, 0.67, antes, ha="center", fontsize=43, fontweight="bold", color=CORES[origem])
        fig.text(centro, 0.56, "para", ha="center", fontsize=15, color=TEXTO_SECUNDARIO)
        fig.text(centro, 0.39, depois, ha="center", fontsize=52, fontweight="bold", color=CORES[destino])
        fig.text(centro, 0.29, medida, ha="center", fontsize=14, color=TEXTO)
        fig.text(centro, 0.22, f"{origem} comparada à {destino}", ha="center", fontsize=10, color="#7F8792")

    fig.lines.extend([
        Line2D([0.35, 0.35], [0.18, 0.73], transform=fig.transFigure, color=GRADE, linewidth=1.2),
        Line2D([0.65, 0.65], [0.18, 0.73], transform=fig.transFigure, color=GRADE, linewidth=1.2),
    ])
    rodape(fig, "Resultados da sequência histórica e da implementação avaliadas")
    finalizar(fig, "08_sintese_resultados.png")


def main():
    configurar_estilo()
    grafico_altura()
    grafico_insercao_estrutural()
    grafico_pontos(
        TEMPOS_INSERCAO_MS,
        "ms",
        "A rubro-negra apresentou o menor tempo e a menor variação na inserção",
        "Dez execuções por estrutura; a linha branca indica a média",
        "03_tempo_insercao.png",
        limite=(0, 225),
    )
    return 
    
    grafico_busca_deterministica()
    grafico_pontos(
        TEMPOS_BUSCA_US,
        "µs",
        "A AVL localizou a chave extrema cerca de 15 vezes mais rápido que a BST",
        "Tempo para localizar o elemento mais à direita (356, 10)",
        "05_tempo_busca_extrema.png",
        limite=(0, 67),
    )
    grafico_pontos(
        TEMPOS_REMOCAO_US,
        "µs",
        "A rubro-negra apresentou o menor tempo de remoção da chave extrema",
        "Dez execuções da remoção da chave (356, 10)",
        "06_tempo_remocao.png",
        limite=(0, 92),
    )
    grafico_percursos()
    grafico_sintese()
    print(f"Gráficos gerados em: {PASTA_SAIDA}")


if __name__ == "__main__":
    main()
