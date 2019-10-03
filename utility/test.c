#include <stdio.h>
#include <stdlib.h>

int main()
{
  int** A;
  size_t N = 50;
  size_t M = 60;
  size_t i;

  A = (int**) malloc(sizeof(int*) * N * M);
  for (i=0; i<N; ++i) {
    A[i] = (int*) malloc(sizeof(int) * M);
  }

  A[1][2] = 3;




  for (i=0; i<N; ++i) {
    free(A[i]);
  }
  free(A);
}
