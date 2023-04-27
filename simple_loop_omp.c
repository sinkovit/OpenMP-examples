#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

// Program for demonstrating simple loop parallelization with OpenMP
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
  struct timeval tv_start_loop, tv_end_loop;
  double *a, *b, sum, elapsed;

  n = atoi(argv[1]);
  a = (double *) malloc(sizeof(double) * n);
  b = (double *) malloc(sizeof(double) * n);

  // Fill array a with random values
  srand48(123456);
  for (int i=0; i<n; i++) {
    a[i] = drand48();
  }

  // Apply f1() to array a and store in b
  gettimeofday(&tv_start_loop, NULL);
#pragma omp parallel for
  for (int i=0; i<n; i++) {
    b[i] = f1(a[i]);
  }
  gettimeofday(&tv_end_loop, NULL);
  
  // Calculate sum over array b and print result.  This will prevent
  // the compiler from optimizing away the expensive calculations  
  sum = 0.0;
  for (int i=0; i<n; i++) {
    sum += b[i];
  }

  elapsed = elapsed_time(tv_start_loop, tv_end_loop);
  printf("sum = %f\n", sum);
  printf("Time spent in loop= %f\n", elapsed);
  
  return 0;
}
