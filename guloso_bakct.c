#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para o algoritmo guloso
typedef struct {
    int valor;
    int pos;
} Elemento;

// Variavel global para medir a profundidade da pilha no Backtracking
int profundidade_maxima = 0;

// Funcao para encontrar o maior entre dois numeros
long long obter_maximo(long long a, long long b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Comparador para a ordenacao do Guloso (qsort)
int comparar_elementos(const void *a, const void *b) {
    Elemento *itemA = (Elemento *)a;
    Elemento *itemB = (Elemento *)b;
    if (itemB->valor > itemA->valor) return 1;
    if (itemB->valor < itemA->valor) return -1;
    return 0;
}

// --- 1. ABORDAGEM BACKTRACKING (Busca a Solucao Otima) ---
long long algoritmo_backtracking(int arr[], int n, int i, int profundidade_atual) {
    if (profundidade_atual > profundidade_maxima) {
        profundidade_maxima = profundidade_atual;
    }
    if (i >= n) return 0;

    long long incluir = arr[i] + algoritmo_backtracking(arr, n, i + 2, profundidade_atual + 1);
    long long excluir = algoritmo_backtracking(arr, n, i + 1, profundidade_atual + 1);

    return obter_maximo(incluir, excluir);
}

// --- 2. ABORDAGEM GULOSA (Solucao Aproximada) ---
long long algoritmo_guloso(int arr[], int n, size_t *memoria_heap) {
    size_t tamanho_elementos = n * sizeof(Elemento);
    size_t tamanho_bloqueio = n * sizeof(int);
    *memoria_heap = tamanho_elementos + tamanho_bloqueio;

    Elemento *elementos = malloc(tamanho_elementos);
    int *bloqueado = calloc(n, sizeof(int));
    long long soma_total = 0;

    for (int i = 0; i < n; i++) {
        elementos[i].valor = arr[i];
        elementos[i].pos = i;
    }

    qsort(elementos, n, sizeof(Elemento), comparar_elementos);

    for (int i = 0; i < n; i++) {
        int p = elementos[i].pos;
        if (bloqueado[p] == 0) {
            soma_total = soma_total + elementos[i].valor;
            bloqueado[p] = 1;
            if (p > 0) bloqueado[p - 1] = 1;
            if (p < n - 1) bloqueado[p + 1] = 1;
        }
    }

    free(elementos);
    free(bloqueado);
    return soma_total;
}

// --- FUNCAO UNICA DE EXPERIMENTO ---
void executar_experimento(int n) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;

    printf("\n--- TESTE PARA N = %d ---\n", n);
    printf("%-15s | %-12s | %-12s | %-15s\n", "METODO", "SOMA", "TEMPO (s)", "MEMORIA");
    printf("--------------------------------------------------------------------------\n");

    // Executa SEMPRE o Guloso
    size_t mem_g = 0;
    clock_t ini_g = clock();
    long long res_g = algoritmo_guloso(arr, n, &mem_g);
    double tempo_g = (double)(clock() - ini_g) / CLOCKS_PER_SEC;
    printf("%-15s | %-12lld | %-12.6f | %zu bytes (Heap)\n", "Guloso", res_g, tempo_g, mem_g);

    // Executa o Backtracking APENAS se N for pequeno (evita travar o PC)
    if (n <= 35) {
        profundidade_maxima = 0;
        clock_t ini_b = clock();
        long long res_b = algoritmo_backtracking(arr, n, 0, 1);
        double tempo_b = (double)(clock() - ini_b) / CLOCKS_PER_SEC;
        long mem_b = profundidade_maxima * 64; 
        printf("%-15s | %-12lld | %-12.6f | %ld bytes (Stack)\n", "Backtracking", res_b, tempo_b, mem_b);
    } else {
        printf("%-15s | %-12s | %-12s | %-15s\n", "Backtracking", "Ignorado", "Muito Lento", "---");
    }

    free(arr);
}

int main() {
    srand(time(NULL));
    
    printf("ESTUDO DE CASO: COMPLEXIDADE EXPONENCIAL VS POLINOMIAL\n");

    // Testes de pequena/media escala (Comparativos)
    executar_experimento(15);
    executar_experimento(25);
    executar_experimento(35);

    // Testes de grande escala (Apenas Guloso)
    executar_experimento(10000);
    executar_experimento(20000);
    executar_experimento(40000);

    return 0;
}