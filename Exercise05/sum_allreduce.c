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
    if ((rank == 0 && !array) || !local_chunk) MPI_Abort(MPI_COMM_WORLD, 1);
    if (rank == 0) for (int i = 0; i < N; i++) array[i] = i + 1;
    double start = MPI_Wtime();
    MPI_Scatter(array, chunk_size, MPI_INT,
                local_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; i++) local_sum += local_chunk[i];
    long long total_sum = 0;
    MPI_Allreduce(&local_sum, &total_sum, 1, MPI_LONG_LONG,
                  MPI_SUM, MPI_COMM_WORLD);
    double percentage = total_sum ? 100.0 * (double)local_sum / (double)total_sum : 0.0;
    printf("Rank %d: local_sum = %lld, total_sum = %lld, contribution = %.2f%%\n",
           rank, local_sum, total_sum, percentage);
    if (rank == 0) {
        long long expected = (long long)N * (N + 1) / 2;
        printf("[Allreduce] Total sum = %lld\n", total_sum);
        printf("[Allreduce] Expected  = %lld\n", expected);
        printf("[Allreduce] Correct?  = %s\n", total_sum == expected ? "YES" : "NO");
        printf("[Allreduce] Time      = %.6f sec\n", MPI_Wtime() - start);
    }
    free(local_chunk);
    free(array);
    MPI_Finalize();
    return 0;
}
