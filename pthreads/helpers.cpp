#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "helpers.h"
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

int min(int a, int b) {
    if (a < b) {
        return a;
    }

    return b;
}

void *thread_function(void *arg) {
    thread_struct_t *current_struct = (thread_struct_t *) arg;
    int begin = current_struct->begin;
    int end = current_struct->end;
    int n = current_struct->nr_of_cities;
    int **cost_matrix = current_struct->cost_matrix;
    int *optimal_costs = current_struct->optimal_costs;
    int **optimal_paths = current_struct->optimal_paths;
    int *cities = current_struct->cities;
    int start_node = current_struct->start_node;

    for (int i = begin; i < end; i++) {
        vector<int> thread_cities;
        for (int j = 0; j < (n - 1); j++) {
            thread_cities.push_back(cities[j]);
        }

        swap(thread_cities[0], thread_cities[i]);
        sort(thread_cities.begin() + 1, thread_cities.end());

        int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));
        int optimal_cost = INT_MAX;
        
        do {
            int current_cost = 0;
 
            int current_city = start_node;
            for (unsigned long int i = 0; i < thread_cities.size(); i++) {
                current_cost += cost_matrix[current_city][thread_cities[i]];
                current_city = thread_cities[i];
            }

            current_cost += cost_matrix[current_city][start_node];
    
            if (current_cost < optimal_cost) {
                optimal_cost = current_cost;

                optimal_path[0] = start_node;

                for (unsigned long int i = 0; i < thread_cities.size(); i++) {
                    optimal_path[i + 1] = thread_cities[i];
                }

                optimal_path[n] = start_node;

                optimal_costs[i] = optimal_cost;
                optimal_paths[i] = optimal_path;
            } 
        } while (std::next_permutation(thread_cities.begin() + 1, thread_cities.end()));
    }

    return NULL;
}

void tsp_pthreads(int **cost_matrix, int start, int n, int thread_num) {
    int *cities = (int *) malloc((n - 1) * sizeof(*cities));

    for (int i = 0, j = 0; i < n; i++) {
        if (i != start) {
            cities[j] = i;
            j++;
        }
    }

    int **optimal_paths = (int **) malloc((n - 1) * sizeof(*optimal_paths));
    int *optimal_costs = (int *) malloc((n - 1) * sizeof(*optimal_costs));

    int num_threads = thread_num;
    pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(*threads));

    int response;
    void *status;
    thread_struct_t *thread_structs = (thread_struct_t *) malloc(num_threads * sizeof(*thread_structs));

    for (int i = 0; i < num_threads; i++) {
        thread_structs[i].cost_matrix = cost_matrix;
        thread_structs[i].optimal_costs = optimal_costs;
        thread_structs[i].optimal_paths = optimal_paths;
        thread_structs[i].cities = cities;
        thread_structs[i].nr_of_cities = n;
        thread_structs[i].start_node = start;

        thread_structs[i].begin = i * (double) (n - 1) / num_threads;
        thread_structs[i].end = min((i + 1) * (double) (n - 1) / num_threads, (n - 1));

        response = pthread_create(&threads[i], NULL, thread_function, (void *) &thread_structs[i]);

        if (response) {
            fprintf(stderr, "%s\n", "Eroare la crearea thread-urilor");
            exit(1);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        response = pthread_join(threads[i], &status);

        if (response) {
            fprintf(stderr, "%s\n", "Eroare la asteptarea thread-urilor");
            exit(1);
        }
    }
 
    printf("Path: ");

    if (n > 1) {
        int minimum_cost_position;
        int minimum_cost = INT_MAX;

        for (int i = 0; i < (n - 1); i++) {
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

        for (int i = 0; i < (n - 1); i++) {
            free(optimal_paths[i]);
        }
    } else {
        printf("1\n");
        printf("Minimum cost: 0\n");
    }

    free(optimal_paths);
    free(optimal_costs);
    free(threads);
    free(thread_structs);
    free(cities);
}
