#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
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

void swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void tsp_openMP(int **cost_matrix, int start, int n) {
    vector<int> cities;
    for (int i = 0; i < n; i++) {
        if (i != start) {
            cities.push_back(i);
        }
    }

    int **optimal_paths = (int **) malloc(cities.size() * sizeof(*optimal_paths));
    int *optimal_costs = (int *) malloc(cities.size() * sizeof(*optimal_costs));

    #pragma omp parallel for schedule(static)
    for (unsigned long int i = 0; i < cities.size(); i++) {
        vector<int> thread_cities;
        for (int city : cities) {
            thread_cities.push_back(city);
        }

        swap(thread_cities[0], thread_cities[i]);
        sort(thread_cities.begin() + 1, thread_cities.end());

        int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));
        int optimal_cost = INT_MAX;
        
        do {
            int current_cost = 0;
 
            int current_city = start;
            for (unsigned long int i = 0; i < thread_cities.size(); i++) {
                current_cost += cost_matrix[current_city][thread_cities[i]];
                current_city = thread_cities[i];
            }

            current_cost += cost_matrix[current_city][start];
    
            if (current_cost < optimal_cost) {
                optimal_cost = current_cost;

                optimal_path[0] = start;

                for (unsigned long int i = 0; i < thread_cities.size(); i++) {
                    optimal_path[i + 1] = thread_cities[i];
                }

                optimal_path[n] = start;
            } 
        } while (std::next_permutation(thread_cities.begin() + 1, thread_cities.end()));

        optimal_costs[i] = optimal_cost;
        optimal_paths[i] = optimal_path;
    }
 
    printf("Path: ");

    if (n > 1) {
        int minimum_cost_position;
        int minimum_cost = INT_MAX;

        for (long unsigned int i = 0; i < cities.size(); i++) {
            if (optimal_costs[i] < minimum_cost) {
                minimum_cost = optimal_costs[i];
                minimum_cost_position = i;
            }
        }

        for (int i = 0; i < n + 1; i++) {
            printf("%d ", optimal_paths[minimum_cost_position][i] + 1);
        }

        printf("\n");
        printf("Minimum cost: %d\n", minimum_cost);

        for (long unsigned int i = 0; i < cities.size(); i++) {
            free(optimal_paths[i]);
        }
    } else {
        printf("1\n");
        printf("Minimum cost: 0\n");
    }

    free(optimal_paths);
    free(optimal_costs);
}
