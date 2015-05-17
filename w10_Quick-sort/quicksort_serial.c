#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100000000

int *array;


void quicksort(int lo, int hi)
{
    int i=lo,j=hi,h;
    int x=array[(lo+hi)/2];
    
    //partition
    do
    {    
        while(array[i]<x) i++; 
        while(array[j]>x) j--;
        if(i<=j)
        {
            h=array[i]; array[i]=array[j]; array[j]=h;
            i++; j--;
        }
    } while(i<=j);
    
    //recursion
    if(lo<j) quicksort(lo,j);
    if(i<hi) quicksort(i,hi);
}

void initialize() {
    array = (int *) malloc(SIZE*sizeof(int));
    
    srand(0);
    
    for(int i=0; i<SIZE; i++) {
        array[i]=rand();
    }
}

void validate() {
    int error=0;
    for(int i=0;i<SIZE&&!error;i++)
        if(array[i-1]>array[i]) error=1;
    
    if(error) printf("Error\n");
    else printf("Ok\n");
}


int main() {
    
    initialize();
    
    double time = omp_get_wtime();
    quicksort(0,SIZE-1);
    printf("Time=%f\n", omp_get_wtime()-time);
    
    validate();
    
}