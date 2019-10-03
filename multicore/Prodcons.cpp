#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

#define NBUFF 3
int nitems;

struct sto {
  int buff[NBUFF];
  sem_t mutex, nempty, nstored;
};

struct sto shared;

void *produce(void*), *consume(void*);

int main(int argc, char* argv[])
{
  pthread_t tid_produce, tid_consumer;
  if (argc != 2) {
    cout << "Usage: prodcons number" << endl;
    exit(0);
  }
  nitems=atoi(argv[1]);
  
  sem_init(&shared.mutex, 0, 1);
  sem_init(&shared.nempty, 0, NBUFF);
  sem_init(&shared.nstored, 0, 0);

  pthread_create(&tid_produce, NULL, produce, NULL);
  pthread_create(&tid_consumer, NULL, consume, NULL);
  pthread_join(tid_produce, NULL);
  pthread_join(tid_consumer, NULL);

  sem_destroy(&shared.mutex);
  sem_destroy(&shared.nempty);
  sem_destroy(&shared.nstored);
  exit(0);
}

void* produce(void* arg)
{
  int i;
  for (i=0; i<nitems; ++i) {
    sem_wait(&shared.nempty);
    sem_wait(&shared.mutex);
    shared.buff[i % NBUFF] = i;
    cout << "Product: " << shared.buff[i % NBUFF] << endl;
    sem_post(&shared.mutex);
    sem_post(&shared.nstored);
  }
  return NULL;
}

void* consume(void* arg)
{
  int i;
  for (i=0; i<nitems; ++i) {
    sem_wait(&shared.nstored);
    sem_wait(&shared.mutex);
    if (shared.buff[i % NBUFF] != i)
      cout << "buff[" << i << "]=" << shared.buff[i%NBUFF] << endl;
    cout << "Consumer: " << shared.buff[i % NBUFF] << endl;
    sem_post(&shared.mutex);
    sem_post(&shared.nempty);
  }
  return NULL;
}
