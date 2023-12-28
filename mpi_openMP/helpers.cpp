#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "helpers.h"
#include "mpi.h"

#define ONE 1
#define MASTER 0
#define TAG 0

using namespace std;

void alloc_cost_matrix(int ***mat, int n) {
    (*mat) = (int **) malloc(n * sizeof(**mat));
    if (!(*mat)) {
        fprintf(stderr, "%s\n", "Malloc failed");
        exit(1);
    }

    (*mat)[0] = (int *) malloc(n * n * sizeof(*(*mat)[0]));
    if (!(*mat)[0]) {
        fprintf(stderr, "%s\n", "Malloc failed");
        exit(1);
    }

    for (int i = 1; i < n; i++) {
        (*mat)[i] = (*mat)[0] + i * n;
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
    free((*mat)[0]);
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

void determine_minimum_cost(int **optimal_paths, int *optimal_costs,
                            int *optimal_cost, int *optimal_path,
                            int size, int elements_in_path) {

    int minimum_cost = INT_MAX;
    int minimum_cost_position;

    for (int i = 0; i < size; i++) {
        if (optimal_costs[i] < minimum_cost) {
            minimum_cost = optimal_costs[i];
            minimum_cost_position = i;
        }
    }

    *optimal_cost = minimum_cost;

    for (int i = 0; i < elements_in_path; i++) {
        optimal_path[i] = optimal_paths[minimum_cost_position][i];
    }
}

void tsp_mpi_worker(int **cost_matrix, int start, int n) {
    int begin, end;

    int *cities = (int *) malloc((n - 1) * sizeof(*cities));

    for (int i = 0, j = 0; i < n; i++) {
        if (i != start) {
            cities[j] = i;
            j++;
        }
    }

    MPI_Recv(&begin, ONE, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&end, ONE, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int size = end - begin;

    int **optimal_paths = (int **) malloc(size * sizeof(*optimal_paths));
    int *optimal_costs = (int *) malloc(size * sizeof(*optimal_costs));

    #pragma omp parallel for schedule(static)
    for (int i = begin; i < end; i++) {
        int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));
        int optimal_cost = INT_MAX;

        vector<int> thread_cities;
        for (int j = 0; j < (n - 1); j++) {
            thread_cities.push_back(cities[j]);
        }

        swap(thread_cities[0], thread_cities[i]);
        sort(thread_cities.begin() + 1, thread_cities.end());
        
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

        optimal_paths[i - begin] = optimal_path;
        optimal_costs[i - begin] = optimal_cost;
    }

    int optimal_cost = INT_MAX;
    int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));

    if (size > 0) {
        determine_minimum_cost(optimal_paths, optimal_costs, &optimal_cost, optimal_path, size, n + 1);
        
        for (int i = 0; i < size; i++) {
            free(optimal_paths[i]);
        }

        free(optimal_paths);
        free(optimal_costs);
    }

    MPI_Send(&optimal_cost, ONE, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
    MPI_Send(optimal_path, n + 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);

    free(optimal_path);
    free(cities);
}

void tsp_mpi_master(int **cost_matrix, int start, int n, int num_procs) {
    int *cities = (int *) malloc((n - 1) * sizeof(*cities));

    for (int i = 0, j = 0; i < n; i++) {
        if (i != start) {
            cities[j] = i;
            j++;
        }
    }

    for (int i = 1; i < num_procs; i++) {
        int begin = i * (double) (n - 1) / num_procs;
        int end = min((i + 1) * (double) (n - 1) / num_procs, (n - 1));

        MPI_Send(&begin, ONE, MPI_INT, i, TAG, MPI_COMM_WORLD);
        MPI_Send(&end, ONE, MPI_INT, i, TAG, MPI_COMM_WORLD);
    }

    int my_begin = 0;
    int my_end = min((double) (n - 1) / num_procs, (n - 1));
    int size = my_end - my_begin;

    int **optimal_paths = (int **) malloc(size * sizeof(*optimal_paths));
    int *optimal_costs = (int *) malloc(size * sizeof(*optimal_costs));

    #pragma omp parallel for schedule(static)
    for (int i = my_begin; i < my_end; i++) {
        int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));
        int optimal_cost = INT_MAX;

        vector<int> thread_cities;
        for (int j = 0; j < (n - 1); j++) {
            thread_cities.push_back(cities[j]);
        }

        swap(thread_cities[0], thread_cities[i]);
        sort(thread_cities.begin() + 1, thread_cities.end());
        
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

        optimal_paths[i - my_begin] = optimal_path;
        optimal_costs[i - my_begin] = optimal_cost;
    }

    int optimal_cost = INT_MAX;
    int *optimal_path = (int *) malloc((n + 1) * sizeof(*optimal_path));

    if (size > 0) {
        determine_minimum_cost(optimal_paths, optimal_costs, &optimal_cost, optimal_path, size, n + 1);

        for (int i = 0; i < size; i++) {
            free(optimal_paths[i]);
        }

        free(optimal_paths);
        free(optimal_costs);
    }

    int current_minimal_cost = INT_MAX;
    int *current_minimal_path = (int *) malloc((n + 1) * sizeof(*current_minimal_path));
    for (int i = 1; i < num_procs; i++) {
        MPI_Recv(&current_minimal_cost, ONE, MPI_INT, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(current_minimal_path, n + 1, MPI_INT, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (current_minimal_cost < optimal_cost) {
            optimal_cost = current_minimal_cost;

            for (int i = 0; i < n + 1; i++) {
                optimal_path[i] = current_minimal_path[i];
            }
        }
    }
 
    printf("Path: ");

    if (n > 1) {
        for (int i = 0; i < n + 1; i++) {
            printf("%d ", optimal_path[i] + 1);
        }

        printf("\n");
        printf("Minimum cost: %d\n", optimal_cost);
    } else {
        printf("1\n");
        printf("Minimum cost: 0\n");
    }

    free(cities);
    free(current_minimal_path);
    free(optimal_path);
}
