/* Compile with:
     g++ -fopenmp -o openmp2 openmp2.cpp
*/
#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
  printf("Hello from serial.\n");
  printf("Thread number=%d\n", omp_get_thread_num());

  #pragma omp parallel
  {
    printf("Hello from parallel. Thread number=%d\n", omp_get_thread_num());
  }
  printf("Hello from serial again.\n");
  return 0;
}
