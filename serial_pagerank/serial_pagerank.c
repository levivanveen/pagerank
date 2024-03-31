#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "links.h"

#define NUM_SITES 15
#define MAX_LINK_LEN 50

extern const char LINKS_ARRAY[NUM_SITES][MAX_LINK_LEN];
void get_serial_hyperlink_matrix(int H[NUM_SITES][NUM_SITES]);
void get_matrices(double google_matrix[NUM_SITES][NUM_SITES]);
void generate_matrices(int H[NUM_SITES][NUM_SITES], double G[NUM_SITES][NUM_SITES]);
void get_matrices(double google_matrix[NUM_SITES][NUM_SITES]);
void power_method(double matrix[NUM_SITES][NUM_SITES], int size, double pagerank[]);

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


// Get empty hyperlink matrix and fill it with values
void get_serial_hyperlink_matrix(int H[NUM_SITES][NUM_SITES]) {   

    // Site 1 - link to all other sites
    for (int i = 2; i <= 15; i++) {
        H[0][i - 1] = 1;
    }

    // Site 2 - links to sites 1,3
    H[1][0] = 1;
    H[1][2] = 1;

    // Site 3 - links to sites 1,2,5
    H[2][0] = 1;
    H[2][1] = 1;
    H[2][4] = 1;

    // Site 4 - links to site 1
    H[3][0] = 1;

    // Site 5 - links to sites 1,3
    H[4][0] = 1;
    H[4][2] = 1;

    // Site 6 - links to sites 1,3,4,5,8,10,14,15
    H[5][0] = 1;
    H[5][2] = 1;
    H[5][3] = 1;
    H[5][4] = 1;
    H[5][7] = 1;
    H[5][9] = 1;
    H[5][13] = 1;
    H[5][14] = 1;

    // Site 7 - links to sites 1, 2, 3, 4, 5, 8, 9, 11
    H[6][0] = 1;
    H[6][1] = 1;
    H[6][2] = 1;
    H[6][3] = 1;
    H[6][4] = 1;
    H[6][7] = 1;
    H[6][8] = 1;
    H[6][10] = 1;

    // Site 8 - links to sites 1, 6, 10, 14
    H[7][0] = 1;
    H[7][5] = 1;
    H[7][9] = 1;
    H[7][13] = 1;

    // Site 9 - no outgoing links

    // Site 10 - links to sites 1, 3, 4, 6, 8
    H[9][0] = 1;
    H[9][2] = 1;
    H[9][3] = 1;
    H[9][5] = 1;
    H[9][7] = 1;

    // site 11 - links to site 1
    H[10][0] = 1;

    // Site 12 - links to sites 1, 4, 9, 13
    H[11][0] = 1;
    H[11][3] = 1;
    H[11][8] = 1;
    H[11][12] = 1;
    
    // Site 13 - links to site 1
    H[12][0] = 1;

    // Site 14 - links to sites 1, 3, 8, 9
    H[13][0] = 1;
    H[13][2] = 1;
    H[13][7] = 1;
    H[13][8] = 1;

    // Site 15 - no outgoing links
}

const char LINKS_ARRAY[NUM_SITES][MAX_LINK_LEN] = {
    "www.wlu.ca",
    "webaccess.wlu.ca",
    "library.wlu.ca",
    "www.laurierathletics.com",
    "www.wlubookstore.com",
    "www.wlusu.com",
    "www.wlufa.ca",
    "www.wlusp.com",
    "www.wlupress.wlu.ca",
    "www.mylaurier.ca",
    "www.laurierconferences.ca",
    "www.laurieralumni.ca",
    "invest.wlu.ca",
    "www.wlugsa.ca",
    "www.lauriercc.ca"
};
