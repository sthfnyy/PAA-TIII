int max(int a, int b) { return (a > b) ? a : b; }

int backtracking(int arr[], int n, int i) {
    if (i >= n) return 0;
    
    // Testa: incluir o atual e pular o próximo OU apenas ignorar o atual
    int incluir = arr[i] + backtracking(arr, n, i + 2);
    int excluir = backtracking(arr, n, i + 1);

    return max(incluir, excluir);
}