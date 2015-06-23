#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "mapreduce.h"
#include "keyvalue.h"

using namespace MAPREDUCE_NS;
int size, pid, nprocs, start, end;

typedef struct mapEntry{
    int key,value;
}MapEntry;

int gcd(int u, int v)
{
if (v == 0) return u;
return gcd(v, u % v);
}

// -- MAP --
void FriendlyNumbers(int itask, KeyValue *kv, void* ptr)
{
    int end=start+(offset*(pid+1)-1);
    int start+=(offset*(pid));
    
    int i, j, factor, ii, sum, done, n;

    for (i = start; i <= end; i++) {
        ii = i - start;
        sum = 1 + i;
        done = i;
        factor = 2;
        while (factor < done) {
            if ((i % factor) == 0) {
                sum += (factor + (i/factor));
                if ((done = i/factor) == factor) sum -= factor;
            }
            factor++;
        }
        num = sum; den = i;
        n = gcd(num, den);
        num /= n;
        den /= n;
        MapEntry me;
        me.key = num;
        me.value = den;
        int toSave = i;
        kv->add((char*)&me,sizeof(MapEntry),(char*)&toSave,sizeof(int))
    }
}

// -- REDUCE --
void results(char *key, int keybytes, char *multivalue, int nvalues, int *valuebytes, KeyValue *kv, void *ptr){
    int i=0;
    if(nvalues){
        for (; i < nvalues; i++) {
            printf("%d ",multivalue[i]);
        }
        printf("\n");
    }
}


int main(int argc, char **argv)
{
    start = atoi(argv[1]);
    end = atoi(argv[2]);
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    double time=MPI_Wtime();
    
    size = end-start+1;
    int block = size / nprocs;
    
    MapReduce *mr = new MapReduce(MPI_COMM_WORLD);
    mr->verbosity=1;
    mr->timer=1;
    MPI_Barrier(MPI_COMM_WORLD);
    
    int nTasks = mr->map(nprocs,&FriendlyNumbers);
    mr->collate(NULL);
    mr->reduce(results,NULL);
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(pid==0)
        printf("Time:%lf seconds\n",time=MPI_Wtime()-time);
    
    delete mr;
    MPI_Finalize();
}
