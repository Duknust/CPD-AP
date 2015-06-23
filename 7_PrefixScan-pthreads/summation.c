#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM 20

int N=NUM;  // number of elements in array X
int X[NUM];
int gSum[NUM_THREADS];  // global storage for partial results

void *Summation (void *pArg)
{
  int tNum = *((int *) pArg);
  int lSum = 0;
  int start, end;
  int i;

  start = (N / NUM_THREADS) * tNum;
  end = (N / NUM_THREADS) * (tNum+1);
  if (tNum == (NUM_THREADS-1)) end = N;
  for (i = start; i < end; i++)
    lSum += X[i];
  gSum[tNum] = lSum;
  delete (int *)pArg;
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
  for (j = 0; j < NUM_THREADS; j++) {
    int *threadNum =(int*) malloc(sizeof(int));
    *threadNum = j;
    pthread_create(&tHandles[j], NULL, Summation, (void *)threadNum);
  }
  for (j = 0; j < NUM_THREADS; j++) {
    pthread_join(tHandles[j], NULL);
    sum += gSum[j];
  }
  printf("The sum of array elements is %d\n", sum);
  return 0;
}
