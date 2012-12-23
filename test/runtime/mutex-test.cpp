// RUN: %srcroot/test/runtime/run-scheduler-test.py %s -gxx "%gxx" -llvmgcc "%llvmgcc" -projbindir "%projbindir" -ternruntime "%ternruntime" -ternannotlib "%ternannotlib"  -ternbcruntime "%ternbcruntime"

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#define N (1000)

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* thread_func(void *arg) {
  char buf[64];
  int tid = (intptr_t)arg;

  for(unsigned i=0;i<100;++i)
    sched_yield();

  //sprintf(buf, "%03d RUNS\n", tid);
  sprintf(buf, "RUNS\n");

  int ret;
  struct timespec   ts;
  struct timeval    tp;

  // test 3 different ways of acquiring a pthread mutex
  switch(tid % 3) {
  case 0:
    pthread_mutex_lock(&m);
    break;
  case 1:
    do{
      ret = pthread_mutex_trylock(&m);
    } while(ret==EBUSY);
    break;
  case 2:
    do {
      gettimeofday(&tp, NULL);
      ts.tv_sec  = tp.tv_sec;
      ts.tv_nsec = tp.tv_usec * 1000 + 100;
      ret = pthread_mutex_timedlock(&m, &ts);
    } while(ret == ETIMEDOUT);
    break;
  }

  write(1, buf, strlen(buf));
  pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[], char *env[]) {
  int ret;
  pthread_t th[N];

  for(unsigned i=0; i<N; ++i) {
    ret  = pthread_create(&th[i], NULL, thread_func, (void*)i);
    assert(!ret && "pthread_create() failed!");
  }

  for(unsigned i=0; i<N; ++i)
    pthread_join(th[i], NULL);
}
// CHECK indicates expected output checked by FileCheck; auto-generated by appending -gen to the RUN command above.
// CHECK:      RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
// CHECK-NEXT: RUNS
