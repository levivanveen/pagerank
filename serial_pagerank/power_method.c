#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix_generation.h"
#include "links.h"

void power_method(double matrix[NUM_SITES][NUM_SITES], int size, double pagerank[]) {
    // Initialize the PageRank vector with equal probabilities
    for (int i = 0; i < size; i++) {
        pagerank[i] = 1.0 / size;
    }

    // Convergence criteria
    double tolerance = 1e-6;
    int maxIterations = 100;

    // Power iteration method
    for (int iter = 0; iter < maxIterations; iter++) {
        double new_pagerank[NUM_SITES] = {0};

        // Multiply matrix by PageRank vector
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                new_pagerank[i] += matrix[i][j] * pagerank[j];

            }
        }

        // Check convergence
        double diff = 0.0;
        for (int i = 0; i < size; i++) {
            diff += fabs(new_pagerank[i] - pagerank[i]);
        }

        // Update PageRank vector
        for (int i = 0; i < size; i++) {
            pagerank[i] = new_pagerank[i];
        }

        // If convergence achieved, exit
        if (diff < tolerance) {
            break;
        }
    }
}

void sort_pagerank(double pagerank[], int indices[], int size) {
    // Initialize indices
    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }

    // Bubble sort
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (pagerank[j] < pagerank[j + 1]) {
                // Swap pagerank
                double temp = pagerank[j];
                pagerank[j] = pagerank[j + 1];
                pagerank[j + 1] = temp;
                // Swap indices
                int temp_index = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp_index;
            }
        }
    }
}


int main() {
    double google_matrix[NUM_SITES][NUM_SITES];
    double pagerank[NUM_SITES];
    int indices[NUM_SITES];

    get_matrices(google_matrix);
    power_method(google_matrix, NUM_SITES, pagerank);

    // Sort the PageRank scores and keep track of the indices
    sort_pagerank(pagerank, indices, NUM_SITES);

    int link_print_len = 30;
    // Print the PageRank scores
    printf("PageRank scores:\n");
    printf("%-5s %-*s %-10s\n", "Rank", link_print_len, "Website", "PageRank");
    for (int i = 0; i < NUM_SITES; i++) {
        printf("%-5d %-*s %.6f\n", indices[i] + 1, link_print_len, LINKS_ARRAY[indices[i]], pagerank[i]);
    }

    return 0;
}