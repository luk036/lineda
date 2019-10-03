#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* pthread func */
void* sum_val(void* arg)
{
  int sum=0;
  int i;
  int count=*(int*)arg;
  for (i=0;i<count;++i)
    sum=sum+i;
  printf("sum is %d\n", sum);
  pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
  pthread_t pt;
  int count=10;
  int ret_val;
  pthread_attr_t attr;
  struct sched_param sp;
  sp.sched_priority=2;

  /* initialization */
  ret_val=pthread_attr_init(&attr);
  if (ret_val != 0) {
    printf("pthread_attr_init error!\n");
    exit(1);
  }

  /* pthread_attr_setdetachstate() */
  ret_val=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if(ret_val != 0) {
    printf("pthread_attr_setdetachstate error!\n");
    exit(1);
  }

  /* setup pthread round-robin */
  ret_val=pthread_attr_setschedpolicy(&attr, SCHED_RR);
  if(ret_val != 0) {
    printf("pthread_attr_setschedpolicy error!\n");
    exit(1);
  }
  
  /* setup pthread priority */
  ret_val=pthread_attr_setschedparam(&attr, &sp);
  if(ret_val != 0) {
    printf("pthread_attr_setschedparam error!\n");
    exit(1);
  }

  ret_val=pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  if(ret_val != 0) {
    printf("pthread_attr_setinheritsched error!\n");
    exit(1);
  }
  
  ret_val=pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  if(ret_val != 0) {
    printf("pthread_attr_setscope error!\n");
    exit(1);
  }

  ret_val=pthread_create(&pt, NULL, sum_val, (void*)&count);
  if (ret_val != 0) {
    printf("pthread_create error!\n");
  }

  /* release */
  pthread_attr_destroy(&attr);

  /* Wait for thread pt finish */
  pthread_join(pt, NULL);
  return 0;
}
