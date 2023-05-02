#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

double f1(double x) {
  // This function doesn't do anything useful except to spend a lot of
  // time calculting expensive math functions

  double y=0.0;
  for (int i=0; i<3; i++) {
    y += sqrt(pow(cosh(x + 0.01*i), 2.3) + pow(sinh(x + 0.01*i), 3.4));
  }
  return(y);
}



double f2(double x, int j) {
  // This function doesn't do anything useful except to spend a lot of
  // time calculting expensive math functions. Note that the number of
  // iterations of the loop depends on the argument j

  double y=0.0;
  for (int i=0; i<j; i++) {
    y += sqrt(pow(cosh(x + 0.01*i), 2.3) + pow(sinh(x + 0.01*i), 3.4));
  }
  return(y);
}



int main(int argc, char **argv)
{
  int *b, n;
  double *a, tbeg, tend, sum;

  n = atoi(argv[1]);
  a = (double *) malloc(sizeof(double) * n);
  b = (int *) malloc(sizeof(int) * n);

  // Fill arrays with random doubles (a) and intgers (b)
  srand48(123456);
  for (int i=0; i<n; i++) {
    a[i] = drand48();
    b[i] = drand48()*10 + 1;
  }

  // Case 1: All iteration take same time, static scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static)
  for (int i=0; i<n; i++) {
    sum += f1(a[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("All iterations equal, schedule(static):                   t = %f\n", tend - tbeg);

  // Case 2: All iteration take same time, static interleaved (4) scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static,4)
  for (int i=0; i<n; i++) {
    sum += f1(a[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("All iterations equal, schedule(static,4):                 t = %f\n", tend - tbeg);

  // Case 3: All iteration take same time, dynamic (4) scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(dynamic,4)
  for (int i=0; i<n; i++) {
    sum += f1(a[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("All iterations equal, schedule(dynamic,4):                t = %f\n", tend - tbeg);

  // Case 4: All iteration take same time, static interleaved (32) scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static,32)
  for (int i=0; i<n; i++) {
    sum += f1(a[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("All iterations equal, schedule(static,32):                t = %f\n", tend - tbeg);

  // Case 5: All iteration take same time, dynamic (32) scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(dynamic,32)
  for (int i=0; i<n; i++) {
    sum += f1(a[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("All iterations equal, schedule(dynamic,32):               t = %f\n", tend - tbeg);

  // Case 6: Iterations take progressively longer, static scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], (int) i*10.0/n);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iterations progressively longer, schedule(static):        t = %f\n", tend - tbeg);
  
  // Case 7: Iterations take progressively longer, static interleaved scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static,16)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], (int) i*10.0/n);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iterations progressively longer, schedule(static,16):     t = %f\n", tend - tbeg);
  
  // Case 8: Iterations take progressively longer, dynamic scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(dynamic,16)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], (int) i*10.0/n);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iterations progressively longer, schedule(dynamic,16):    t = %f\n", tend - tbeg);

  // Case 9: Iterations time random, static scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], b[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iteration time random, schedule(static):                  t = %f\n", tend - tbeg);
  
  // Case 10: Iterations time random, static interleaved scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(static,16)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], b[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iteration time random, schedule(static,16):               t = %f\n", tend - tbeg);
  
  // Case 11: Iterations time random, dynamic scheduling
  tbeg = omp_get_wtime();
  sum = 0.0;
  #pragma omp parallel for reduction(+:sum) schedule(dynamic,16)
  for (int i=0; i<n; i++) {
    sum += f2(a[i], b[i]);
  }
  if (sum == 0.0) { printf("%f\n", sum); }
  tend = omp_get_wtime();
  printf("Iteration time random, schedule(dynamic,16):              t = %f\n", tend - tbeg);
  


  
  return 0;
}
