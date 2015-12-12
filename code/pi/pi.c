#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int nprocs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nintervals;
    double time_start;
    if (rank == 0) {
        printf("Enter number of intervals: \n");
        scanf("%d", &nintervals);
        time_start = MPI_Wtime();
    }
    MPI_Bcast(&nintervals, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double h = 1.0 / (double)nintervals;
    double sum = 0.0;
    for (int i = rank; i < nintervals; i += nprocs) {
        double x = h * ((double)i + 0.5);
        sum += 4.0 / (1.0 + x * x);
    }
    double my_pi = h * sum;

    double pi;
    MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double duration = MPI_Wtime() - time_start;
        double error = fabs(pi - 4.0 * atan(1.0));
        printf("pi=%e, error=%e, duration=%e\n", pi, error, duration);
    }

    MPI_Finalize();

    return 0;
}