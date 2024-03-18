#include <stdio.h>
#include "links.h"

void generate_matrices(int H[NUM_SITES][NUM_SITES], double G[NUM_SITES][NUM_SITES]) {

    printf("Hyperlink matrix H:\n");
    for (int i = 0; i < NUM_SITES; i++) {
        for (int j = 0; j < NUM_SITES; j++) {
            printf("%d ", H[i][j]);
        }
        printf("\n");
    }

    // Calculate stochastic matrix S
    double S[NUM_SITES][NUM_SITES];
    for (int j = 0; j < NUM_SITES; j++) {
        int sum = 0;
        for (int i = 0; i < NUM_SITES; i++) {
            sum += H[i][j];
        }
        
        for (int i = 0; i < NUM_SITES; i++) {
            S[i][j] = (double)H[i][j] / sum;
        }
    }

    printf("Stochastic matrix S:\n");
    
    for (int i = 0; i < NUM_SITES; i++) {
        for (int j = 0; j < NUM_SITES; j++) {
            printf("%.2lf ", S[i][j]);
        }
        printf("\n");
    }

    // Calculate Google matrix G
    double damping_factor = 0.85;

    double teleportation_prob = (1 - damping_factor) / NUM_SITES;

    for (int i = 0; i < NUM_SITES; i++) {
        for (int j = 0; j < NUM_SITES; j++) {
            G[i][j] = damping_factor * S[i][j] + teleportation_prob;
        }
    }

    printf("Google matrix G:\n");
    for (int i = 0; i < NUM_SITES; i++) {
        for (int j = 0; j < NUM_SITES; j++) {
            printf("%.2lf ", G[i][j]);
        }
        printf("\n");
    }
}

void get_matrices(double google_matrix[NUM_SITES][NUM_SITES]) {
    int hyperlink_matrix[NUM_SITES][NUM_SITES] = {0};

    // Fill the hyperlink matrix
    get_serial_hyperlink_matrix(hyperlink_matrix);
    // generate google matrix
    generate_matrices(hyperlink_matrix, google_matrix);
}