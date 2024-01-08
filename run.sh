#!/bin/bash

make build

THREADS=24
PROCS=24

# for i in {1..14}; do
#    total_time=0.0
#    last_path=""
#    last_minimum_cost=""

#    for j in {1..3}; do
#        output=$(./serial_implementation < tests/in/test${i}.in)
       
#        elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
#        last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
#        last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

#        total_time=$(echo "$total_time + $elapsed_time" | bc)
#    done
   
#    mean_total_time=$(echo "scale=6; $total_time / 5 " | bc)
   
#    if [[ "$mean_total_time" == .* ]]; then
#        mean_total_time="0$mean_total_time"
#    fi
   
#    echo "Path: ${last_path}" > tests/out_serial/test${i}.out
#    echo "Minimum cost: ${last_minimum_cost}" >> tests/out_serial/test${i}.out
#    echo "Mean total time: ${mean_total_time} seconds" >> tests/out_serial/test${i}.out
#    echo "Test ${i} done - serial"
# done

# for i in {1..14}; do
#     total_time=0.0
#     last_path=""
#     last_minimum_cost=""

#     for j in {1..3}; do
#         output=$(./openMP_implementation ${THREADS} < tests/in/test${i}.in)
    
#         elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
#         last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
#         last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

#         total_time=$(echo "$total_time + $elapsed_time" | bc)
#     done

#     mean_total_time=$(echo "scale=6; $total_time / 3" | bc)

#     if [[ "$mean_total_time" == .* ]]; then
#         mean_total_time="0$mean_total_time"
#     fi

#     echo "Path: ${last_path}" > tests/out_openMP/test${i}.out.${THREADS}.average
#     echo "Minimum cost: ${last_minimum_cost}" >> tests/out_openMP/test${i}.out.${THREADS}.average
#     echo "Mean total time: ${mean_total_time} seconds" >> tests/out_openMP/test${i}.out.${THREADS}.average
#     echo "Test ${i} done - openMP"
# done

# for i in {1..14}; do
#     total_time=0.0
#     last_path=""
#     last_minimum_cost=""

#     for j in {1..3}; do
#         output=$(./pthread_implementation ${THREADS} < tests/in/test${i}.in)
        
#         elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
#         last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
#         last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

#         total_time=$(echo "$total_time + $elapsed_time" | bc)
#     done
    
#     mean_total_time=$(echo "scale=6; $total_time / 3" | bc)
    
#     if [[ "$mean_total_time" == .* ]]; then
#         mean_total_time="0$mean_total_time"
#     fi
    
#     echo "Path: ${last_path}" > tests/out_pthreads/test${i}.out.${THREADS}.average
#     echo "Minimum cost: ${last_minimum_cost}" >> tests/out_pthreads/test${i}.out.${THREADS}.average
#     echo "Mean total time: ${mean_total_time} seconds" >> tests/out_pthreads/test${i}.out.${THREADS}.average
#     echo "Test ${i} done - pthreads"
# done

for i in {1..14}; do
    total_time=0.0
    last_path=""
    last_minimum_cost=""

    for j in {1..3}; do
        output=$(mpirun -np ${PROCS} --oversubscribe mpi_implementation < tests/in/test${i}.in)
        
        elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
        last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
        last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

        total_time=$(echo "$total_time + $elapsed_time" | bc)
    done
    
    mean_total_time=$(echo "scale=6; $total_time / 3" | bc)
    
    if [[ "$mean_total_time" == .* ]]; then
        mean_total_time="0$mean_total_time"
    fi
    
    echo "Path: ${last_path}" > tests/out_mpi/test${i}.out.${PROCS}.average
    echo "Minimum cost: ${last_minimum_cost}" >> tests/out_mpi/test${i}.out.${PROCS}.average
    echo "Mean total time: ${mean_total_time} seconds" >> tests/out_mpi/test${i}.out.${PROCS}.average
    echo "Test ${i} done - MPI"
done

# for i in {1..15}; do
#     total_time=0.0
#     last_path=""
#     last_minimum_cost=""

#     for j in {1..10}; do
#         output=$(mpirun -np 6 --oversubscribe mpi_openmp_implementation < tests/in/test${i}.in)
        
#         elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
#         last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
#         last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

#         total_time=$(echo "$total_time + $elapsed_time" | bc)
#     done
    
#     mean_total_time=$(echo "scale=6; $total_time / 20" | bc)
    
#     if [[ "$mean_total_time" == .* ]]; then
#         mean_total_time="0$mean_total_time"
#     fi
    
#     echo "Path: ${last_path}" > tests/out_mpi_openMP/test${i}.out
#     echo "Minimum cost: ${last_minimum_cost}" >> tests/out_mpi_openMP/test${i}.out
#     echo "Mean total time: ${mean_total_time} seconds" >> tests/out_mpi_openMP/test${i}.out
#     echo "Test ${i} done - MPI_OPENMP"
# done

# for i in {1..15}; do
#     total_time=0.0
#     last_path=""
#     last_minimum_cost=""

#     for j in {1..10}; do
#         output=$(mpirun -np 5 --oversubscribe mpi_pthreads_implementation < tests/in/test${i}.in)
        
#         elapsed_time=$(echo "$output" | awk '/Total time:/ { print $3 }')
#         last_path=$(echo "$output" | awk '/Path:/ { $1=""; print $0 }')
#         last_minimum_cost=$(echo "$output" | awk '/Minimum cost:/ { print $3 }')

#         total_time=$(echo "$total_time + $elapsed_time" | bc)
#     done
    
#     mean_total_time=$(echo "scale=6; $total_time / 20" | bc)
    
#     if [[ "$mean_total_time" == .* ]]; then
#         mean_total_time="0$mean_total_time"
#     fi
    
#     echo "Path: ${last_path}" > tests/out_mpi_pthreads/test${i}.out
#     echo "Minimum cost: ${last_minimum_cost}" >> tests/out_mpi_pthreads/test${i}.out
#     echo "Mean total time: ${mean_total_time} seconds" >> tests/out_mpi_pthreads/test${i}.out
#     echo "Test ${i} done - MPI_PTHREADS"
# done

make clean
