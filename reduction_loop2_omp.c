#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

// Program for demonstrating reduction variables with OpenMP
//
// Expected run times for serial execution on single AMD EPYC Zen2
// core with 'n' calls to f1()
// n=1000000:   14 seconds
// n=10000000: 140 seconds 

double f1(double x) {
  // This function doesn't do anything useful except to spend a lot of
  // time calculting expensive math functions

  double y=0.0;
  int i;

  for (i=0; i<100; i++) {
    y += sqrt(pow(cosh(x + 0.01*i), 2.3) + pow(sinh(x + 0.01*i), 3.4));
  }
  return(y);
}


double elapsed_time(struct timeval tv_start, struct timeval tv_end) {
  // Function to calculate elapsed time from difference between
  // results from two calls to gettimeofday()

  double elapsed;
  elapsed = (tv_end.tv_sec - tv_start.tv_sec) +
    (tv_end.tv_usec - tv_start.tv_usec) / 1000000.0;
  return(elapsed);
}


int main(int argc, char **argv)
{
  int i, n;
  struct timeval tv_start_loop1, tv_end_loop1;
  struct timeval tv_start_loop2, tv_end_loop2;
  struct timeval tv_start_loop3, tv_end_loop3;
  double *a, sum1, elapsed1, sum2, elapsed2, sum3, elapsed3;

  n = atoi(argv[1]);
  a = (double *) malloc(sizeof(double) * n);

  // Fill array a with random values
  srand48(123456);
  for (int i=0; i<n; i++) {
    a[i] = drand48();
  }

  // Apply f1() to array and sum results (race condition)
  sum1 = 0.0;
  gettimeofday(&tv_start_loop1, NULL);
  #pragma omp parallel for
  for (int i=0; i<n; i++) {
    sum1 += f1(a[i]);
  }
  gettimeofday(&tv_end_loop1, NULL);
  elapsed1 = elapsed_time(tv_start_loop1, tv_end_loop1);

  // Apply f1() to array sum results (atomic))
  sum2 = 0.0;
  gettimeofday(&tv_start_loop2, NULL);
  #pragma omp parallel for
  for (int i=0; i<n; i++) {
    #pragma omp atomic
    sum2 += f1(a[i]);
  }
  gettimeofday(&tv_end_loop2, NULL);
  elapsed2 = elapsed_time(tv_start_loop2, tv_end_loop2);

  // Apply f1() to array and sum results(reduction variable)
  sum3 = 0.0;
  gettimeofday(&tv_start_loop3, NULL);
  #pragma omp parallel for reduction(+:sum3)
  for (int i=0; i<n; i++) {
    sum3 += f1(a[i]);
  }
  gettimeofday(&tv_end_loop3, NULL);
  elapsed3 = elapsed_time(tv_start_loop3, tv_end_loop3);
    
  printf("Naive summation\n");
  printf("sum1 = %f\n", sum1);
  printf("Time spent in loop 1 = %f\n", elapsed1);
  printf("\n");

  printf("Summation using atomic pragma\n")  ;
  printf("sum1 = %f\n", sum2);
  printf("Time spent in loop 2 = %f\n", elapsed2);
  printf("\n");

  printf("Summation using reduction clause\n"); 
  printf("sum1 = %f\n", sum3);
  printf("Time spent in loop 3 = %f\n", elapsed3);
  
  return 0;
}
