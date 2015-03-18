
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//#define N 100

int A[N];

void OESort_parallel(int NN, int *A)
{
    int exchO, exchE = 1, turns = 0, i;
    
    while (exchE) {
        exchO = 0;
        exchE = 0;

        #pragma omp parallel
        {
            int temp;
            #pragma omp for
            for (i = 0; i < NN-1; i+=2) {
                if (A[i] > A[i+1]) {
                    temp = A[i];
                    A[i] = A[i+1];
                    A[i+1] = temp;
                    exchO = 1;
                }
            }
            if(exchO || !turns){
                #pragma omp for
                for (i = 1; i < NN-1; i+=2) {
                    if (A[i] > A[i+1]) {
                        temp = A[i];
                        A[i] = A[i+1];
                        A[i+1] = temp;
                        exchE = 1;
                    }
                }
            }
        }
        turns=1;
    }
}

void OESort(int NN, int *A)
{
    int exch = 1, start = 0, i;
    int temp;

    while (exch || start) {
        exch = 0;
        for (i = 0; i < NN-1; i+=2) {
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

void init_data()
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

    init_data();

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
