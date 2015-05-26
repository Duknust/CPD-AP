#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define DEBUG 0

int myrank;
int comm_sz;
int *num; 
int *den;

int gcd(int u, int v)
{
    if (v == 0) 
        return u;
    return gcd(v, u % v);
}

void FriendlyNumbers (int start, int end)
{
    int last = end-start+1;
    int *the_num = new int[last];
    if(myrank==0){
        num = new int[last*comm_sz];
        den = new int[last*comm_sz];
        if(DEBUG)
            printf("sou o %d, tenho um array de %d,start:%d,end:%d\n",myrank,last*comm_sz,start,end);
    }else{
        num = new int[last];
        den = new int[last]; 
        if(DEBUG) 
            printf("sou o %d, tenho um array de %d,start:%d,end:%d\n",myrank,last,start,end);
    }
#pragma omp parallel
 {  int i, j, factor, ii, sum, done, n;
    // -- MAP --
    #pragma omp for schedule (dynamic, 16)
    for (i = start; i <= end; i++) {
        ii = i - start;
        sum = 1 + i;
        the_num[ii] = i;
        done = i;
        factor = 2;
        while (factor < done) {
            if ((i % factor) == 0) {
                sum += (factor + (i/factor));
                if ((done = i/factor) == factor) sum -= factor;
            }
            factor++;
        }
        num[ii] = sum; den[ii] = i;
        n = gcd(num[ii], den[ii]);
        num[ii] /= n;
        den[ii] /= n;
    } // end for
 } // end parallel region
    //delete[] the_num;
    //delete[] num;
    //delete[] den;
 int i;
}

int main(int argc, char **argv)
{
    unsigned int start = 1;
    unsigned int end = 200;
    int i,j;
    int last = end-start+1;
    //int *num = new int[last];
    //int *den = new int[last];
    
    if(DEBUG)
        printf("size: %d\n",last);
    
    comm_sz=-1;
    myrank=-1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);  

    int space = last/comm_sz;
    FriendlyNumbers(myrank*space+start,(myrank+1)*space);

    if(myrank==0){ //0 den; 1 num
        for(i=1;i<comm_sz;i++){
            MPI_Recv(&den[i*space], space, MPI_INT, i, 0,
            MPI_COMM_WORLD, NULL);
            MPI_Recv(&num[i*space], space, MPI_INT, i, 1,
            MPI_COMM_WORLD, NULL);
            if(DEBUG){
                printf("recv_den:%d\n",last);
                for(i=0;i<last/comm_sz;i++){
                    printf("%d ",den[i]);
                }
                printf("\n");

                printf("recv_num:%d\n",last);
                for(i=0;i<last/comm_sz;i++){
                    printf("%d ",num[i]);
                }
                printf("\n");
            }
        }
        #pragma omp for schedule (static, 8)
        for (i = 0; i < last; i++) {
            for (j = i+1; j< last; j++) {
                if ((num[i] == num[j]) && (den[i] == den[j]))
                    printf ("%d and %d are FRIENDLY \n", start+i, start+j);
            }
        }
    }else{
        MPI_Send(&den[0], space, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&num[0], space, MPI_INT, 0, 1, MPI_COMM_WORLD);

        if(DEBUG){
            printf("send_den:%d\n",last);
            for(i=0;i<last/comm_sz;i++){
                printf("%d ",den[i]);
            }
            printf("\n");

            printf("send_num:%d\n",last);
            for(i=0;i<last/comm_sz;i++){
                printf("%d ",num[i]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
}