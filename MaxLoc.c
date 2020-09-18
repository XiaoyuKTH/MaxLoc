#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

// timer
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;
  i = gettimeofday(&tp,&tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

int main(){

  int N = 10000000;
  double *X = (double *)malloc(N*sizeof(double));
  srand(time(0));
  for(int i=0; i<N; i++){
    X[i] = ((double)(rand())/RAND_MAX)*((double)(rand())/RAND_MAX)*((double)(rand())/RAND_MAX);
  }
  int k = omp_get_num_threads();
  double tStart, tEnd, maxval;
  int maxloc, Threads;
  #pragma omp parallel 
  {
    #pragma omp master
    {
      Threads = omp_get_num_threads();
    }
  }

  // serial
  maxval = -10.0; 
  maxloc = 0;
  tStart = mysecond();
  for(int i=0; i<N; i++){
    if(X[i] > maxval){
      maxval = X[i]; 
      maxloc = i;
    } 
  }
  tEnd = mysecond();
  printf("Threads %02i, Time %.2e s, %f, %i\n", Threads, tEnd-tStart, maxval, maxloc);

  // openmp
  maxval = -10.0; 
  maxloc = 0;
  tStart = mysecond();
  #pragma omp parallel for
  for(int i=0; i<N; i++){
    if(X[i] > maxval){
      maxval = X[i]; 
      maxloc = i;
    } 
  }
  tEnd = mysecond();
  printf("Threads %02i, Time %.2e s, %f, %i\n", Threads, tEnd-tStart, maxval, maxloc);
  
  // openmp critical
  maxval = -10.0; 
  maxloc = 0;
  tStart = mysecond();
  #pragma omp parallel for
  for(int i=0; i<N; i++){
    #pragma omp critical
    {
    if(X[i] > maxval){
      maxval = X[i]; 
      maxloc = i;
    }
    } 
  }
  tEnd = mysecond();
  printf("Threads %02i, Time %.2e s, %f, %i\n", Threads, tEnd-tStart, maxval, maxloc);

  // openmp temporary arrays
  int maxloc1[Threads];
  double maxval1[Threads];
  tStart = mysecond();
  #pragma omp parallel shared(maxloc1, maxval1)
  {
    int id  = omp_get_thread_num();
    maxval1[id] = -10.0;
    #pragma omp for
    for(int i=0; i<N; i++){
      if(X[i] > maxval1[id]){
        maxval1[id] = X[i]; 
        maxloc1[id] = i;
      } 
    }
  }
  maxloc = maxloc1[0];
  maxval = maxval1[0];
  for(int i=0; i<Threads; i++){
    if(maxval1[i]>maxval){
      maxloc = maxloc1[i];
      maxval = maxval1[i];
    }
  }
  tEnd = mysecond();
  printf("Threads %02i, Time %.2e s, %f, %i\n", Threads, tEnd-tStart, maxval, maxloc);

  // openmp solving false sharing
  typedef struct{double val; int loc; char pad[128];} tvals;
  tvals maxinfo[Threads];
  tStart = mysecond();
  #pragma omp parallel shared(maxinfo)
  {
    int id  = omp_get_thread_num();
    maxinfo[id].val = -10.0;
    #pragma omp for
    for(int i=0; i<N; i++){
      if(X[i] > maxinfo[id].val){
        maxinfo[id].val = X[i]; 
        maxinfo[id].loc = i;
      } 
    }
  }
  maxloc = maxinfo[0].loc;
  maxval = maxinfo[0].val;
  for(int i=0; i<Threads; i++){
    if(maxinfo[i].val > maxval){
      maxloc = maxinfo[i].loc;
      maxval = maxinfo[i].val;
    }
  }
  tEnd = mysecond();
  printf("Threads %02i, Time %.2e s, %f, %i\n", Threads, tEnd-tStart, maxval, maxloc);


  return 0;

}

