#ifndef HPCFINAL_KMEANS_H
#define HPCFINAL_KMEANS_H

struct KMeans {
    int no_iteration;
    int no_clusters;
    float *centroids;
    int rows;
    int columns;
    float *data;
    int *data_clusters;
};

void random_centroids(struct KMeans *model);
float euclidean_dist(float *pt1, struct KMeans *model, int pt2_index);
float *get_cluster_centroid(struct KMeans *model, int cluster);
void init_model(struct KMeans *cluster);
void assign_cluster(const float *cluster_dist, struct KMeans *model, int index);
void add_to_cluster_sum(int row, float *cluster_sum, int cluster,
                        struct KMeans *model);
void update_centroids(struct KMeans *model);
void print_centroids(struct KMeans *model);
void print_cluster_count(struct KMeans *model);
void fit(struct KMeans *model);
void write_to_csv(struct KMeans *model, char *filename);

#endif  // HPCFINAL_KMEANS_H