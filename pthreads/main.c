#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "helpers.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s\n", "Trebuie sa existe exact 2 parametrii");
        exit(1);
    }

    int num_threads = atoi(argv[1]);

    int nr_of_cities;
    scanf("%d", &nr_of_cities);

    if (nr_of_cities < 1)
    {
        fprintf(stderr, "%s\n", "There has to be at least one city.");
        exit(1);
    }

    int initial_city = 0;

    int **cost_matrix;
    alloc_cost_matrix(&cost_matrix, nr_of_cities);
    read_cost_matrix(cost_matrix, nr_of_cities);

    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    tsp_pthreads(cost_matrix, initial_city, nr_of_cities, num_threads);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = finish.tv_sec - start.tv_sec;
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Total time: %lf seconds\n", elapsed);
    free_cost_matrix(&cost_matrix, nr_of_cities);

    return 0;
}
