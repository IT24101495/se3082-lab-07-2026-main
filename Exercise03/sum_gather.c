#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1000000
int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (N % size != 0) {
        if (rank == 0) fprintf(stderr, "Process count must divide %d\n", N);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int chunk_size = N / size;
    int *array = rank == 0 ? malloc((size_t)N * sizeof(int)) : NULL;
    int *local_chunk = malloc((size_t)chunk_size * sizeof(int));
    long long *all_sums = rank == 0 ? malloc((size_t)size * sizeof(long long)) : NULL;
    if ((rank == 0 && (!array || !all_sums)) || !local_chunk)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if (rank == 0) for (int i = 0; i < N; i++) array[i] = i + 1;
    double start = MPI_Wtime();
    MPI_Scatter(array, chunk_size, MPI_INT,
                local_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; i++) local_sum += local_chunk[i];
    printf("Rank %d: local_sum = %lld\n", rank, local_sum);
    MPI_Gather(&local_sum, 1, MPI_LONG_LONG,
               all_sums, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        long long total_sum = 0;
        for (int i = 0; i < size; i++) total_sum += all_sums[i];
        long long expected = (long long)N * (N + 1) / 2;
        printf("[Gather] Total sum = %lld\n", total_sum);
        printf("[Gather] Expected  = %lld\n", expected);
        printf("[Gather] Correct?  = %s\n", total_sum == expected ? "YES" : "NO");
        printf("[Gather] Time      = %.6f sec\n", MPI_Wtime() - start);
    }
    free(all_sums);
    free(local_chunk);
    free(array);
    MPI_Finalize();
    return 0;
}
