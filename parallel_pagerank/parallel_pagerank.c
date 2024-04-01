#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define NUM_SITES 895
#define MAX_LINK_LEN 200
#define DAMPING_FACTOR 0.85
#define DEBUG 1

void generate_matrices(int **H, double **G, int num_sites) {
    // Calculate stochastic matrix S
    double **S = (double**)malloc(NUM_SITES * sizeof(double*));
    for (int i = 0; i < NUM_SITES; i++) {
        S[i] = (double*)malloc(NUM_SITES * sizeof(double));
    }
    for (int j = 0; j < NUM_SITES; j++) {
        int sum = 0;
        for (int i = 0; i < NUM_SITES; i++) {
            sum += H[i][j];
        }
        for (int i = 0; i < NUM_SITES; i++) {
            S[i][j] = sum ? (double) H[i][j] / sum : 1.0 / NUM_SITES;
        }
    }

    // Calculate Google matrix G
    double teleportation_prob = (1.0 - DAMPING_FACTOR) / NUM_SITES;
    for (int i = 0; i < NUM_SITES; i++) {
        for (int j = 0; j < NUM_SITES; j++) {
            G[i][j] = DAMPING_FACTOR * S[i][j] + teleportation_prob;
        }
    }
}

void get_hyperlink_matrix_from_file(int **H, int size) {
    // Read hyperlink matrix from file
    // Each row represents outgoing links from a site
    // Each column represents incoming links to a site
    // Entry H[i][j] = 1 if there is a link from site j to site i, otherwise 0
    FILE *fp = fopen("hyperlink_matrix.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%d", &H[i][j]);
            //printf("%d", H[i][j]);
        }
    }
    fclose(fp); 
}

void fill_links_array(char LINKS_ARRAY[NUM_SITES][MAX_LINK_LEN], int size) {
    // Fill LINKS_ARRAY with website names
    // Each row corresponds to a website
    // Example: strcpy(LINKS_ARRAY[0], "www.example.com");
    FILE *fp = fopen("website_names.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    char buffer[MAX_LINK_LEN];
    for (int i = 0; i < size; i++) {
        fgets(buffer, MAX_LINK_LEN, fp);
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(LINKS_ARRAY[i], buffer);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Trick to contiguously allocate 2d array from
    //https://stackoverflow.com/questions/5104847/mpi-bcast-a-dynamic-2d-array
    double *p = (double*)malloc(NUM_SITES * NUM_SITES * sizeof(double));
    if (!p) return -1;
    double **google_matrix = (double**)malloc(NUM_SITES*sizeof(double*));
    if (!google_matrix) {
        printf("fail");
        free(p);
        return -1;
    }
    for (int i = 0; i < NUM_SITES; i++) {
        google_matrix[i] = &(p[i*NUM_SITES]);
    }
    /* for (int i = 0; i < NUM_SITES; i++) {
        google_matrix[i] = (double*)malloc(NUM_SITES * sizeof(double));
        for (int j = 0; j < NUM_SITES; j++) {
            google_matrix[i][j] = 0;
        }
    } */
    int **hyperlink_matrix;
    double pagerank[NUM_SITES];
    int indices[NUM_SITES];
    if (rank == 0) {
        hyperlink_matrix = (int**)malloc(NUM_SITES * sizeof(int*));
        for (int i = 0; i < NUM_SITES; i++) {
            hyperlink_matrix[i] = (int*)malloc(NUM_SITES * sizeof(int));
            for (int j = 0; j < NUM_SITES; j++) {
                hyperlink_matrix[i][j] = 0;
            }
        }
        get_hyperlink_matrix_from_file(hyperlink_matrix, NUM_SITES);
        if (0) {
            for (int i = 0; i < NUM_SITES; i++) {
                for (int j = 0; j < NUM_SITES; j++) {
                    printf("%d ", hyperlink_matrix[i][j]);
                }
                printf("\n");
            }
        }
        generate_matrices(hyperlink_matrix, google_matrix, NUM_SITES);
    }
    printf("Got to MPI_Bcast"); 
    // Broadcast the Google matrix to all processes
    MPI_Bcast(&(google_matrix[0][0]), NUM_SITES * NUM_SITES, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Power iteration method
    double tolerance = 1e-6;
    int maxIterations = 100;
    for (int i = 0; i < NUM_SITES; i++) {
        pagerank[i] = 1.0 / NUM_SITES; // Initialize PageRank scores
    } 
    
    for (int iter = 0; iter < maxIterations; iter++) {
        double new_pagerank[NUM_SITES] = {0};
        for (int i = 0; i < NUM_SITES; i++) {
            for (int j = 0; j < NUM_SITES; j++) {
                new_pagerank[i] += google_matrix[i][j] * pagerank[j];
            }
        }
        // Compute difference and check convergence
        double diff = 0.0;
        for (int i = 0; i < NUM_SITES; i++) {
            diff += fabs(new_pagerank[i] - pagerank[i]);
            pagerank[i] = new_pagerank[i];
        }

        // Use MPI_Allreduce to get the sum of diffs from all processes
        double global_diff = 0.0;
        MPI_Allreduce(&diff, &global_diff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        
        // Check convergence across all processes
        if (global_diff < tolerance) {
            break;
        }
    }
    
    if (rank == 0) {
        // Dont forget to free hyperlink matrix; 
        // Gather PageRank scores and sort
        for (int i = 0; i < NUM_SITES; i++) {
            indices[i] = i;
        }
        for (int i = 0; i < NUM_SITES - 1; i++) {
            for (int j = i + 1; j < NUM_SITES; j++) {
                if (pagerank[j] > pagerank[i]) {
                    double temp = pagerank[i];
                    pagerank[i] = pagerank[j];
                    pagerank[j] = temp;
                    int temp_index = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp_index;
                }
            }
        }

        // Print the PageRank scores
        printf("PageRank scores:\n");
        char file_links_array[NUM_SITES][MAX_LINK_LEN];
        fill_links_array(file_links_array, NUM_SITES);
        for (int i = 0; i < NUM_SITES; i++) {
            printf("%-50s: %f\n", file_links_array[indices[i]], pagerank[i]);
        }
    } 
    if (rank == 0){
        for (int i = 0; i < NUM_SITES; i++) {
            free(hyperlink_matrix[i]);
        }
        free (hyperlink_matrix);
    }
    free(&(google_matrix[0][0]));
    MPI_Finalize();
    return 0;
}


