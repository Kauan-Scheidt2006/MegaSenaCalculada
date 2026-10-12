# MegaSenaCalculada

Programa acadêmico de visualização e comparação de estruturas de dados usando o histórico de concursos (sorteios) da Mega-Sena. O projeto foi desenvolvido para a disciplina de Estrutura de Dados do IFC — Instituto Federal Catarinense, campus São Bento do Sul.

Ao ser iniciado, o programa insere os concursos em árvores binária de busca, AVL e rubro-negra e permite observar sua organização, seus percursos e métricas de navegação. A intenção é tornar visíveis conceitos que costumam ser estudados apenas de forma abstrata e permitir a comparação do comportamento das estruturas com um conjunto de dados real.

O histórico é usado exclusivamente para fins didáticos e para análises descritivas. Frequências e padrões passados **não permitem prever resultados futuros da Mega-Sena**.

## Tecnologias e versões de referência

| Tecnologia | Versão | Finalidade |
|---|---:|---|
| C++ | C++20 | aplicação, regras e implementação de conceitos estudados na disciplina|
| Qt | 6.11.1, compilador MinGW 64-bit | interface gráfica e leitura de JSON |
| MinGW/GCC | 13.1.0, 64-bit | compilação no Windows |
| CMake | 3.30.5 (mínimo aceito: 3.21) | configuração da compilação |
| Ninja | 1.12.1 | execução da compilação |
| Python | 3.11.9 | conversão da planilha em JSON e gráficos auxiliares |

As versões acima são as usadas no ambiente de desenvolvimento e são a configuração recomendada. As bibliotecas Python têm suas versões fixadas em [`requirements.txt`](requirements.txt).

## Pré-requisitos

O roteiro principal abaixo considera Windows 10 ou 11 em 64 bits.

1. Instale o [Git](https://git-scm.com/download/win).
2. Instale o [Python 3.11.9](https://www.Python.org/downloads/release/Python-3119/) e marque a opção para adicioná-lo ao `PATH`.
3. Instale o [Qt](https://www.qt.io/download-qt-installer-oss) pelo instalador oficial. No Qt Maintenance Tool, selecione:
   - Qt 6.11.1 → MinGW 13.1.0 64-bit;
   - Developer and Designer Tools → CMake 3.30.5 ou mais recente;
   - Developer and Designer Tools → Ninja 1.12.1 ou mais recente;
   - Developer and Designer Tools → MinGW 13.1.0 64-bit.

O projeto está configurado para a instalação padrão do Qt em `C:\Qt`. Se o Qt estiver em outro local, ajuste `CMAKE_PREFIX_PATH` no início do arquivo [`CMakeLists.txt`](CMakeLists.txt).

## Baixar o projeto

Abra o PowerShell e execute:

```powershell
git clone https://github.com/Kauan-Scheidt2006/MegaSenaCalculada.git
```

## Preparar o Python

O Python não é necessário para abrir a aplicação quando `concursos.json` já está presente. Ele é necessário para recriar esse arquivo a partir de uma planilha `.xlsx` e para executar os scripts de gráficos.
Para gerar o JSON com o script Python, execute o arquivo [gerar_concursos_json.py]
```powershell
Python gerar_concursos_json.py --entrada "C:\caminho\para\Mega-Sena.xlsx" --saida concursos.json
```

A planilha deve conter a aba `MEGA SENA` e as colunas `Concurso`, `Data do Sorteio` e `Bola1` a `Bola6`.

## Observação
Os comandos abaixo são para fazer o build od pprojeto e gerar o executável
Esse executável carregará os dados vindos do JSON, criará as árvores e fará a apresentação das mesmas visualmente.
## Compilar no Windows

Em um novo PowerShell, dentro da raiz do projeto, disponibilize as ferramentas do Qt na sessão:

```powershell
$env:Path = "C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.11.1\mingw_64\bin;$env:Path"
```

Configure e compile em uma pasta separada:

```powershell
cmake -S . -B build-local -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-local
```

Se uma versão diferente do compilador for encontrada primeiro no `PATH`, remova `build-local`, corrija o `PATH` e configure novamente. Não misture bibliotecas Qt para MinGW com MSVC ou com outra versão do MinGW.

## Executar

Mantenha `concursos.json` na raiz do projeto e execute a aplicação a partir dessa pasta:

```powershell
.\build-local\MegaSenaCalculada.exe
```

Executar pela raiz permite que o programa encontre o histórico. A aplicação também procura o JSON ao lado do executável e na pasta imediatamente acima dele.

Se o Windows informar que uma DLL do Qt está ausente, confirme que `C:\Qt\6.11.1\mingw_64\bin` permanece no `PATH` da sessão ou execute o programa pelo Qt Creator com o kit MinGW 13.1.0 64-bit.


## Estrutura principal

```text
MegaSenaCalculada/
├── src/                    código-fonte C++
│   ├── aplicacao/          casos de uso e coordenação
│   ├── dominio/            modelos do domínio
│   ├── estruturas/         BST, AVL e árvore rubro-negra
│   ├── infraestrutura/     leitura do histórico em JSON
│   ├── interface/          interface gráfica Qt
│   └── processamento/      processamento histórico
├── recursos/               estilos e recursos da interface
├── docs/                   decisões e documentação do projeto
├── concursos.json          dados consumidos pela aplicação
├── gerar_concursos_json.py conversor de XLSX para JSON
├── CMakeLists.txt          configuração de compilação
└── requirements.txt        dependências Python
```

## Solução de problemas

- **`cmake` ou `ninja` não é reconhecido:** revise o comando que adiciona as ferramentas do Qt ao `PATH`.
- **Qt6 não encontrado:** confirme a instalação de `C:\Qt\6.11.1\mingw_64` e o valor de `CMAKE_PREFIX_PATH` no `CMakeLists.txt`.
- **Compilador incompatível:** use o MinGW instalado junto com a mesma versão do Qt.
- **Histórico não encontrado:** execute a partir da raiz do projeto e confirme a presença de `concursos.json`.
- **Módulo Python ausente:** ative `.venv` e repita `Python -m pip install -r requirements.txt`.

## Licença

Consulte o arquivo [`LICENSE`](LICENSE).
