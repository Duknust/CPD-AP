#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <iostream>

#define NUM_THREADS 4
#define NUM 20

int N=NUM;  // number of elements in array X
int X[NUM];
int gSum[NUM_THREADS];  // global storage for partial results
pthread_mutex_t locks[NUM];

void *Summation (void *pArg)
{
  int tNum = *((int *) pArg);
  
  std::cout << "tnum "<<tNum << "\n";
  int lSum = 0;
  int start, end;
  int i,j=0,k;

  for(j=0;j<NUM-1;j++){
    

    pthread_mutex_lock(&(locks[tNum*2+j*NUM_THREADS]));
    pthread_mutex_lock(&(locks[tNum*2+j*NUM_THREADS+1])); 
    X[tNum*2+j*NUM_THREADS+1]+=X[tNum*2+j*NUM_THREADS];
    //std::cout << tNum*2+j*NUM_THREADS << " "<< tNum*2+j*NUM_THREADS+1 << " ";
    pthread_mutex_unlock(&(locks[tNum*2+j*NUM_THREADS]));
    pthread_mutex_unlock(&(locks[tNum*2+j*NUM_THREADS+1]));
  }

  //if(tNum==0){
      std::cout <<"\n";
      for(i=0;i<NUM;i++){
        std::cout << X[i] << " ";
      }
    //}

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
  int j, sum = 0;
  pthread_t tHandles[NUM_THREADS];

  InitializeArray(X,&N);  // get values into A array; not shown
  for(j=0; j<NUM ;j++)
    pthread_mutex_init(&locks[j], NULL);

  for (j = 0; j < NUM_THREADS; j++) {
    int *threadNum = (int*)malloc(sizeof (int));
    *threadNum = j;
    pthread_create(&tHandles[j], NULL, Summation, (void *)threadNum);
  }
  for (j = 0; j < NUM_THREADS; j++) {
    pthread_join(tHandles[j], NULL);
  }
  printf("The sum of array elements is %d\n", X[N-1]);
  return 0;
}
