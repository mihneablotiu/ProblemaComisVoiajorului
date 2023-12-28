#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "helpers.h"
#include "mpi.h"

#define INITIAL_CITY 0
#define MASTER 0
#define ONE 1

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s\n", "Trebuie sa existe exact 2 parametrii");
        exit(1);
    }

    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    int nr_of_cities;
    int initial_city = INITIAL_CITY;

    int **cost_matrix;

    if (rank == MASTER) {
        scanf("%d", &nr_of_cities);

        if (nr_of_cities < 1) {
            fprintf(stderr, "%s\n", "There has to be at least one city.");
            exit(1);
        }

        alloc_cost_matrix(&cost_matrix, nr_of_cities);
        read_cost_matrix(cost_matrix, nr_of_cities);

        MPI_Bcast(&nr_of_cities, ONE, MPI_INT, MASTER, MPI_COMM_WORLD);
        MPI_Bcast(&cost_matrix[0][0], nr_of_cities * nr_of_cities, MPI_INT, MASTER, MPI_COMM_WORLD);

        struct timespec start, finish;
        double elapsed;

        clock_gettime(CLOCK_MONOTONIC, &start);

        tsp_mpi_master(cost_matrix, initial_city, nr_of_cities, num_procs);
        
        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = finish.tv_sec - start.tv_sec;
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf("Total time: %lf seconds\n", elapsed);
    } else {
        MPI_Bcast(&nr_of_cities, ONE, MPI_INT, MASTER, MPI_COMM_WORLD);

        alloc_cost_matrix(&cost_matrix, nr_of_cities);
        MPI_Bcast(&cost_matrix[0][0], nr_of_cities * nr_of_cities, MPI_INT, MASTER, MPI_COMM_WORLD);

        tsp_mpi_worker(cost_matrix, initial_city, nr_of_cities);
    }

    free_cost_matrix(&cost_matrix, nr_of_cities);
    MPI_Finalize();
    return 0;
}
