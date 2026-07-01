#ifndef GRAFO_H
#define GRAFO_H

#define MATRIZ 1
#define LISTA 2

typedef struct No {
    int vertice;
    struct No* prox;
} No;


typedef struct {
    int num_vertices;
    int num_arestas;
    int tipo_representacao; // pode ser matriz ou lista

    int **matriz_adj;
    No **lista_adj;
} Grafo;

// FUNÇÕES PARA MANIPULAR GRAFOS
Grafo* ler_grafo(const char *nome_arquivo, int tipo_representacao);
void liberar_grafo(Grafo *grafo);
void gerar_estatistica(Grafo *grafo, const char *nome_arquivo_saida);

// ALGORITMO DE BUSCA E CALCULO EM GRAFO
void busca_largura(Grafo *grafo, int vertice_inicial, const char *nome_arquivo_saida);
void busca_profundidade(Grafo *grafo, int vertice_inicial, const char *nome_arquivo_saida);
int calcular_distancia(Grafo *grafo, int origem, int destino);
int calcular_diametro(Grafo *grafo);
void encontrar_conexos(Grafo *grafo, const char *nome_arquivo_saida);

#endif