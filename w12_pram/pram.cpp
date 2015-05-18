#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <semaphore.h>
#include <iostream>

#define NUM_THREADS 4
#define NUM 20

int N=NUM;  // number of elements in array X
int X[NUM];//={3,5,2,5,7,9,-4,6,7,-3,1,7,6,8,-1,2};
int inTotals[NUM_THREADS];  // global storage for partial results
int outTotals[NUM_THREADS];
pthread_mutex_t locks[NUM_THREADS];
sem_t semaforo[NUM_THREADS], semaforo2[NUM_THREADS];

void printArray(int *array, int size){
  for(int i=0; i<size; i++){
    printf(" %d ",array[i]);
  }
  printf("\n ");
}

void *Summation (void *pArg)
{
  int tNum = *((int *) pArg);
  int size = N/NUM_THREADS;
  int start=tNum*size, end=(tNum+1)*size;
  int j=0;


  for(j=start+1;j<end;j++){
    X[j]+=X[j-1];    
  }

  inTotals[tNum]=X[end-1];

  printf("numth_%d_start_%d_end_%d_mytotal_%d\n",tNum,start,end,inTotals[tNum]);

  sem_post(&semaforo2[tNum]);
  sem_wait(&semaforo[tNum]);

  for (j=start; j<end; j++){
    X[j]+=outTotals[tNum];
  }

  return 0;
}

void InitializeArray(int *S, int *N)
{
  int i;
  for (i = 0; i < NUM; i++){
    S[i] = i+1;
  }
}

int main(int argc, char* argv[])
{
  int j;
  pthread_t tHandles[NUM_THREADS];

  InitializeArray(X,&N);  // get values into A array; not shown
  for(j=0;j<NUM_THREADS;j++){
    sem_init(&semaforo[j], 0, 0);
    sem_init(&semaforo2[j], 0, 0);
  }

  for (j = 0; j < NUM_THREADS; j++) {
    int *threadNum = (int*)malloc(sizeof (int));
    *threadNum = j;
    pthread_create(&tHandles[j], NULL, Summation, (void *)threadNum);
  }

  printf("x\n");
  printArray(X,NUM);
  printf("\n");


  for(j=0;j<NUM_THREADS;j++){
    sem_wait(&semaforo2[j]);
  }

  printf("inTotals\n");
  printArray(inTotals,NUM_THREADS);
  printf("\n");  

  outTotals[0]=0;
  for(j=1; j<NUM_THREADS;j++){
    outTotals[j]=inTotals[j-1]+outTotals[j-1];
  }

  printf("outTotals\n");
  printArray(outTotals,NUM_THREADS);
  printf("\n");  

  for (j=0;j<NUM_THREADS;j++){ //threads podem voltar a trabalhar
    sem_post(&semaforo[j]); 
  }

  for (j = 0; j < NUM_THREADS; j++) {
    pthread_join(tHandles[j], NULL);
  }
  printf("The sum of array elements is %d\n", X[N-1]);
  for(j=0;j<NUM_THREADS;j++){
    sem_destroy(&semaforo[j]); 
    sem_destroy(&semaforo2[j]);
  }
  return 0;
}


