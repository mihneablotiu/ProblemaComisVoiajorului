#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

void alloc_cost_matrix(int ***mat, int n) {
    (*mat) = (int **) malloc(n * sizeof(**mat));
    if (!(*mat)) {
        fprintf(stderr, "%s\n", "Malloc failed");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        (*mat)[i] = (int *) malloc(n * sizeof(*(*mat)[i]));
        if (!(*mat)[i]) {
            fprintf(stderr, "%s\n", "Malloc failed");
            exit(1);
        }
    }
}

void read_cost_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
}

void free_cost_matrix(int ***mat, int n) {
    for (int i = 0; i < n; i++) {
        free((*mat)[i]);
    }

    free(*mat);
    mat = NULL;
}

void tsp_serial(int **cost_matrix, int start, int n) {
    vector<int> cities, optimal_path;
    for (int i = 0; i < n; i++) {
        if (i != start) {
            cities.push_back(i);
        }
    }
 
    int optimal_cost = INT_MAX;

    do {
        int current_cost = 0;
 
        int current_city = start;
        for (unsigned long int i = 0; i < cities.size(); i++) {
            current_cost += cost_matrix[current_city][cities[i]];
            current_city = cities[i];
        }

        current_cost += cost_matrix[current_city][start];
 
        if (current_cost < optimal_cost) {
            optimal_cost = current_cost;

            optimal_path.clear();

            optimal_path.push_back(start);

            for (unsigned long int i = 0; i < cities.size(); i++) {
                optimal_path.push_back(cities[i]);
            }

            optimal_path.push_back(start);
        }
    } while (next_permutation(cities.begin(), cities.end()));
 
    printf("Path: ");

    if (n > 1) {
        for (int i = 0; i < n + 1; i++) {
            printf("%d ", optimal_path[i] + 1);
        }
    } else {
        printf("1");
    }

    printf("\n");
    printf("Minimum cost: %d\n", optimal_cost);
}
