typedef struct {
    int nr_of_cities;
    int begin;
    int end;

    int proc_begin;

    int start_node;

    int *cities;

    int **optimal_paths;
    int *optimal_costs;

    int **cost_matrix;
} thread_struct_t;

void alloc_cost_matrix(int ***mat, int n);
void read_cost_matrix(int **mat, int n);
void free_cost_matrix(int ***mat, int n);

void tsp_mpi_master(int **cost_matrix, int start, int n, int num_procs, int thread_num);
void tsp_mpi_worker(int **cost_matrix, int start, int n, int thread_num);

int min(int a, int b);
void swap(int *a, int *b);

void determine_minimum_cost(int **optimal_paths, int *optimal_costs,
                            int *optimal_cost, int *optimal_path,
                            int size, int elements_in_path);
