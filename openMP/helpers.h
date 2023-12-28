void alloc_cost_matrix(int ***mat, int n);
void read_cost_matrix(int **mat, int n);
void free_cost_matrix(int ***mat, int n);

void tsp_openMP(int **cost_matrix, int start, int n);

void swap(int *a, int *b);
