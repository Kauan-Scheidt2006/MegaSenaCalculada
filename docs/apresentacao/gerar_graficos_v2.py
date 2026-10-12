"""Segunda proposta visual para os gráficos da apresentação.

Abordagem editorial: fundo claro, tipografia forte, poucos elementos por tela e
comparações orientadas à conclusão. Os dados permanecem definidos como listas
em ``gerar_graficos.py`` e são reutilizados aqui sem duplicação.
"""

from pathlib import Path
from statistics import mean, stdev

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle
from matplotlib.ticker import FuncFormatter

from gerar_graficos import (
    ALTURA_AVL,
    ALTURA_BST,
    ALTURA_RN,
    COMPARACOES_BUSCA,
    COMPARACOES_INSERCAO,
    ESTRUTURAS,
    NOS_VISITADOS_BUSCA,
    REGISTROS_ALTURA,
    ROTACOES,
    TEMPOS_BUSCA_US,
    TEMPOS_EM_ORDEM_MS,
    TEMPOS_INSERCAO_MS,
    TEMPOS_POS_ORDEM_MS,
    TEMPOS_PRE_ORDEM_MS,
    TEMPOS_REMOCAO_US,
)


# ---------------------------------------------------------------------------
# Identidade visual editorial
# ---------------------------------------------------------------------------

PAPEL = "#F1EBDD"
PAPEL_CLARO = "#FAF7EF"
PRETO = "#151515"
CINZA = "#5F625F"
CINZA_CLARO = "#CFC8B8"
AMARELO = "#F3C623"
AZUL = "#476C7A"
VERMELHO = "#C94949"

CORES = {
    "BST": AMARELO,
    "AVL": AZUL,
    "Rubro-negra": VERMELHO,
}

SAIDA = Path(__file__).resolve().parent / "graficos_v2"


def inteiro_br(valor, _pos=None):
    return f"{int(valor):,}".replace(",", ".")


def decimal_br(valor, casas=2):
    return f"{valor:.{casas}f}".replace(".", ",")


def configurar():
    plt.rcParams.update({
        "figure.facecolor": PAPEL,
        "axes.facecolor": PAPEL,
        "savefig.facecolor": PAPEL,
        "font.family": "DejaVu Sans",
        "text.color": PRETO,
        "axes.labelcolor": CINZA,
        "xtick.color": CINZA,
        "ytick.color": CINZA,
        "axes.edgecolor": CINZA_CLARO,
        "font.size": 13,
    })


def figura():
    return plt.figure(figsize=(16, 9), dpi=120, facecolor=PAPEL)


def cabecalho(fig, secao, titulo, subtitulo=None):
    fig.patches.append(Rectangle((0, 0.87), 1, 0.13, transform=fig.transFigure,
                                 facecolor=PRETO, edgecolor="none", zorder=-1))
    fig.text(0.045, 0.954, secao.upper(), fontsize=11, fontweight="bold", color=AMARELO)
    fig.text(0.045, 0.902, titulo, fontsize=23, fontweight="bold", color=PAPEL_CLARO)
    if subtitulo:
        fig.text(0.955, 0.835, subtitulo, fontsize=10, color=CINZA, ha="right")


def nota(fig, texto):
    fig.text(0.045, 0.028, texto, fontsize=9.5, color="#77766F")


def salvar(fig, nome):
    SAIDA.mkdir(parents=True, exist_ok=True)
    fig.savefig(SAIDA / nome, dpi=180, bbox_inches="tight", pad_inches=0.18)
    plt.close(fig)


def limpar(ax, grade=False):
    ax.spines[["top", "right", "left"]].set_visible(False)
    ax.spines["bottom"].set_color(CINZA_CLARO)
    ax.tick_params(length=0)
    if grade:
        ax.grid(axis="y", color=CINZA_CLARO, linewidth=0.8, alpha=0.75)
        ax.set_axisbelow(True)


def grafico_altura_com_ampliacao():
    fig = figura()
    cabecalho(
        fig,
        "Resultado estrutural",
        "A sequência histórica separou radicalmente a BST das árvores balanceadas",
        "altura a cada 600 inserções",
    )

    ax_principal = fig.add_axes([0.055, 0.14, 0.56, 0.64])
    ax_zoom = fig.add_axes([0.69, 0.19, 0.27, 0.52], facecolor=PAPEL_CLARO)

    ax_principal.fill_between(REGISTROS_ALTURA, ALTURA_BST, color=AMARELO, alpha=0.22)
    ax_principal.plot(REGISTROS_ALTURA, ALTURA_BST, color=PRETO, linewidth=3.2)
    ax_principal.scatter(REGISTROS_ALTURA[::5], ALTURA_BST[::5], s=48, color=AMARELO,
                         edgecolor=PRETO, linewidth=0.8, zorder=3)
    ax_principal.text(REGISTROS_ALTURA[-1] - 450, ALTURA_BST[-1] + 16, "416",
                      ha="right", fontsize=32, fontweight="bold", color=PRETO)
    ax_principal.text(550, 380, "BST", fontsize=15, fontweight="bold", color=PRETO)
    ax_principal.set_ylim(0, 455)
    ax_principal.set_xlim(0, 19_000)
    ax_principal.set_ylabel("Altura")
    ax_principal.set_xlabel("Registros inseridos")
    ax_principal.xaxis.set_major_formatter(FuncFormatter(inteiro_br))
    limpar(ax_principal, grade=True)

    ax_zoom.plot(REGISTROS_ALTURA, ALTURA_AVL, color=AZUL, linewidth=3.0, label="AVL")
    ax_zoom.plot(REGISTROS_ALTURA, ALTURA_RN, color=VERMELHO, linewidth=3.0,
                 label="Rubro-negra")
    ax_zoom.scatter(REGISTROS_ALTURA[-1], ALTURA_AVL[-1], s=75, color=AZUL, zorder=3)
    ax_zoom.scatter(REGISTROS_ALTURA[-1], ALTURA_RN[-1], s=75, color=VERMELHO, zorder=3)
    ax_zoom.text(17_950, 15.2, "AVL  16", ha="right", va="top", fontsize=13,
                 fontweight="bold", color=AZUL)
    ax_zoom.text(17_950, 19.35, "Rubro-negra  19", ha="right", va="bottom", fontsize=13,
                 fontweight="bold", color=VERMELHO)
    ax_zoom.set_ylim(9.5, 20.5)
    ax_zoom.set_xlim(0, 19_000)
    ax_zoom.set_title("Ampliação das árvores balanceadas", loc="left", fontsize=14,
                      fontweight="bold", pad=16)
    ax_zoom.set_xlabel("Registros inseridos", fontsize=10)
    ax_zoom.set_ylabel("Altura", fontsize=10)
    ax_zoom.xaxis.set_major_formatter(FuncFormatter(inteiro_br))
    ax_zoom.spines[:].set_color(CINZA_CLARO)
    ax_zoom.grid(color=CINZA_CLARO, linewidth=0.7, alpha=0.65)
    ax_zoom.set_axisbelow(True)

    fig.text(0.69, 0.75, "A ampliação evita que 16 e 19 desapareçam visualmente\nna mesma escala da BST.",
             fontsize=10.5, color=CINZA, linespacing=1.45)
    nota(fig, "18.366 registros | 3.061 concursos | mesma sequência de entrada")
    salvar(fig, "01_altura_com_ampliacao.png")


def grafico_retrato_insercao():
    fig = figura()
    cabecalho(
        fig,
        "Construção das árvores",
        "O mesmo conjunto de dados produziu perfis de inserção muito diferentes",
        "comparações, rotações e tempo",
    )

    medias = [mean(TEMPOS_INSERCAO_MS[e]) for e in ESTRUTURAS]
    centros = [0.20, 0.50, 0.80]
    for i, (x, estrutura) in enumerate(zip(centros, ESTRUTURAS)):
        fig.patches.append(Rectangle((x - 0.125, 0.15), 0.25, 0.61,
                                     transform=fig.transFigure, facecolor=PAPEL_CLARO,
                                     edgecolor=CINZA_CLARO, linewidth=1.2))
        fig.patches.append(Rectangle((x - 0.125, 0.70), 0.25, 0.06,
                                     transform=fig.transFigure, facecolor=CORES[estrutura],
                                     edgecolor="none"))
        cor_titulo = PRETO if estrutura == "BST" else PAPEL_CLARO
        fig.text(x, 0.718, estrutura, ha="center", fontsize=16, fontweight="bold",
                 color=cor_titulo)

        fig.text(x, 0.59, inteiro_br(COMPARACOES_INSERCAO[i]), ha="center",
                 fontsize=28, fontweight="bold", color=PRETO)
        fig.text(x, 0.545, "comparações", ha="center", fontsize=11, color=CINZA)

        fig.text(x, 0.42, inteiro_br(ROTACOES[i]), ha="center", fontsize=28,
                 fontweight="bold", color=CORES[estrutura] if ROTACOES[i] else CINZA)
        fig.text(x, 0.375, "rotações", ha="center", fontsize=11, color=CINZA)

        fig.text(x, 0.25, f"{decimal_br(medias[i])} ms", ha="center", fontsize=29,
                 fontweight="bold", color=PRETO)
        fig.text(x, 0.205, "tempo médio", ha="center", fontsize=11, color=CINZA)

    fig.text(0.50, 0.105,
             "A rubro-negra executou mais comparações que a AVL, mas terminou a inserção em menos tempo.",
             ha="center", fontsize=13, color=PRETO, fontweight="bold")
    nota(fig, "Tempos observados em dez execuções por estrutura; métricas de comparação e rotação são determinísticas.")
    salvar(fig, "02_retrato_insercao.png")


def grafico_faixas_tempo():
    fig = figura()
    cabecalho(
        fig,
        "Estabilidade da inserção",
        "As dez medições mostram distância entre as estruturas e variação interna",
        "tempo em milissegundos",
    )
    ax = fig.add_axes([0.11, 0.16, 0.82, 0.61])

    y = np.array([2, 1, 0])
    rng = np.random.default_rng(18)
    for pos, estrutura in zip(y, ESTRUTURAS):
        valores = np.array(TEMPOS_INSERCAO_MS[estrutura])
        ax.hlines(pos, valores.min(), valores.max(), color=CORES[estrutura], linewidth=8,
                  alpha=0.30)
        ax.scatter(valores, pos + rng.uniform(-0.07, 0.07, len(valores)), s=75,
                   color=CORES[estrutura], edgecolor=PRETO, linewidth=0.6, zorder=3)
        media = mean(valores)
        ax.scatter([media], [pos], s=230, marker="D", color=PRETO, zorder=4)
        ax.text(media, pos + 0.25, f"média {decimal_br(media)} ms", ha="center",
                fontsize=13, fontweight="bold", color=PRETO)
        ax.text(valores.max() + 5, pos, f"DP {decimal_br(stdev(valores))}", va="center",
                fontsize=11, color=CINZA)

    ax.set_yticks(y, ESTRUTURAS)
    ax.set_xlim(15, 215)
    ax.set_ylim(-0.55, 2.55)
    ax.set_xlabel("Tempo de inserção (ms)")
    ax.grid(axis="x", color=CINZA_CLARO, linewidth=0.8)
    ax.set_axisbelow(True)
    ax.spines[:].set_visible(False)
    ax.tick_params(length=0)
    fig.text(0.11, 0.105, "● execução individual", color=CINZA, fontsize=10)
    fig.text(0.24, 0.105, "◆ média", color=PRETO, fontsize=10, fontweight="bold")
    nota(fig, "n = 10 por estrutura | o eixo começa em 15 ms para preservar a leitura das faixas")
    salvar(fig, "03_faixas_tempo_insercao.png")


def grafico_busca_causa_efeito():
    fig = figura()
    cabecalho(
        fig,
        "Busca da chave extrema",
        "Menos nós visitados produziram menor tempo de busca",
        "chave (356, 10)",
    )
    ax1 = fig.add_axes([0.08, 0.18, 0.38, 0.56])
    ax2 = fig.add_axes([0.57, 0.18, 0.38, 0.56])
    y = np.array([2, 1, 0])

    medias_tempo = [mean(TEMPOS_BUSCA_US[e]) for e in ESTRUTURAS]
    for ax, valores, rotulo, limite in [
        (ax1, NOS_VISITADOS_BUSCA, "Nós visitados", 450),
        (ax2, medias_tempo, "Tempo médio (µs)", 58),
    ]:
        for pos, estrutura, valor in zip(y, ESTRUTURAS, valores):
            ax.hlines(pos, 0, valor, color=CINZA_CLARO, linewidth=3)
            ax.scatter(valor, pos, s=270, color=CORES[estrutura], edgecolor=PRETO,
                       linewidth=1.1, zorder=3)
            texto = inteiro_br(valor) if rotulo == "Nós visitados" else decimal_br(valor)
            ax.text(valor + limite * 0.035, pos, texto, va="center", fontsize=15,
                    fontweight="bold", color=PRETO)
        ax.set_yticks(y, ESTRUTURAS)
        ax.set_xlim(0, limite)
        ax.set_ylim(-0.6, 2.6)
        ax.set_xlabel(rotulo)
        ax.spines[:].set_visible(False)
        ax.grid(axis="x", color=CINZA_CLARO, linewidth=0.8)
        ax.set_axisbelow(True)
        ax.tick_params(length=0)

    fig.patches.append(Rectangle((0.487, 0.20), 0.006, 0.51, transform=fig.transFigure,
                                 facecolor=AMARELO, edgecolor="none"))
    fig.text(0.50, 0.105,
             "AVL: 26 vezes menos nós visitados e aproximadamente 15,5 vezes menos tempo que a BST",
             ha="center", fontsize=13, fontweight="bold", color=PRETO)
    nota(fig, "Nós visitados são uma métrica determinística; o tempo é a média de dez execuções.")
    salvar(fig, "04_busca_causa_efeito.png")


def grafico_velocidade_operacoes():
    fig = figura()
    cabecalho(
        fig,
        "Ganho relativo",
        "Quanto cada árvore foi mais rápida que a BST nas operações medidas",
        "BST = referência 1,0×",
    )
    ax = fig.add_axes([0.10, 0.18, 0.83, 0.57])

    operacoes = ["Inserção", "Busca extrema", "Remoção extrema"]
    fontes = [TEMPOS_INSERCAO_MS, TEMPOS_BUSCA_US, TEMPOS_REMOCAO_US]
    y = np.arange(len(operacoes))[::-1]

    for pos, operacao, dados in zip(y, operacoes, fontes):
        media_bst = mean(dados["BST"])
        ganhos = {
            "BST": 1.0,
            "AVL": media_bst / mean(dados["AVL"]),
            "Rubro-negra": media_bst / mean(dados["Rubro-negra"]),
        }
        ax.hlines(pos, 1, max(ganhos.values()), color=CINZA_CLARO, linewidth=3)
        deslocamentos = {"BST": -0.12, "AVL": 0.0, "Rubro-negra": 0.12}
        for estrutura in ESTRUTURAS:
            valor = ganhos[estrutura]
            ax.scatter(valor, pos + deslocamentos[estrutura], s=210,
                       color=CORES[estrutura], edgecolor=PRETO, linewidth=0.9, zorder=3)
            ax.text(valor, pos + deslocamentos[estrutura] + 0.18,
                    f"{decimal_br(valor, 1)}×", ha="center", fontsize=11,
                    fontweight="bold", color=PRETO)

    ax.set_yticks(y, operacoes)
    ax.set_xlim(0, 17)
    ax.set_xlabel("Velocidade relativa em comparação à BST")
    ax.grid(axis="x", color=CINZA_CLARO, linewidth=0.8)
    ax.set_axisbelow(True)
    ax.spines[:].set_visible(False)
    ax.tick_params(length=0)

    handles = [plt.Line2D([0], [0], marker="o", color="none", markerfacecolor=CORES[e],
                          markeredgecolor=PRETO, markersize=10, label=e) for e in ESTRUTURAS]
    ax.legend(handles=handles, ncol=3, loc="lower right", frameon=False)
    nota(fig, "Razões calculadas a partir das médias de dez execuções; operações usam unidades diferentes, mas razões são comparáveis.")
    salvar(fig, "05_ganho_relativo.png")


def grafico_percursos_conectados():
    fig = figura()
    cabecalho(
        fig,
        "Percursos completos",
        "As três estruturas permaneceram próximas porque todos os nós precisam ser visitados",
        "médias de dez execuções",
    )
    ax = fig.add_axes([0.12, 0.18, 0.80, 0.57])

    percursos = ["Pré-ordem", "Em ordem", "Pós-ordem"]
    fontes = [TEMPOS_PRE_ORDEM_MS, TEMPOS_EM_ORDEM_MS, TEMPOS_POS_ORDEM_MS]
    y = np.arange(3)[::-1]

    for pos, percurso, dados in zip(y, percursos, fontes):
        valores = [mean(dados[e]) for e in ESTRUTURAS]
        ax.hlines(pos, min(valores), max(valores), color=CINZA_CLARO, linewidth=4)
        for estrutura, valor in zip(ESTRUTURAS, valores):
            ax.scatter(valor, pos, s=230, color=CORES[estrutura], edgecolor=PRETO,
                       linewidth=1.0, zorder=3)
            deslocamento = 0.16 if estrutura != "AVL" else -0.22
            ax.text(valor, pos + deslocamento, decimal_br(valor, 3), ha="center",
                    fontsize=10.5, fontweight="bold", color=PRETO)

    ax.set_yticks(y, percursos)
    ax.set_xlim(1.70, 2.45)
    ax.set_xlabel("Tempo médio (ms)")
    ax.grid(axis="x", color=CINZA_CLARO, linewidth=0.8)
    ax.set_axisbelow(True)
    ax.spines[:].set_visible(False)
    ax.tick_params(length=0)
    handles = [plt.Line2D([0], [0], marker="o", color="none", markerfacecolor=CORES[e],
                          markeredgecolor=PRETO, markersize=10, label=e) for e in ESTRUTURAS]
    ax.legend(handles=handles, ncol=3, loc="lower right", frameon=False)
    fig.text(0.12, 0.105,
             "Escala ampliada para mostrar diferenças pequenas; o intervalo completo permanece inferior a 1 ms.",
             fontsize=11, color=CINZA)
    nota(fig, "Cada percurso processa os mesmos 18.366 registros; use este gráfico como controle experimental.")
    salvar(fig, "06_percursos_conectados.png")


def grafico_fechamento():
    fig = figura()
    cabecalho(
        fig,
        "Síntese",
        "O balanceamento mudou a forma da árvore e o custo das operações de caminho",
        "resultados observados",
    )

    itens = [
        ("ALTURA", "416", "16", "BST", "AVL", "26× menor"),
        ("BUSCA", "832", "32", "BST", "AVL", "26× menos comparações"),
        ("REMOÇÃO", "67,48 µs", "7,30 µs", "BST", "Rubro-negra", "9,2× mais rápida"),
    ]
    y_positions = [0.67, 0.45, 0.23]
    for y, (rotulo, antes, depois, origem, destino, conclusao) in zip(y_positions, itens):
        fig.text(0.07, y, rotulo, fontsize=11, fontweight="bold", color=CINZA)
        fig.text(0.22, y, antes, fontsize=31, fontweight="bold", color=CORES[origem], va="center")
        fig.patches.append(Rectangle((0.39, y - 0.011), 0.17, 0.022, transform=fig.transFigure,
                                     facecolor=CINZA_CLARO, edgecolor="none"))
        fig.patches.append(Rectangle((0.53, y - 0.018), 0.028, 0.036, transform=fig.transFigure,
                                     facecolor=AMARELO, edgecolor="none"))
        fig.text(0.63, y, depois, fontsize=31, fontweight="bold", color=CORES[destino], va="center")
        fig.text(0.82, y, conclusao, fontsize=13, fontweight="bold", color=PRETO, va="center")

    fig.text(0.50, 0.105,
             "A altura explica o caminho. O caminho explica o custo de busca e remoção.",
             ha="center", fontsize=17, fontweight="bold", color=PRETO)
    nota(fig, "Síntese da sequência histórica e da implementação avaliadas.")
    salvar(fig, "07_sintese_editorial.png")


def main():
    configurar()
    grafico_altura_com_ampliacao()
    grafico_retrato_insercao()
    grafico_faixas_tempo()
    grafico_busca_causa_efeito()
    grafico_velocidade_operacoes()
    grafico_percursos_conectados()
    grafico_fechamento()
    print(f"Nova proposta gerada em: {SAIDA}")


if __name__ == "__main__":
    main()
