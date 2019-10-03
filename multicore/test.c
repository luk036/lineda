#include <stdio.h>

int a[100];
int b[100];

void* work(float* c, int N) 
{
  float x, y; int i;
#pragma omp parallel for private(x, y)
  for (i=0; i<N; ++i) {
    x = a[i]; y=b[i];
    c[i] = x + y;
  }
}

float dot_prod(float* a, float* b, int N)
{
  float sum = 0.0;
  int i;
#pragma omp parallel for reduction(+:sum)
  for (i=0; i<N; ++i) {
#pragma omp critical
    sum += a[i] * b[i];
  }
}


int main()
{
  int i;
  printf("hello \n");
  #pragma omp parallel for
  for (i=0; i<6; ++i) {
    printf("Iter: %d\n",i);
  }
  printf("Goodbye world \n");
}
