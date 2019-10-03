/* Compile with:
      g++ -fopenmp -o openmp1 openmp1.cpp
   Set environment variable OMP_NUM_THREADS
      export 
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
