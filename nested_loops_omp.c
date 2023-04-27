#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

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
  int i, j, ninner, nouter;
  struct timeval tv_start_loop, tv_end_loop;
  double *a, *b, sum, elapsed;

  ninner = atoi(argv[1]);
  nouter = atoi(argv[2]);
  a = (double *) malloc(sizeof(double) * ninner);
  b = (double *) malloc(sizeof(double) * ninner);

  // ----- Parallel region and parallel loop -----
  srand48(123456);
  for (int j=0; j<ninner; j++) {
    a[j] = drand48();
  }
  for (int j=0; j<ninner; j++) {
    b[j] = 0.0;
  }

  gettimeofday(&tv_start_loop, NULL);
  #pragma omp parallel
  {
    for (int i=0; i<nouter; i++) {
      #pragma omp for
      for (int j=1; j<ninner-1; j++) {
	b[j] = sqrt(0.2*a[j-1]) + sqrt(0.6*a[j]) + sqrt(0.2*a[j+1]);
	//b[j] = 0.2*a[j-1] + 0.6*a[j] + 0.2*a[j+1];
      }
      #pragma omp for
      for (int j=1; j<ninner-1; j++) {
	a[j] = sqrt(0.2*b[j-1]) + sqrt(0.6*b[j]) + sqrt(0.2*b[j+1]);
	//a[j] = 0.2*b[j-1] + 0.6*b[j] + 0.2*b[j+1];
      }
    }
  }
  gettimeofday(&tv_end_loop, NULL);
  
  sum = 0.0;
  for (int j=0; j<ninner; j++) {
    sum += b[j];
  }
  elapsed = elapsed_time(tv_start_loop, tv_end_loop);
  printf("Parallel region + parallel loop\n");
  printf("sum = %f, elapsed = %f\n", sum, elapsed);

  // ----- Combined construct -----
  srand48(123456);
  for (int j=0; j<ninner; j++) {
    a[j] = drand48();
  }
  for (int j=0; j<ninner; j++) {
    b[j] = 0.0;
  }

  gettimeofday(&tv_start_loop, NULL);
  for (int i=0; i<nouter; i++) {
    #pragma omp parallel for
    for (int j=1; j<ninner-1; j++) {
      b[j] = sqrt(0.2*a[j-1]) + sqrt(0.6*a[j]) + sqrt(0.2*a[j+1]);
      //b[j] = 0.2*a[j-1] + 0.6*a[j] + 0.2*a[j+1];
    }
    #pragma omp parallel for
    for (int j=1; j<ninner-1; j++) {
      a[j] = sqrt(0.2*b[j-1]) + sqrt(0.6*b[j]) + sqrt(0.2*b[j+1]);
      //a[j] = 0.2*b[j-1] + 0.6*b[j] + 0.2*b[j+1];
    }
  }
  gettimeofday(&tv_end_loop, NULL);
  
  sum = 0.0;
  for (int j=0; j<ninner; j++) {
    sum += b[j];
  }
  elapsed = elapsed_time(tv_start_loop, tv_end_loop);
  printf("Parallel combined construct\n");
  printf("sum = %f, elapsed = %f\n", sum, elapsed);

  printf("\n");
  return 0;
}
