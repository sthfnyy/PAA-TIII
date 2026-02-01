#include <stdio.h>
#include <time.h>

#define MAX_ARRAY 1000

// --- FUNÇÕES AUXILIARES ---
int max(int a, int b) { return (a > b) ? a : b; }

// --- ABORDAGEM GULOSA ---
int algoritmo_guloso(int arr[], int n) {
    int soma_total = 0;
    int bloqueado[MAX_ARRAY] = {0}; // Estrutura principal de memória

    for (int i = 0; i < n; i++) {
        int maior_valor = -1;
        int pos_maior = -1;
        for (int j = 0; j < n; j++) {
            if (bloqueado[j] == 0 && arr[j] > maior_valor) {
                maior_valor = arr[j];
                pos_maior = j;
            }
        }
        if (pos_maior != -1) {
            soma_total += arr[pos_maior];
            bloqueado[pos_maior] = 1;
            if (pos_maior > 0) bloqueado[pos_maior - 1] = 1;
            if (pos_maior < n - 1) bloqueado[pos_maior + 1] = 1;
        }
    }
    return soma_total;
}

// --- ABORDAGEM BACKTRACKING ---
int algoritmo_backtracking(int arr[], int n, int i) {
    if (i >= n) return 0;
    int escolhe = arr[i] + algoritmo_backtracking(arr, n, i + 2);
    int pula = algoritmo_backtracking(arr, n, i + 1);
    return max(escolhe, pula);
}

int main() {
    // Array de teste (n = 10)
    int dados[] = {10, 20, 15, 1, 15, 30, 10, 5, 40, 25};
    int n = 10;

    printf("=== ANALISE DE MEMORIA TEORICA E TEMPO ===\n\n");

    // --- EXECUÇÃO E TEMPO ---
    clock_t t_ini_g = clock();
    int res_g = algoritmo_guloso(dados, n);
    clock_t t_fim_g = clock();
    double tempo_g = ((double)(t_fim_g - t_ini_g) * 1000000) / CLOCKS_PER_SEC;

    clock_t t_ini_b = clock();
    int res_b = algoritmo_backtracking(dados, n, 0);
    clock_t t_fim_b = clock();
    double tempo_b = ((double)(t_fim_b - t_ini_b) * 1000000) / CLOCKS_PER_SEC;

    // --- CÁLCULO TEÓRICO DE MEMÓRIA ---
    
    // 1. Memória do Guloso:
    // Ocupa o array 'bloqueado' + variáveis locais (soma, i, j, maior, pos)
    size_t mem_guloso = (MAX_ARRAY * sizeof(int)) + (5 * sizeof(int));

    // 2. Memória do Backtracking:
    // Ocupa a Pilha de Recursão. Cada chamada guarda:
    // Parâmetros (arr*, n, i) + Variáveis (escolhe, pula) + Endereço de Retorno
    // Estimativa média: 32 bytes por nível de recursão.
    int profundidade_maxima = n; 
    size_t mem_backtracking = profundidade_maxima * 32; 

    // --- RESULTADOS ---
    printf(">> ALGORITMO GULOSO:\n");
    printf("   Soma: %d | Tempo: %.2f us\n", res_g, tempo_g);
    printf("   Memoria Estimada: %zu bytes (Fixa para ate %d elementos)\n\n", mem_guloso, MAX_ARRAY);

    printf(">> ALGORITMO BACKTRACKING:\n");
    printf("   Soma: %d | Tempo: %.2f us\n", res_b, tempo_b);
    printf("   Memoria Estimada na Pilha: %zu bytes (Baseada na profundidade %d)\n\n", mem_backtracking, n);

    printf("OBS: Enquanto o Guloso tem memoria constante, o Backtracking cresce\n");
    printf("linearmente com o tamanho da entrada (n) na pilha do sistema.\n");

    return 0;
}