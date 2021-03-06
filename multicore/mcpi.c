#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#pragma omp parallel private(x, y, i) reduction(+:f)
{
#prama omp for
  for (j=0; j<BLOCK_SIZE; ++j) {

    for(i=0; i<BLOCK_SIZE; ++i)
double f(double a)
{
  return 4.0/(1.0 + a*a);
}

int num_steps = 1000000;
double step;

int main()
{
  clock_t start, stop;
  double x, pi, sum=0.0;
  int i;

  step = 1./(double)num_steps;
  start = clock();

#pragma omp parallel for private(x) reduction(+:sum)
  for (i=0; i< num_steps; ++i) {
    x = step * ((double) i - 0.5);
    sum += f(x);
  }

  pi = step*sum;

  printf("pi = %g\n",pi);
}
