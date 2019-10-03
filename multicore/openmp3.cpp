/* Compile with:
     g++ -fopenmp -o openmp3 openmp3.cpp
*/
#include <stdio.h>
#include <omp.h>

void initialization()
{
  int counter=0;
  printf("thread %d start initialization\n", omp_get_thread_num());
  for (int i=0; i<100000; ++i)
    counter++;
  printf("thread %d finish initialization\n", omp_get_thread_num());
}

void process()
{
  int counter=0;
  printf("thread %d start process\n",omp_get_thread_num());
  for (int i=0; i<100000; ++i)
    counter += i;
  printf("thread %d finish process\n",omp_get_thread_num());
}

int main(int argc, char* argv[])
{
#pragma omp parallel
  {
    initialization();
#pragma omp barrier
    process();
  }
}
