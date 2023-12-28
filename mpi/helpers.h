void alloc_cost_matrix(int ***mat, int n);
void read_cost_matrix(int **mat, int n);
void free_cost_matrix(int ***mat, int n);

void tsp_mpi_master(int **cost_matrix, int start, int n, int num_procs);
void tsp_mpi_worker(int **cost_matrix, int start, int n);

int min(int a, int b);
void swap(int *a, int *b);
