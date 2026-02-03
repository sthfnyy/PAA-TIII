#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para o algoritmo guloso
typedef struct {
    int valor;
    int pos;
} Elemento;

// Variáveis globais para métricas de complexidade
int profundidade_maxima = 0;
long long total_chamadas = 0; // CENÁRIO 3: Contador de operações (recursões)

long long obter_maximo(long long a, long long b) {
    if (a > b) return a;
    return b;
}

int comparar_elementos(const void *a, const void *b) {
    Elemento *itemA = (Elemento *)a;
    Elemento *itemB = (Elemento *)b;
    return itemB->valor - itemA->valor;
}

// --- 1. ABORDAGEM BACKTRACKING ---
long long algoritmo_backtracking(int arr[], int n, int i, int profundidade_atual) {
    total_chamadas++; // CENÁRIO 3: Registra cada "operação" ou decisão tomada

    if (profundidade_atual > profundidade_maxima) {
        profundidade_maxima = profundidade_atual;
    }

    if (i >= n) return 0;

    // A exaustividade garante a qualidade (CENÁRIO 2)
    long long incluir = arr[i] + algoritmo_backtracking(arr, n, i + 2, profundidade_atual + 1);
    long long excluir = algoritmo_backtracking(arr, n, i + 1, profundidade_atual + 1);

    return obter_maximo(incluir, excluir);
}

// --- 2. ABORDAGEM GULOSA ---
long long algoritmo_guloso(int arr[], int n, size_t *memoria_heap, long long *ops_guloso) {
    size_t tamanho_elementos = n * sizeof(Elemento);
    size_t tamanho_bloqueio = n * sizeof(int);
    *memoria_heap = tamanho_elementos + tamanho_bloqueio;

    Elemento *elementos = malloc(tamanho_elementos);
    int *bloqueado = calloc(n, sizeof(int));
    long long soma_total = 0;

    for (int i = 0; i < n; i++) {
        (*ops_guloso)++; // Contando iterações simples
        elementos[i].valor = arr[i];
        elementos[i].pos = i;
    }

    // Ordenação (parte vital do custo do guloso)
    qsort(elementos, n, sizeof(Elemento), comparar_elementos);

    for (int i = 0; i < n; i++) {
        (*ops_guloso)++; // Contando a lógica de decisão
        int p = elementos[i].pos;
        if (bloqueado[p] == 0) {
            soma_total += elementos[i].valor;
            bloqueado[p] = 1;
            if (p > 0) bloqueado[p - 1] = 1;
            if (p < n - 1) bloqueado[p + 1] = 1;
        }
    }

    free(elementos);
    free(bloqueado);
    return soma_total;
}

void realizar_teste(int n, int eh_cenario_armadilha) {
    int *arr = malloc(n * sizeof(int));
    
    if (eh_cenario_armadilha == 1) {
        // CENÁRIO 2: Aqui forçamos o array [50, 100, 80, 20]
        // O guloso pegará 100, o Backtracking pegará 50+80=130.
        int armadilha[] = {50, 100, 80, 20};
        for(int i = 0; i < 4; i++) arr[i] = armadilha[i];
        n = 4;
    } else {
        for(int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    }

    printf("\n--- Teste com N = %d ---\n", n);

    // Medição Backtracking
    total_chamadas = 0;
    profundidade_maxima = 0;
    clock_t inicio_b = clock();
    long long res_b = algoritmo_backtracking(arr, n, 0, 1);
    double tempo_b = (double)(clock() - inicio_b) / CLOCKS_PER_SEC;

    // Medição Guloso
    size_t memoria_heap = 0;
    long long ops_guloso = 0;
    clock_t inicio_g = clock();
    long long res_g = algoritmo_guloso(arr, n, &memoria_heap, &ops_guloso);
    double tempo_g = (double)(clock() - inicio_g) / CLOCKS_PER_SEC;

    printf("METODO       | SOMA       | OPERACOES (Cenario 3) | TEMPO (s)\n");
    printf("Backtracking | %-10lld | %-20lld | %-10.6f\n", res_b, total_chamadas, tempo_b);
    printf("Guloso       | %-10lld | %-20lld | %-10.6f\n", res_g, ops_guloso, tempo_g);
    
    if (res_b > res_g) {
        printf(">>> AVISO: Qualidade Inferior no Guloso (Cenario 2) detectada!\n");
    }

    free(arr);
}

int main() {
    srand(time(NULL));
    
    printf("ESTUDO: GULOSO (APROXIMADO) VS BACKTRACKING (OTIMO)\n");

    // CENÁRIO 2: Demonstrar que o Guloso nem sempre é o melhor
    printf("\n[CENARIO 2: ARMADILHA DE QUALIDADE]");
    realizar_teste(4, 1);

    // CENÁRIO 3: Demonstrar a explosão de operações (N=30 gera milhões de chamadas)
    printf("\n[CENARIO 3: CONTAGEM DE OPERACOES E ESCALABILIDADE]");
    realizar_teste(30, 0); 

    printf("\n[DADOS MASSIVOS: O LIMITANTE DO BACKTRACKING]");
    printf("\nPara N=100.000, o Backtracking faria aprox. 2^100.000 operacoes.");
    printf("\nO Guloso resolvera em milissegundos...\n");

    return 0;
}