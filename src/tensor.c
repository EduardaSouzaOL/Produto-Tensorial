#include "../include/tensor.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



Matriz* criar_matriz(int linhas, int colunas) {
    Matriz *m = (Matriz*) malloc(sizeof(Matriz));
    if (m == NULL) return NULL;

    m->linhas = linhas;
    m->colunas = colunas;


    m->dados = (int**) malloc(linhas * sizeof(int*));
    if (m->dados == NULL) {
        free(m);
        return NULL;
    }

    // Aloca cada linha
    for (int i = 0; i < linhas; i++) {
        m->dados[i] = (int*) malloc(colunas * sizeof(int));
        if (m->dados[i] == NULL) {
            // Se falhar, libera o que já foi alocado para evitar vazamento
            for (int j = 0; j < i; j++) free(m->dados[j]);
            free(m->dados);
            free(m);
            return NULL;
        }
    }
    return m;
}

void liberar_matriz(Matriz *m) {
    if (m != NULL) {
        if (m->dados != NULL) {
            for (int i = 0; i < m->linhas; i++) {
                if (m->dados[i] != NULL) {
                    free(m->dados[i]);
                }
            }
            free(m->dados);
        }
        free(m);
    }
}



void imprimir_matriz(Matriz *m) {
    if (!m) return;
    printf("Matriz %dx%d:\n", m->linhas, m->colunas);
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            printf("%d ", m->dados[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

Matriz* ler_matriz(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Nao foi possivel abrir '%s'.\n", nome_arquivo);
        return NULL;
    }

    int linhas = 0;
    int colunas = -1; // -1 indica que as colunas ainda não foram contadas
    char buffer[TAMANHO_MAX_LINHA];
    
    // conta dimensões e valida consistência estrutural
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        int colunas_nesta_linha = 0;
        char *token = strtok(buffer, " \t\n");
        
        while (token != NULL) {
            colunas_nesta_linha++;
            token = strtok(NULL, " \t\n");
        }

        //se a linha realmente tiver números
        if (colunas_nesta_linha > 0) {
            if (colunas == -1) {
                colunas = colunas_nesta_linha; // Define o padrão baseado na 1ª linha
            } else if (colunas != colunas_nesta_linha) {
                // Falha caso uma linha tenha um número diferente de colunas
                fprintf(stderr, "Erro: Matriz no arquivo '%s' esta inconsistente.\n", nome_arquivo);
                fclose(arquivo);
                return NULL;
            }
            linhas++;
        }
    }

    if (linhas == 0 || colunas <= 0) {
        fprintf(stderr, "Erro: Arquivo '%s' vazio ou invalido.\n", nome_arquivo);
        fclose(arquivo);
        return NULL;
    }

    //  Alocar e Preencher
    rewind(arquivo); 
    Matriz *m = criar_matriz(linhas, colunas);
    if (!m) {
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (fscanf(arquivo, "%d", &m->dados[i][j]) != 1) {
                fprintf(stderr, "Erro de leitura na linha %d, coluna %d do arquivo '%s'.\n", i, j, nome_arquivo);
                liberar_matriz(m);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return m;
}


Matriz* produto_tensorial(Matriz *A, Matriz *B) {
    if (!A || !B) return NULL;

    int novas_linhas = A->linhas * B->linhas;
    int novas_colunas = A->colunas * B->colunas;

    Matriz *C = criar_matriz(novas_linhas, novas_colunas);
    if (!C) return NULL;

    for (int i = 0; i < A->linhas; i++) {
        for (int j = 0; j < A->colunas; j++) {
            
            for (int k = 0; k < B->linhas; k++) {
                for (int l = 0; l < B->colunas; l++) {
                    
                    int linha_res = (i * B->linhas) + k;
                    int col_res   = (j * B->colunas) + l;
                    
                    C->dados[linha_res][col_res] = A->dados[i][j] * B->dados[k][l];
                }
            }
        }
    }
    return C;
}

void salvar_matriz_arquivo(Matriz *m, const char *nome_arquivo) {
    if (!m) return;
    
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        fprintf(stderr, "Erro: Nao foi possivel criar o arquivo de saida '%s'.\n", nome_arquivo);
        return;
    }

    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            fprintf(arquivo, "%d ", m->dados[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}