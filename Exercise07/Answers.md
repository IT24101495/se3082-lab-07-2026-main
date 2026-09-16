
### Table 1: Comparison of MPI Programs

| Program | Communication Operations | Array Memory Allocation | Manual Summation on Root? | Final Result Available |
|---|---|---|---|---|
| Broadcast | MPI_Bcast + MPI_Send/MPI_Recv | Full array on all processes | Yes | Root only |
| Scatter | MPI_Scatter + MPI_Send/MPI_Recv | Full array on root; local chunks on all processes | Yes | Root only |
| Gather | MPI_Scatter + MPI_Gather | Full array on root; local chunks on all processes | Yes | Root only |
| Reduce | MPI_Scatter + MPI_Reduce | Full array on root; local chunks on all processes | No | Root only |
| Allreduce | MPI_Scatter + MPI_Allreduce | Full array on root; local chunks on all processes | No | All processes |
| Scan | MPI_Scatter + MPI_Scan | Full array on root; local chunks on all processes | No | Different prefix sum for each rank |


### Table 2: Execution Time Comparison

| Program | 2 Processes (sec) | 4 Processes (sec) | 8 Processes (sec) |
|---|---:|---:|---:|
| Broadcast | 0.006519 | 0.016684 | 0.007714 |
| Scatter | 0.003891 | 0.002744 | 0.002829 |
| Gather | **0.002644** | **0.002159** | 0.003328 |
| Reduce | 0.003556 | 0.005587 | **0.002023** |
| Allreduce | 0.004916 | 0.002226 | 0.002913 |
| Scan | 0.006741 | 0.011992 | 0.008338 |



Based on the recorded execution times, MPI_Gather was the fastest approach with 2 processes (0.002644 seconds) and 4 processes (0.002159 seconds). MPI_Reduce was the fastest with 8 processes (0.002023 seconds).

Broadcast generally showed higher execution times because it distributes the entire array to every process, even though each process only requires a portion. Scatter-based approaches reduce unnecessary communication by distributing only the required data chunks.

Increasing the number of processes did not consistently decrease execution time. This may be due to communication overhead, process scheduling, and system resource limitations. Repeated measurements would provide a more reliable comparison.




### Thinking Question:

MPI_Scan is preferred when each process needs a cumulative result based on its own value and the values of all preceding processes. In contrast, MPI_Allreduce provides the same global result to every process.

For example, in a parallel transaction-processing system, each process may calculate the sum of its assigned transactions. MPI_Scan can calculate the cumulative transaction total up to each process, allowing each process to determine its position in the overall sequence.

Therefore, MPI_Scan is useful for prefix sums, cumulative calculations, and determining offsets in parallel applications.
