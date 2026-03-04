#include "../include/tensor.h"

int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        fprintf(stderr, "Erro: Argumentos insuficientes.\n");
        fprintf(stderr, "Uso: %s [matriz1] [matriz2] ... [matrizN]\n", argv[0]);
        return 1;
    }

    int total_arquivos = argc - 1;
    
    Matriz **matrizes_lidas = (Matriz**) malloc(total_arquivos * sizeof(Matriz*));
    if (!matrizes_lidas) {
        fprintf(stderr, "Erro de alocacao de memoria inicial.\n");
        return 1;
    }

    // Leitura dos Arquivos
    for (int i = 0; i < total_arquivos; i++) {
        // argv[i+1] pq argv[0] é o nome do programa
        matrizes_lidas[i] = ler_matriz(argv[i + 1]);
        
        if (!matrizes_lidas[i]) {
            fprintf(stderr, "Falha ao ler arquivo '%s'. Abortando.\n", argv[i + 1]);
            
            // libera o que já foi lido antes de sair
            for (int j = 0; j < i; j++) {
                liberar_matriz(matrizes_lidas[j]);
            }
            free(matrizes_lidas);
            return 1; 
        }
        printf("Arquivo %d ('%s') carregado: %dx%d\n", i+1, argv[i+1], matrizes_lidas[i]->linhas, matrizes_lidas[i]->colunas);
    }

    //calculando o produto das duas primeiras: A (x) B
    Matriz *resultado_acumulado = produto_tensorial(matrizes_lidas[0], matrizes_lidas[1]);
    
    if (!resultado_acumulado) {
        fprintf(stderr, "Erro ao calcular o primeiro produto tensorial.\n");
        // Limpeza completa
        for(int i = 0; i < total_arquivos; i++) liberar_matriz(matrizes_lidas[i]);
        free(matrizes_lidas);
        return 1;
    }

    // mais de 2 matrizes, continua
    for (int i = 2; i < total_arquivos; i++) {
        printf("Multiplicando resultado parcial pela matriz %d...\n", i+1);
        
        Matriz *temp = resultado_acumulado; // Guarda o ponteiro antigo para liberar depois
        resultado_acumulado = produto_tensorial(temp, matrizes_lidas[i]);
        
        liberar_matriz(temp); 

        if (!resultado_acumulado) {
            fprintf(stderr, "Erro critico de memoria no passo %d.\n", i+1);
            break;
        }
    }

    // resultado final 
    if (resultado_acumulado) {
        printf("\n=== RESULTADO FINAL ===\n");
        

        imprimir_matriz(resultado_acumulado);
        
        const char *arquivo_saida = "tensor_" LOGIN ".out";
        
        // Chama a função para escrever no arquivo
        salvar_matriz_arquivo(resultado_acumulado, arquivo_saida);
        
        liberar_matriz(resultado_acumulado);
    }

    // liberar as matrizes lidas
    for (int i = 0; i < total_arquivos; i++) {
        liberar_matriz(matrizes_lidas[i]);
    }
    free(matrizes_lidas);

    printf("Memoria liberada. Fim do programa.\n");
    return 0;
}