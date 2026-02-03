#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para o algoritmo guloso (Tema 05) 
typedef struct {
    int valor;
    int pos;
} Elemento;

// Variáveis globais para métricas de desempenho
int profundidade_maxima = 0;
long long total_chamadas = 0; 

long long obter_maximo(long long a, long long b) {
    return (a > b) ? a : b;
}

int comparar_elementos(const void *a, const void *b) {
    return ((Elemento *)b)->valor - ((Elemento *)a)->valor;
}

// 1. ABORDAGEM BACKTRACKING (Busca a Solução Ótima) 
long long algoritmo_backtracking(int arr[], int n, int i, int prof) {
    total_chamadas++; // Contagem de operações para o Cenário 3 
    if (prof > profundidade_maxima) profundidade_maxima = prof;
    if (i >= n) return 0;

    // Decisão exaustiva: incluir ou excluir o elemento atual
    long long incluir = arr[i] + algoritmo_backtracking(arr, n, i + 2, prof + 1);
    long long excluir = algoritmo_backtracking(arr, n, i + 1, prof + 1);

    return obter_maximo(incluir, excluir);
}

// 2. ABORDAGEM GULOSA (Solução Aproximada) 
long long algoritmo_guloso(int arr[], int n, size_t *mem_heap, long long *ops) {
    size_t tam_elem = n * sizeof(Elemento);
    size_t tam_bloq = n * sizeof(int);
    *mem_heap = tam_elem + tam_bloq; // Medição de memória (Heap) 

    Elemento *elementos = malloc(tam_elem);
    int *bloqueado = calloc(n, sizeof(int));
    long long soma = 0;

    for (int i = 0; i < n; i++) {
        (*ops)++;
        elementos[i].valor = arr[i];
        elementos[i].pos = i;
    }

    qsort(elementos, n, sizeof(Elemento), comparar_elementos);

    for (int i = 0; i < n; i++) {
        (*ops)++;
        int p = elementos[i].pos;
        if (bloqueado[p] == 0) {
            soma += elementos[i].valor;
            bloqueado[p] = 1;
            if (p > 0) bloqueado[p - 1] = 1;
            if (p < n - 1) bloqueado[p + 1] = 1;
        }
    }

    free(elementos);
    free(bloqueado);
    return soma;
}

void realizar_teste(int n, int eh_armadilha) {
    int *arr = malloc(n * sizeof(int));
    if (eh_armadilha) {
        // CENÁRIO 2: Onde o Guloso falha (Qualidade) 
        int armadilha[] = {50, 100, 80, 20}; 
        for(int i = 0; i < 4; i++) arr[i] = armadilha[i];
        n = 4;
    } else {
        for(int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    }

    // Medição Backtracking
    total_chamadas = 0;
    profundidade_maxima = 0;
    clock_t ini_b = clock();
    long long res_b = algoritmo_backtracking(arr, n, 0, 1);
    double tempo_b = (double)(clock() - ini_b) / CLOCKS_PER_SEC; // Medição de tempo [cite: 36]
    long mem_b = profundidade_maxima * 64; // Memória (Stack) [cite: 37]

    // Medição Guloso
    size_t mem_g = 0;
    long long ops_g = 0;
    clock_t ini_g = clock();
    long long res_g = algoritmo_guloso(arr, n, &mem_g, &ops_g);
    double tempo_g = (double)(clock() - ini_g) / CLOCKS_PER_SEC; // Medição de tempo [cite: 36]

    // IMPRESSÃO DA TABELA COMPLETA (Atende os requisitos de apresentação) [cite: 49]
    printf("\n--- RESULTADOS PARA N = %d ---\n", n);
    printf("%-12s | %-6s | %-12s | %-10s | %-10s\n", "METODO", "SOMA", "OPERACOES", "TEMPO (s)", "MEMORIA");
    printf("----------------------------------------------------------------------\n");
    printf("%-12s | %-6lld | %-12lld | %-10.6f | %ld Bytes\n", "Backtracking", res_b, total_chamadas, tempo_b, mem_b);
    printf("%-12s | %-6lld | %-12lld | %-10.6f | %zu Bytes\n", "Guloso", res_g, ops_g, tempo_g, mem_g);
    
    if (res_b > res_g) {
        printf("\n[AVISO]: Diferenca de qualidade detectada! Guloso perdeu por %lld.\n", res_b - res_g);
    }

    free(arr);
}

int main() {
    srand(time(NULL));
    printf("PROJETO E ANALISE DE ALGORITMOS\n");
    
    printf("\nDEMONSTRACAO: CENARIO 1 (QUALIDADE DA SOLUCAO) ");
    realizar_teste(4, 1);

    printf("\nDEMONSTRACAO: CENARIO 2 (TEMPO E ESCALABILIDADE)");
    realizar_teste(32, 0); // N=32 já evidencia bem a lentidão do Backtracking

    return 0;
}