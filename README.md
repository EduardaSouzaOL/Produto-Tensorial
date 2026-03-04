# 🧮 Produto Tensorial em C

Desenvolvimento de um programa em C capaz de processar e calcular o Produto Tensorial de múltiplas matrizes passadas por arquivos de texto. O projeto destaca a manipulação avançada de ponteiros e o gerenciamento eficiente de alocação dinâmica de memória frente ao crescimento exponencial de dados, com garantia de zero vazamentos (memory leaks) atestada pela ferramenta Valgrind


O projeto foi estruturado seguindo boas práticas de organização com separação entre:

- `include/` → arquivos de cabeçalho (.h)
- `src/` → arquivos fonte (.c)
- `obj/` → arquivos objeto gerados automaticamente
- `Makefile` → automação de compilação

## ⚙️ Requisitos

- GCC
- Make
- Valgrind (opcional, para análise de memória)

### 🐧 Linux / WSL (Ubuntu)

```bash
sudo apt update
sudo apt install build-essential valgrind
```


### Como reproduzir (compilar, executar, testar e validar)

```bash
make
./tensor matriz.txt matriz2.txt
make valgrind ARGS="matriz.txt matriz2.txt"
```

Este projeto foi desenvolvido e testado em ambiente **Linux (WSL)**, utilizando o compilador GCC e a ferramenta Valgrind para análise de memória.

**Compilação**
Para compilar o projeto e gerar o executável `tensor`, abra o terminal na raiz do projeto e execute:
`make`
O `Makefile` cuidará de compilar os códigos-fonte e gerar os arquivos objeto no diretório `obj/`, além do binário final.

**Execução**
O programa aceita **N arquivos** de texto como argumento via linha de comando. Cada arquivo deve conter as dimensões da matriz na primeira linha, seguidas pelos seus elementos.
`./tensor matriz.txt matriz2.txt [matriz3.txt ...]`

**Teste e Validação de Memória**
A ausência de vazamentos de memória (*memory leaks*) é um requisito estrito desta entrega. Para rodar a verificação automatizada com o **Valgrind**, utilize a regra customizada do Makefile passando os arquivos na variável `ARGS`:
`make valgrind ARGS="matriz.txt matriz2.txt"`

**Critério de Sucesso:** O programa é considerado validado se o relatório final do Valgrind exibir:
* `O tensor_meso.out na raiz do projeto`
* `All heap blocks were freed -- no leaks are possible`
* `ERROR SUMMARY: 0 errors from 0 contexts`
