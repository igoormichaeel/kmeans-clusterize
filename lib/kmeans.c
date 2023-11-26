//
// Created by pi on 12/3/21.
//

#include "kmeans.h"

#include <malloc.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 100

// Função para inicializar centróides de forma aleatória
void random_centroids(struct KMeans *model) {
    float *data = model->data;
    int rows = model->rows;
    int columns = model->columns;
    int clusters = model->no_clusters;

    for (int cluster = 0; cluster < clusters; cluster++) {
        float random_number = ((float)rand() / (float)RAND_MAX);
        int random_row = (int)(random_number * rows);

        for (int i = 0; i < columns; i++) {
            model->centroids[columns * cluster + i] =
                data[random_row * columns + i];
        }
    }
}

// Função para calcular a distância euclidiana entre dois pontos
float euclidean_dist(float *pt1, struct KMeans *model, int pt2_index) {
    float dist = 0;
    for (int i = 0; i < model->columns; i++) {
        int data_index = model->columns * pt2_index + i;
        dist += (pt1[i] - model->data[data_index]) *
                (pt1[i] - model->data[data_index]);
    }
    return sqrtf(dist);
}

// Função para obter o centróide de um cluster
float *get_cluster_centroid(struct KMeans *model, int cluster) {
    float *centroids = malloc(sizeof(float) * model->columns);
    for (int i = 0; i < model->columns; i++) {
        centroids[i] = model->centroids[cluster * model->columns + i];
    }
    return centroids;
}

// Inicializar o modelo KMeans
void init_model(struct KMeans *cluster) { random_centroids(cluster); }

// Atribuir um ponto a um cluster com base na distância euclidiana
void assign_cluster(const float *cluster_dist, struct KMeans *model,
                    int index) {
    if (cluster_dist == NULL) {
        fprintf(stderr,
                "Distância do cluster não inicializada ao atribuir....");
        exit(0);
    }

    float min_dist = cluster_dist[0];
    model->data_clusters[index] = 0;

    for (int i = 1; i < model->no_clusters; i++) {
        if (min_dist >= cluster_dist[i]) {
            min_dist = cluster_dist[i];
            model->data_clusters[index] = i;
        }
    }
}

// Adicionar valores de um ponto ao somatório de um cluster
void add_to_cluster_sum(int row, float *cluster_sum, int cluster,
                        struct KMeans *model) {
    for (int i = 0; i < model->columns; i++) {
        cluster_sum[cluster * model->columns + i] +=
            model->data[row * model->columns + i];
    }
}

// Atualizar os centróides com base na média dos pontos do cluster
void update_centroids(struct KMeans *model) {
    int cluster_count[model->no_clusters];
    float sum_cluster_columns[model->no_clusters * model->columns];

    // Inicializa a contagem de clusters para 0
    for (int i = 0; i < model->no_clusters; i++) {
        cluster_count[i] = 0;
        for (int c = 0; c < model->columns; c++) {
            sum_cluster_columns[i * model->columns + c] = 0;
        }
    }

    // Itera por todos os pontos de dados e calcula o somatório dos pontos de
    // colunas e obtém a contagem para o cluster dado. Isso será usado mais
    // tarde para calcular a média e atualizar o centróide.
    for (int row = 0; row < model->rows; row++) {
        // Variável booleana para verificar se o ponto foi atribuído a algum
        // cluster
        int assigned_to_cluster = 0;

        for (int k = 0; k < model->no_clusters; k++) {
            if (model->data_clusters[row] == k) {
                // Atualiza a contagem e o somatório do cluster para o ponto
                // atual
                cluster_count[k]++;
                add_to_cluster_sum(row, sum_cluster_columns, k, model);
                // Ativa a flag indicando que o ponto foi atribuído a algum
                // cluster
                assigned_to_cluster = 1;
            }
        }

        // Se o ponto não foi atribuído a nenhum cluster, pode ser necessário
        // ajustar a lógica aqui dependendo dos requisitos do seu algoritmo
        if (!assigned_to_cluster) {
            // Lógica a ser executada quando o ponto não é atribuído a nenhum
            // cluster
        }
    }

    // Atualiza os centróides com a média
    for (int k = 0; k < model->no_clusters; k++) {
        for (int c = 0; c < model->columns; c++) {
            if (cluster_count[k] != 0) {
                model->centroids[k * model->columns + c] =
                    sum_cluster_columns[k * model->columns + c] /
                    (float)cluster_count[k];
            }
        }
    }
}
// Imprimir os centróides
void print_centroids(struct KMeans *model) {
    // printf("\n printing centroids: \n");
    // for (int cluster = 0; cluster < model->no_clusters; cluster++) {
    //     for (int i = 0; i < model->columns; i++) {
    //         float point = model->centroids[model->columns * cluster + i];
    //         printf("%f, ", point);
    //     }
    //     printf("\n");
    // }
}

// Imprimir a contagem de pontos por cluster
void print_cluster_count(struct KMeans *model) {
    // int cluster_count[model->no_clusters];
    // for (int i = 0; i < model->no_clusters; i++) cluster_count[i] = 0;

    // for (int row = 0; row < model->rows; row++) {
    //     for (int k = 0; k < model->no_clusters; k++) {
    //         if (model->data_clusters[row] == k) {
    //             cluster_count[k] = cluster_count[k] + 1;
    //             break;
    //         }
    //     }
    // }

    // for (int i = 0; i < model->no_clusters; i++)
    //     printf("\nCount for cluster: %d = %d", i, cluster_count[i]);
}

// Algoritmo principal para ajustar o modelo KMeans
void fit(struct KMeans *model) {
    // Inicializa os centróides e imprime-os
    print_centroids(model);

    // Aloca espaço para os centróides dos clusters
    float **clusterCentroids = malloc(sizeof(float *) * model->no_clusters);
    int iteration = model->no_iteration;

    // Itera pelo algoritmo KMeans
    while (iteration >= 0) {
        // Obtém os centróides dos clusters
        for (int k = 0; k < model->no_clusters; k++) {
            clusterCentroids[k] = get_cluster_centroid(model, k);
        }

        // Atribui pontos aos clusters com base na distância euclidiana
        for (int i = 0; i < model->rows; i++) {
            float min_dist = euclidean_dist(clusterCentroids[0], model, i);
            model->data_clusters[i] = 0;

            for (int k = 1; k < model->no_clusters; k++) {
                float dist = euclidean_dist(clusterCentroids[k], model, i);
                if (dist <= min_dist) {
                    min_dist = dist;
                    model->data_clusters[i] = k;
                }
            }
        }

        // Após cada iteração, atualize os centróides.
        update_centroids(model);
        iteration--;
    }
}

// Escrever os resultados em um arquivo CSV

void write_to_csv(struct KMeans *model, char *filename) {
    /*

    // Abre o arquivo CSV para escrita
    FILE *fp = fopen(filename, "a");

    // Verifica se o arquivo foi aberto com sucesso
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int row_index = 0;
    // Escreve os dados e clusters no arquivo CSV
    while (row_index < model->rows) {
        int column = 0;

        while (column < model->columns) {
            fprintf(fp, "%f,",
                    model->data[row_index * model->columns + column]);
            column++;
        }

        fprintf(fp, "%d\n", model->data_clusters[row_index]);
        row_index++;
    }

    // Fecha o arquivo após a escrita
    fclose(fp);
    */
}