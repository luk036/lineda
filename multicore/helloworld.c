/* Compile with
     gcc -o helloworld helloworld.c -lpthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUMBER 2
int retval_hello1=2, retval_hello2=3;

void* hello1(void* arg)
{
  char* hello_str=(char*)arg;
  sleep(1);
  printf("%s\n", hello_str);
  pthread_exit(&retval_hello1);
}

void* hello2(void* arg)
{
  char* hello_str=(char*)arg;
  sleep(2);
  printf("%s\n", hello_str);
  pthread_exit(&retval_hello2);
}

int main(int argc, char* argv[])
{
  int i;
  int ret_val;
  int* retval_hello[2];

  /* declare pthread_t ... */
  pthread_t pt[THREAD_NUMBER];
  const char* arg[THREAD_NUMBER];
  arg[0] = "hello world from thread1";
  arg[1] = "hello world from thread2";
  printf("Begin to create threads...\n");

  /* use pthread_create */
  ret_val = pthread_create(&pt[0], NULL, hello1, (void*)arg[0]);
  if (ret_val != 0) {
    printf("pthread_create error!\n");
    exit(1);
  }
  ret_val = pthread_create(&pt[1], NULL, hello2, (void*)arg[1]);
  if (ret_val != 0) {
    printf("pthread_create error!\n");
    exit(1);
  }
  printf("Now, the main thread returns.\n");
  printf("Begin to wait for threads...\n");

  /* Use pthread_join */
  for(i=0; i<THREAD_NUMBER; ++i) {
    ret_val=pthread_join(pt[i], (void**)&retval_hello[i]); 
    if (ret_val != 0) {
      printf("pthread_join error! \n");
      exit(1);
    }
    else {
      printf("return value is %d\n", *retval_hello[i]);
    }
  }
  return 0;
}
