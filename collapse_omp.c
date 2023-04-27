#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

double f4(double w, double x, double y, double z) {
  // This function doesn't do anything useful except to spend a lot of
  // time calculting expensive math functions

  double a=0.0;
  
  for (int i=0; i<160; i++) {
    double eps = i*0.01;
    a += sqrt(cos(x*y*z+eps) + sin(w*x*y+eps) + pow(x,y+eps) + pow(w,z+eps));
  }
  a = pow(a,0.125);

  return(a);
}



int main(int argc, char **argv)
{
  int n;
  double tbeg, tend, elapsed1, elapsed2;
  double *a, *b, *c, *d, sum1=0.0, sum2=0.0;

  n = atoi(argv[1]);
  a = (double *) malloc(sizeof(double) * n);
  b = (double *) malloc(sizeof(double) * n);
  c = (double *) malloc(sizeof(double) * n);
  d = (double *) malloc(sizeof(double) * n);

  // Fill arrays with random values
  srand48(123456);
  for (int i=0; i<n; i++) {
    a[i] = drand48();
    b[i] = drand48();
    c[i] = drand48();
    d[i] = drand48();
  }

  tbeg = omp_get_wtime();
  #pragma omp parallel for reduction(+:sum1)
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      for (int k=0; k<n; k++) {
	for (int l=0; l<n; l++) {
	  sum1 += f4(a[i], b[j], c[k], d[l]);
	}
      }
    }
  }
  tend = omp_get_wtime();
  elapsed1 = tend - tbeg;

  tbeg = omp_get_wtime();
  #pragma omp parallel for reduction(+:sum2) collapse(4)
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      for (int k=0; k<n; k++) {
	for (int l=0; l<n; l++) {
	  sum2 += f4(a[i], b[j], c[k], d[l]);
	}
      }
    }
  }
  tend = omp_get_wtime();
  elapsed2 = tend - tbeg;

  printf("sum1 = %f, sum2 = %f\n", sum1, sum2);
  printf("Time w/o collapse = %f\n", elapsed1);
  printf("Time w/ collapse  = %f\n", elapsed2);
  
  return 0;
}
