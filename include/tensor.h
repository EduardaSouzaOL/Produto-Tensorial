#ifndef TENSOR_H
#define TENSOR_H

#include <stdio.h>
#include <stdlib.h>

#define LOGIN "meso"
#define TAMANHO_MAX_LINHA 4096

typedef struct {
    int linhas;
    int colunas;
    int **dados;
} Matriz;

Matriz* criar_matriz(int linhas, int colunas);
Matriz* ler_matriz(const char *nome_arquivo);
void liberar_matriz(Matriz *m);
void imprimir_matriz(Matriz *m);
Matriz* produto_tensorial(Matriz *A, Matriz *B);
void salvar_matriz_arquivo(Matriz *m, const char *nome_arquivo);

#endif