#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"

int main() {
    printf("Carregando o Grafo 1...\n");
    Grafo *grafo = ler_grafo("grafos/grafo_1.txt", 1);

    if (!grafo) {
        printf("Erro ao ler o grafo do arquivo.\n");
        return 1;
    }
    printf("Grafo carregado com sucesso!\n\n");

    printf("Gerando estatisticas gerais do grafo...\n");
    gerar_estatistica(grafo, "saidas/estatisticas_grafo1.txt");

    printf("Gerando arvores BFS e DFS para as raizes 1, 2 e 3...\n");
    
    // Execuções da Busca em Largura (BFS)
    busca_largura(grafo, 1, "saidas/resultado_bfs_raiz1.txt");
    busca_largura(grafo, 2, "saidas/resultado_bfs_raiz2.txt");
    busca_largura(grafo, 3, "saidas/resultado_bfs_raiz3.txt");

    // Execuções da Busca em Profundidade (DFS)
    busca_profundidade(grafo, 1, "saidas/resultado_dfs_raiz1.txt");
    busca_profundidade(grafo, 2, "saidas/resultado_dfs_raiz2.txt");
    busca_profundidade(grafo, 3, "saidas/resultado_dfs_raiz3.txt");
    
    printf("Arquivos de busca salvos na pasta 'saidas/'.\n\n");

    printf("=== DISTANCIAS PONTUAIS ===\n");
    printf("Distancia entre (10, 20): %d arestas\n", calcular_distancia(grafo, 10, 20));
    printf("Distancia entre (10, 30): %d arestas\n", calcular_distancia(grafo, 10, 30));
    printf("Distancia entre (20, 30): %d arestas\n", calcular_distancia(grafo, 20, 30));
    printf("\n");

    printf("=== COMPONENTES CONEXAS ===\n");
    encontrar_conexos(grafo, "saidas/componentes_conexas_grafo1.txt");
    printf("Arquivo de componentes salvo em 'saidas/'.\n\n");

    printf("=== DIAMETRO DO GRAFO ===\n");
    printf("Calculando o diametro (isso pode levar alguns segundos)...\n");
    int diametro = calcular_diametro(grafo);
    printf("Diâmetro do Grafo 1: %d arestas\n\n", diametro);

    printf("=== TESTE DE DESEMPENHO (100 BUSCAS) ===\n");
    
    clock_t inicio_bfs = clock();
    for (int i = 1; i <= 100; i++) {
        busca_largura(grafo, i, NULL); 
    }
    clock_t fim_bfs = clock();
    double tempo_medio_bfs = (((double)(fim_bfs - inicio_bfs)) / CLOCKS_PER_SEC) / 100.0;
    printf("Tempo medio da BFS: %f segundos\n", tempo_medio_bfs);

    clock_t inicio_dfs = clock();
    for (int i = 1; i <= 100; i++) {
        busca_profundidade(grafo, i, NULL);
    }
    clock_t fim_dfs = clock();
    double tempo_medio_dfs = (((double)(fim_dfs - inicio_dfs)) / CLOCKS_PER_SEC) / 100.0;
    printf("Tempo medio da DFS: %f segundos\n\n", tempo_medio_dfs);

    liberar_grafo(grafo);
    printf("Memoria do grafo liberada. Fim da execucao.\n");

    return 0;
}