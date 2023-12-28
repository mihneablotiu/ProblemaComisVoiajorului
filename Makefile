CC=g++
CCMPI=mpic++
CFLAGS=-Wall -Werror -Wextra -pedantic
CMPIFLAGS=-Wall -Werror -pedantic -g

SERIAL_EXECUTABLE=serial_implementation
SERIAL=$(wildcard ./serial/*.c ./serial/*.cpp)

OPENMP_EXECUTABLE=openMP_implementation
OPENMP=$(wildcard ./openMP/*.c ./openMP/*.cpp)

PTHREAD_EXECUTABLE=pthread_implementation
PTHREAD=$(wildcard ./pthreads/*.c ./pthreads/*.cpp)

MPI_EXECUTABLE=mpi_implementation
MPI=$(wildcard ./mpi/*.c ./mpi/*.cpp)

MPI_OPENMP_EXECUTABLE=mpi_openmp_implementation
MPI_OPENMP=$(wildcard ./mpi_openMP/*.c ./mpi_openMP/*.cpp)

MPI_PTHREADS_EXECUTABLE=mpi_pthreads_implementation
MPI_PTHREADS=$(wildcard ./mpi_pthreads/*.c ./mpi_pthreads/*.cpp)

RUN_MPI=mpirun

TEST=test1
THREADS=24
NP=4

build: $(SERIAL_EXECUTABLE) $(OPENMP_EXECUTABLE) $(PTHREAD_EXECUTABLE) $(MPI_EXECUTABLE) $(MPI_OPENMP_EXECUTABLE) $(MPI_PTHREADS_EXECUTABLE)

serial_implementation: $(SERIAL)
	$(CC) $(CFLAGS) $^ -o $(SERIAL_EXECUTABLE)

openMP_implementation: $(OPENMP)
	$(CC) $(CFLAGS) $^ -o $(OPENMP_EXECUTABLE) -fopenmp

pthread_implementation: $(PTHREAD)
	$(CC) $(CFLAGS) $^ -o $(PTHREAD_EXECUTABLE) -lpthread

mpi_implementation: $(MPI)
	$(CCMPI) $(CMPIFLAGS) $^ -o $(MPI_EXECUTABLE)

mpi_openmp_implementation: $(MPI_OPENMP)
	$(CCMPI) $(CMPIFLAGS) $^ -o $(MPI_OPENMP_EXECUTABLE) -fopenmp

mpi_pthreads_implementation: $(MPI_PTHREADS)
	$(CCMPI) $(CMPIFLAGS) $^ -o $(MPI_PTHREADS_EXECUTABLE) -lpthread 

run_test_serial: $(SERIAL_EXECUTABLE)
	./$(SERIAL_EXECUTABLE) < ./tests/in/$(TEST).in

run_test_openMP: $(OPENMP_EXECUTABLE)
	./$(OPENMP_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in

run_test_pthreads: $(PTHREAD_EXECUTABLE)
	./$(PTHREAD_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in

run_test_mpi: $(MPI_EXECUTABLE)
	$(RUN_MPI) -np $(NP) --oversubscribe $(MPI_EXECUTABLE) < ./tests/in/$(TEST).in

run_test_mpi_openMP: $(MPI_OPENMP_EXECUTABLE)
	$(RUN_MPI) -np $(NP) --oversubscribe $(MPI_OPENMP_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in

run_test_mpi_pthreads: $(MPI_PTHREADS_EXECUTABLE)
	$(RUN_MPI) -np $(NP) --oversubscribe $(MPI_PTHREADS_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in

clean:
	rm $(SERIAL_EXECUTABLE)
	rm $(OPENMP_EXECUTABLE)
	rm $(PTHREAD_EXECUTABLE)
	rm $(MPI_EXECUTABLE)
	rm $(MPI_OPENMP_EXECUTABLE)
	rm $(MPI_PTHREADS_EXECUTABLE)
