#include <pthread.h>
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <execinfo.h>
#include <tern/hooks.h>
#include <tern/space.h>
#include <tern/options.h>
#include <tern/runtime/runtime.h>

using namespace tern;

#define __NEED_INPUT_INSID
#define __USE_TERN_RUNTIME
//#define PRINT_DEBUG

/*
#ifdef __USE_TERN_RUNTIME
#define __INSTALL_RUNTIME
#endif

#ifdef __INSTALL_RUNTIME

struct preload_static_block
{
  preload_static_block()
  {
    InstallRuntime();
  }
};

// notice, it doesn't work for functions called in static constructors of program 

static preload_static_block preload_initializer;

#endif
*/

#define __SPEC_HOOK___libc_start_main

static void print_stack()
{
#if 0
  void* tracePtrs[500];
  printf("here");
  fflush(stdout);
  int count = backtrace( tracePtrs, 500 );
  printf("here");
  fflush(stdout);
  char** funcNames = backtrace_symbols( tracePtrs, count );
  
  printf("here");
  fflush(stdout);
  // Print the stack trace
  for( int ii = 0; ii < count; ii++ )
    printf( "%s\n", funcNames[ii] );
  fflush(stdout);

  // Free the string pointers
  free( funcNames );
#endif
}

void *get_eip()
{
  // Change this "idx" to be 2 when pthread_*() functions are called by 
  // application code directly; and change it to be 3 when pthread_*()
  // functions are called by the app's wrapper functions.
  const int idx = 3;
  const int len = 5;
  void *tracePtrs[len];
  int i;
  uint64_t ret = 0; 
  /* Fixme: sometimes after fork(), the child process may hang (100% cpu) here, weird.
  Saw this case when running the simple-fork-test.c testcase. */
  int count = backtrace(tracePtrs, len);

  if (options::whole_stack_eip_signature)
  {
    ret = 0;
    for (i = 0; i < count; ++i)
      ret = ret * 97 + (uint64_t) tracePtrs[i];
    return (void*) ret;
    //std::cout << std::hex << tracePtrs[2] << std::dec << std::endl;
    //char** funcNames = backtrace_symbols( tracePtrs, count );
    //printf( "%s\n", funcNames[2] );
    //printf(stderr, "reteip: %p\n", tracePtrs[1]);
  } else
    return tracePtrs[idx];  //  this is ret_eip of my caller
}

#define HOOK_MUTEX_COND
#define HOOK_BARRIER
#define HOOK_SEMAPHORE
#define HOOK_BLOCKING_FUNCS
#define HOOK_USER_ANNOTATIONS

//  notice we have not implemented tern_fork yet. so the
//  hooking code is disabled. 
//#define __SPEC_HOOK_fork

#ifndef HOOK_MUTEX_COND
#define __SPEC_HOOK_pthread_mutex_init
#define __SPEC_HOOK_pthread_mutex_lock
#define __SPEC_HOOK_pthread_mutex_unlock
#define __SPEC_HOOK_pthread_mutex_trylock
#define __SPEC_HOOK_pthread_mutex_timedlock
#define __SPEC_HOOK_pthread_cond_wait
#define __SPEC_HOOK_pthread_cond_signal
#define __SPEC_HOOK_pthread_cond_timedwait
#define __SPEC_HOOK_pthread_cond_broadcast
#endif

#ifndef HOOK_BARRIER
#define __SPEC_HOOK_pthread_barrier_wait
#define __SPEC_HOOK_pthread_barrier_init
#define __SPEC_HOOK_pthread_barrier_destroy
#endif

#ifndef HOOK_SEMAPHORE
#define __SPEC_HOOK_sem_wait
#define __SPEC_HOOK_sem_trywait
#define __SPEC_HOOK_sem_timedwait
#define __SPEC_HOOK_sem_post
#endif

#ifndef HOOK_BLOCKING_FUNCS
#define __SPEC_HOOK_recv
#define __SPEC_HOOK_connect
#define __SPEC_HOOK_accept
#define __SPEC_HOOK_accept4
#define __SPEC_HOOK_read
#define __SPEC_HOOK_write
#define __SPEC_HOOK_sigwait
#define __SPEC_HOOK_select
#define __SPEC_HOOK_recvfrom
#define __SPEC_HOOK_recvmsg
#define __SPEC_HOOK_sleep
#define __SPEC_HOOK_usleep
#define __SPEC_HOOK_nanosleep
#define __SPEC_HOOK_epoll_wait
#endif

#ifndef HOOK_USER_ANNOTATIONS
#define __SPEC_HOOK_tern_lineup_init
#define __SPEC_HOOK_tern_lineup_destroy
#define __SPEC_HOOK_tern_lineup_start
#define __SPEC_HOOK_tern_lineup_end
#define __SPEC_HOOK_tern_lineup
#endif

#include "spec_hooks.cpp"
#include "template.cpp"
#include "annotation_hooks.cpp"

