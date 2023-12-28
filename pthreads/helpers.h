typedef struct {
    int nr_of_cities;
    int begin;
    int end;

    int start_node;

    int *cities;

    int **optimal_paths;
    int *optimal_costs;

    int **cost_matrix;
} thread_struct_t;


void alloc_cost_matrix(int ***mat, int n);
void read_cost_matrix(int **mat, int n);
void free_cost_matrix(int ***mat, int n);

void *thread_function(void *arg);
void tsp_pthreads(int **cost_matrix, int start, int n, int thread_num);

int min(int a, int b);
void swap(int *a, int *b);
