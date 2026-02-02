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

// Funcao para encontrar o maior entre dois numeros (sem operador ternario)
long long obter_maximo(long long a, long long b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Comparador para a ordenacao do Guloso
int comparar_elementos(const void *a, const void *b) {
    Elemento *itemA = (Elemento *)a;
    Elemento *itemB = (Elemento *)b;
    return itemB->valor - itemA->valor;
}

// --- 1. ABORDAGEM BACKTRACKING (Busca a Solucao Otima) ---
// Mede a memoria via profundidade da recursao (Pilha/Stack)
long long algoritmo_backtracking(int arr[], int n, int i, int profundidade_atual) {
    // Rastreio de memoria da pilha
    if (profundidade_atual > profundidade_maxima) {
        profundidade_maxima = profundidade_atual;
    }

    // Caso base: se chegar ao fim do array
    if (i >= n) {
        return 0;
    }

    // Escolha 1: Incluir o numero atual (pula o vizinho proximo)
    long long incluir = arr[i] + algoritmo_backtracking(arr, n, i + 2, profundidade_atual + 1);

    // Escolha 2: Ignorar o numero atual (vai para o proximo)
    long long excluir = algoritmo_backtracking(arr, n, i + 1, profundidade_atual + 1);

    // Retorna a melhor escolha (solucao otima)
    return obter_maximo(incluir, excluir);
}

// --- 2. ABORDAGEM GULOSA (Solucao Aproximada) ---
// Mede a memoria via alocacao dinamica (Heap)
long long algoritmo_guloso(int arr[], int n, size_t *memoria_heap) {
    size_t tamanho_elementos = n * sizeof(Elemento);
    size_t tamanho_bloqueio = n * sizeof(int);
    
    // Registra o consumo de memoria exigido pelo trabalho
    *memoria_heap = tamanho_elementos + tamanho_bloqueio;

    Elemento *elementos = malloc(tamanho_elementos);
    int *bloqueado = calloc(n, sizeof(int));
    long long soma_total = 0;

    for (int i = 0; i < n; i++) {
        elementos[i].valor = arr[i];
        elementos[i].pos = i;
    }

    // Ordena do maior para o menor
    qsort(elementos, n, sizeof(Elemento), comparar_elementos);

    for (int i = 0; i < n; i++) {
        int p = elementos[i].pos;
        if (bloqueado[p] == 0) {
            soma_total = soma_total + elementos[i].valor;
            bloqueado[p] = 1;
            
            // Bloqueia vizinhos adjacentes
            if (p > 0) {
                bloqueado[p - 1] = 1;
            }
            if (p < n - 1) {
                bloqueado[p + 1] = 1;
            }
        }
    }

    free(elementos);
    free(bloqueado);
    return soma_total;
}

// --- FUNCOES DE TESTE PARA OS CENARIOS ---

void realizar_teste(int n, int eh_cenario_armadilha) {
    int *arr = malloc(n * sizeof(int));
    
    if (eh_cenario_armadilha == 1) {
        // Exemplo especifico onde o Guloso falha (Qualidade da Solucao)
        int armadilha[] = {50, 100, 80, 20};
        for(int i = 0; i < 4; i++) arr[i] = armadilha[i];
        n = 4;
    } else {
        for(int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    }

    printf("\n--- Teste com N = %d ---\n", n);

    // Medicao Backtracking
    profundidade_maxima = 0;
    clock_t inicio_b = clock();
    long long res_b = algoritmo_backtracking(arr, n, 0, 1);
    clock_t fim_b = clock();
    double tempo_b = (double)(fim_b - inicio_b) / CLOCKS_PER_SEC;
    // Estimativa: cada quadro na pilha consome cerca de 64 bytes
    long memoria_stack = profundidade_maxima * 64; 

    // Medicao Guloso
    size_t memoria_heap = 0;
    clock_t inicio_g = clock();
    long long res_g = algoritmo_guloso(arr, n, &memoria_heap);
    clock_t fim_g = clock();
    double tempo_g = (double)(fim_g - inicio_g) / CLOCKS_PER_SEC;

    printf("METODO       | SOMA       | TEMPO (s)  | MEMORIA\n");
    printf("Backtracking | %-10lld | %-10.6f | %ld bytes (Stack)\n", res_b, tempo_b, memoria_stack);
    printf("Guloso       | %-10lld | %-10.6f | %zu bytes (Heap)\n", res_g, tempo_g, memoria_heap);
    
    if (res_b > res_g) {
        printf("Analise: O Guloso perdeu em qualidade por %lld unidades.\n", res_b - res_g);
    }

    free(arr);
}

int main() {
    srand(time(NULL));
    
    printf("TRABALHO: GULOSO VS BACKTRACKING [cite: 6]\n");

    // 1. Cenario Armadilha (Qualidade da Solucao [cite: 11])
    printf("\nCENARIO: ARMADILHA (QUALIDADE)");
    realizar_teste(4, 1);

    // 2. Cenario Escalabilidade (Tempo de Execucao [cite: 12])
    printf("\nCENARIO: ESCALABILIDADE (TEMPO)");
    realizar_teste(35, 0); // Backtracking comeca a demorar aqui

    // 3. Demonstracao para N grande (Apenas Guloso)
    printf("\nCENARIO: DADOS MASSIVOS (APENAS GULOSO)");
    size_t mem_g = 0;
    int n_grande = 100000;
    int *arr_grande = malloc(n_grande * sizeof(int));
    for(int i=0; i<n_grande; i++) arr_grande[i] = 10;
    
    clock_t ini = clock();
    algoritmo_guloso(arr_grande, n_grande, &mem_g);
    printf("\nN = 100.000 | Tempo Guloso: %.4f s | Memoria: %zu bytes\n", 
           (double)(clock() - ini) / CLOCKS_PER_SEC, mem_g);
    printf("Nota: O Backtracking travaria por tempo e Stack Overflow aqui.\n");

    free(arr_grande);
    return 0;
}