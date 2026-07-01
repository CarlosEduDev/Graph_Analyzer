#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

typedef struct Fila {
    int* dados;
    int frente;
    int tras;
    int capacidade;
} Fila;

Fila* criar_fila(int capacidade) {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->dados = (int*)malloc(capacidade * sizeof(int));
    f->frente = 0;
    f->tras = 0;
    f->capacidade = capacidade;
    return f;
}

void enfileirar(Fila* f, int elemento) {
    f->dados[f->tras++] = elemento;
}

int desenfileirar(Fila* f) {
    return f->dados[f->frente++];
}

int fila_vazia(Fila* f) {
    return f->frente == f->tras;
}

void liberar_fila(Fila* f) {
    free(f->dados);
    free(f);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int n, int i) {
    int smallest = i; 
    int l = 2 * i;     
    int r = 2 * i + 1; 

    if (l <= n && arr[l] < arr[smallest]) smallest = l;
    if (r <= n && arr[r] < arr[smallest]) smallest = r;

    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void heap_sort(int arr[], int n) {
    for (int i = n / 2; i >= 1; i--) {
        heapify(arr, n, i);
    }
    
    int temp_n = n;
    int* aux = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        aux[i] = arr[1];
        arr[1] = arr[temp_n];
        temp_n--;
        heapify(arr, temp_n, 1);
    }
    for (int i = 1; i <= n; i++) arr[i] = aux[i];
    free(aux);
}

Grafo* ler_grafo(const char* nome_arquivo, int tipo_representacao) {
    FILE* file = fopen(nome_arquivo, "r");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        return NULL;
    }

    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (fscanf(file, "%d", &g->num_vertices) != 1) {
        free(g);
        fclose(file);
        return NULL;
    }

    g->num_arestas = 0;
    g->tipo_representacao = tipo_representacao;
    g->matriz_adj = NULL;
    g->lista_adj = NULL;

    if (tipo_representacao == MATRIZ) {
        g->matriz_adj = (int**)malloc((g->num_vertices + 1) * sizeof(int*));
        for (int i = 1; i <= g->num_vertices; i++) {
            g->matriz_adj[i] = (int*)calloc((g->num_vertices + 1), sizeof(int));
        }
    } else if (tipo_representacao == LISTA) {
        g->lista_adj = (No**)calloc((g->num_vertices + 1), sizeof(No*));
    }

    int u, v;
    while (fscanf(file, "%d %d", &u, &v) == 2) {
        g->num_arestas++;
        if (tipo_representacao == MATRIZ) {
            g->matriz_adj[u][v] = 1;
            g->matriz_adj[v][u] = 1;
        } else if (tipo_representacao == LISTA) {
            
            No* novo1 = (No*)malloc(sizeof(No));
            novo1->vertice = v;
            novo1->prox = g->lista_adj[u];
            g->lista_adj[u] = novo1;

            No* novo2 = (No*)malloc(sizeof(No));
            novo2->vertice = u;
            novo2->prox = g->lista_adj[v];
            g->lista_adj[v] = novo2;
        }
    }

    fclose(file);
    return g;
}

void liberar_grafo(Grafo* g) {
    if (!g) return;

    if (g->tipo_representacao == MATRIZ && g->matriz_adj) {
        for (int i = 1; i <= g->num_vertices; i++) {
            free(g->matriz_adj[i]);
        }
        free(g->matriz_adj);
    } 
    else if (g->tipo_representacao == LISTA && g->lista_adj) {
        for (int i = 1; i <= g->num_vertices; i++) {
            No* atual = g->lista_adj[i];
            while (atual) {
                No* temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }
        free(g->lista_adj);
    }
    free(g);
}

void gerar_estatistica(Grafo* g, const char* nome_arquivo_saida) {
    FILE* out = fopen(nome_arquivo_saida, "w");
    if (!out) return;

    int* graus = (int*)calloc((g->num_vertices + 1), sizeof(int));
    int grau_min = g->num_vertices;
    int grau_max = 0;
    long long soma_graus = 0;

    // Levantamento dos graus
    for (int i = 1; i <= g->num_vertices; i++) {
        if (g->tipo_representacao == MATRIZ) {
            for (int j = 1; j <= g->num_vertices; j++) {
                if (g->matriz_adj[i][j]) graus[i]++;
            }
        } else {
            No* atual = g->lista_adj[i];
            while (atual) {
                graus[i]++;
                atual = atual->prox;
            }
        }
        if (graus[i] < grau_min) grau_min = graus[i];
        if (graus[i] > grau_max) grau_max = graus[i];
        soma_graus += graus[i];
    }

    double grau_medio = (double)soma_graus / g->num_vertices;

    heap_sort(graus, g->num_vertices);
    double mediana;
    if (g->num_vertices % 2 != 0) {
        mediana = graus[(g->num_vertices + 1) / 2];
    } else {
        mediana = (graus[g->num_vertices / 2] + graus[(g->num_vertices / 2) + 1]) / 2.0;
    }

    fprintf(out, "Número de vértices: %d\n", g->num_vertices);
    fprintf(out, "Número de arestas: %d\n", g->num_arestas);
    fprintf(out, "Grau mínimo: %d\n", grau_min);
    fprintf(out, "Grau máximo: %d\n", grau_max);
    fprintf(out, "Grau médio: %.2f\n", grau_medio);
    fprintf(out, "Mediana de grau: %.2f\n", mediana);

    free(graus);
    fclose(out);
    
    encontrar_conexos(g, nome_arquivo_saida);
}


void busca_largura(Grafo* g, int vertice_inicial, const char* arquivo_saida) {
    int* pai = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    int* nivel = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    int* visitado = (int*)calloc((g->num_vertices + 1), sizeof(int));

    for (int i = 1; i <= g->num_vertices; i++) {
        pai[i] = -1;
        nivel[i] = -1;
    }

    Fila* f = criar_fila(g->num_vertices + 1);

    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;
    pai[vertice_inicial] = 0;
    enfileirar(f, vertice_inicial);

    while (!fila_vazia(f)) {
        int u = desenfileirar(f);

        if (g->tipo_representacao == MATRIZ) {
            for (int v = 1; v <= g->num_vertices; v++) {
                if (g->matriz_adj[u][v] && !visitado[v]) {
                    visitado[v] = 1;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    enfileirar(f, v);
                }
            }
        } else {
            No* atual = g->lista_adj[u];
            while (atual) {
                int v = atual->vertice;
                if (!visitado[v]) {
                    visitado[v] = 1;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    enfileirar(f, v);
                }
                atual = atual->prox;
            }
        }
    }

    FILE* out = fopen(arquivo_saida, "w");
    if (out) {
        fprintf(out, "\n--- ÁRVORE GERADORA BFS (Raiz: %d) ---\n", vertice_inicial);
        for (int i = 1; i <= g->num_vertices; i++) {
            if (nivel[i] != -1) {
                fprintf(out, "Vértice: %d | Pai: %d | Nível: %d\n", i, pai[i], nivel[i]);
            }
        }
        fclose(out);
    }

    liberar_fila(f);
    free(pai);
    free(nivel);
    free(visitado);
}

void dfs_recursivo(Grafo* g, int u, int* visitado, int* pai, int* nivel, int nivel_atual) {
    visitado[u] = 1;
    nivel[u] = nivel_atual;

    if (g->tipo_representacao == MATRIZ) {
        for (int v = 1; v <= g->num_vertices; v++) {
            if (g->matriz_adj[u][v] && !visitado[v]) {
                pai[v] = u;
                dfs_recursivo(g, v, visitado, pai, nivel, nivel_atual + 1);
            }
        }
    } else {
        No* atual = g->lista_adj[u];
        while (atual) {
            int v = atual->vertice;
            if (!visitado[v]) {
                pai[v] = u;
                dfs_recursivo(g, v, visitado, pai, nivel, nivel_atual + 1);
            }
            atual = atual->prox;
        }
    }
}

void busca_profundidade(Grafo* g, int vertice_inicial, const char* arquivo_saida) {
    int* pai = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    int* nivel = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    int* visitado = (int*)calloc((g->num_vertices + 1), sizeof(int));

    for (int i = 1; i <= g->num_vertices; i++) {
        pai[i] = -1;
        nivel[i] = -1;
    }

    pai[vertice_inicial] = 0;
    dfs_recursivo(g, vertice_inicial, visitado, pai, nivel, 0);

    FILE* out = fopen(arquivo_saida, "a");
    if (out) {
        fprintf(out, "\n--- ÁRVORE GERADORA DFS (Raiz: %d) ---\n", vertice_inicial);
        for (int i = 1; i <= g->num_vertices; i++) {
            if (nivel[i] != -1) {
                fprintf(out, "Vértice: %d | Pai: %d | Nível: %d\n", i, pai[i], nivel[i]);
            }
        }
        fclose(out);
    }

    free(pai);
    free(nivel);
    free(visitado);
}

int calcular_distancia(Grafo* g, int origem, int destino) {
    if (origem == destino) return 0;

    int* nivel = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    int* visitado = (int*)calloc((g->num_vertices + 1), sizeof(int));
    for (int i = 1; i <= g->num_vertices; i++) nivel[i] = -1;

    Fila* f = criar_fila(g->num_vertices + 1);

    visitado[origem] = 1;
    nivel[origem] = 0;
    enfileirar(f, origem);

    int dist = -1;
    while (!fila_vazia(f)) {
        int u = desenfileirar(f);

        if (u == destino) {
            dist = nivel[u];
            break;
        }

        if (g->tipo_representacao == MATRIZ) {
            for (int v = 1; v <= g->num_vertices; v++) {
                if (g->matriz_adj[u][v] && !visitado[v]) {
                    visitado[v] = 1;
                    nivel[v] = nivel[u] + 1;
                    enfileirar(f, v);
                }
            }
        } else {
            No* atual = g->lista_adj[u];
            while (atual) {
                int v = atual->vertice;
                if (!visitado[v]) {
                    visitado[v] = 1;
                    nivel[v] = nivel[u] + 1;
                    enfileirar(f, v);
                }
                atual = atual->prox;
            }
        }
    }

    liberar_fila(f);
    free(nivel);
    free(visitado);
    return dist;
}

int calcular_diametro(Grafo* g) {
    int max_dist = 0;
    
    for (int i = 1; i <= g->num_vertices; i++) {
        for (int j = i + 1; j <= g->num_vertices; j++) {
            int d = calcular_distancia(g, i, j);
            if (d > max_dist) max_dist = d;
        }
    }
    return max_dist;
}


typedef struct Componente {
    int* vertices;
    int tamanho;
} Componente;

int comparar_componentes(const void* a, const void* b) {
    return ((Componente*)b)->tamanho - ((Componente*)a)->tamanho;
}

void encontrar_conexos(Grafo* g, const char* arquivo_saida) {
    int* visitado = (int*)calloc((g->num_vertices + 1), sizeof(int));
    Componente* comps = (Componente*)malloc((g->num_vertices + 1) * sizeof(Componente));
    int qtd_componentes = 0;

    for (int i = 1; i <= g->num_vertices; i++) {
        if (!visitado[i]) {
            qtd_componentes++;
            comps[qtd_componentes].vertices = (int*)malloc((g->num_vertices) * sizeof(int));
            int tam = 0;

            Fila* f = criar_fila(g->num_vertices + 1);
            visitado[i] = 1;
            enfileirar(f, i);

            while (!fila_vazia(f)) {
                int u = desenfileirar(f);
                comps[qtd_componentes].vertices[tam++] = u;

                if (g->tipo_representacao == MATRIZ) {
                    for (int v = 1; v <= g->num_vertices; v++) {
                        if (g->matriz_adj[u][v] && !visitado[v]) {
                            visitado[v] = 1;
                            enfileirar(f, v);
                        }
                    }
                } else {
                    No* atual = g->lista_adj[u];
                    while (atual) {
                        int v = atual->vertice;
                        if (!visitado[v]) {
                            visitado[v] = 1;
                            enfileirar(f, v);
                        }
                        atual = atual->prox;
                    }
                }
            }
            comps[qtd_componentes].tamanho = tam;
            liberar_fila(f);
        }
    }

    qsort(&comps[1], qtd_componentes, sizeof(Componente), comparar_componentes);

    FILE* out = fopen(arquivo_saida, "a");
    if (out) {
        fprintf(out, "\n--- COMPONENTES CONEXAS ---\n");
        fprintf(out, "Quantidade total de componentes: %d\n", qtd_componentes);
        for (int c = 1; c <= qtd_componentes; c++) {
            fprintf(out, "Componente %d | Tamanho: %d | Vértices: ", c, comps[c].tamanho);
            for (int v = 0; v < comps[c].tamanho; v++) {
                fprintf(out, "%d ", comps[c].vertices[v]);
            }
            fprintf(out, "\n");
            free(comps[c].vertices);
        }
        fclose(out);
    }

    free(comps);
    free(visitado);
}