
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//#define N 100

int *A;

void OESort_parallel(int NN, int *A)
{
    int exch = 1, start = 0, i;
    #pragma omp parallel
    {
        int temp;
        exch = omp_get_num_threads();
        while (1) {
            if(exch <= 0 && start == 0)
                break;
            #pragma omp critical
            exch--;
            #pragma omp barrier
            #pragma omp for
            for (i = start; i < NN-1; i+=2) {
                if (A[i] > A[i+1]) {
                    temp = A[i]; A[i] = A[i+1]; A[i+1] = temp;
                    #pragma omp critical
                    exch = omp_get_num_threads();
                    //printf("alterei %d\n",exch);
                }
            }
            #pragma omp single
            if (start == 0) start = 1;
            else start = 0;
        }
    }
}

void OESort(int NN, int *A)
{
    int exch = 1, start = 0, i;
    int temp;

    while (exch || start) {
        exch = 0;
        for (i = start; i < NN-1; i+=2) {
            if (A[i] > A[i+1]) {
                temp = A[i];
                A[i] = A[i+1];
                A[i+1] = temp;
                exch = 1;
            }
        }
        if (start == 0) start = 1;
        else start = 0;
    }
}

void init_data(int N)
{
    int i;
    for (i = 0; i < N/2; i++)
        A[i] = i + N/2;
    //  A[i] = rand() % N;
    for (i = N/2; i < N; i++)
        A[i] = i - N/2;

}

int main(int argc, char* argv[])
{   
    int i, j;

    A = (int*)malloc(sizeof(int)*atoi(argv[1]))
    init_data(atoi(argv[1]));

    //for ( i = 0; i < N; i++) printf("%3d ",A[i]);
    //printf("\n\n");
    double start_time = omp_get_wtime();
    OESort_parallel(atoi(argv[1]),A);
    //OESort(N,A);
    double tempo = omp_get_wtime() - start_time;
    printf ("Tempo: %g\n",tempo);
    //for ( j = 0; j < N; j++) printf("%3d ",A[j]);
    return 0;
}
