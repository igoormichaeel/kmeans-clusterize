#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib/csvReader.h"
#include "lib/kmeans.h"
#define MAXCHAR 1000

int main(int argc, char* argv[]) {
    int no_iteration = strtol(argv[1], NULL, 10);
    int clusters = 6;
    double processing_time;
    clock_t start, finish;
    // Read dataset and assign the elements to struct.
    struct CSVFile csv_data = readCsv("data/genres_v2.csv");
    struct KMeans model = {
        .no_iteration = no_iteration,
        .no_clusters = clusters,
        .centroids = malloc(sizeof(float) * csv_data.columns * clusters),
        .columns = csv_data.columns,
        .rows = csv_data.rows,
        .data = csv_data.data,
        .data_clusters = malloc(sizeof(int) * csv_data.rows)};

    // Initialize the model with random centroids.
    init_model(&model);

    // Computing time taken to run the function.
    start = clock();

    // Run the KMeans clustering algorithm on data points.
    fit(&model);

    // Display the results.
    // printCentroids(&model);
    // printClusterCount(&model);

    finish = clock();
    processing_time = ((double)(finish - start) / CLOCKS_PER_SEC);

    printf("\nThe program took %f seconds to processing.\n", processing_time);

    char* resFile = "cluster_results_serial.csv";
    write_to_csv(&model, resFile);
    return 0;
}