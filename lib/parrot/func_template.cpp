#ifndef __SPEC_HOOK_FUNC_NAME
extern "C" FUNC_RET_TYPE FUNC_NAME(ARGS_WITH_NAME) {
  typedef FUNC_RET_TYPE(*orig_func_type)(ARGS_WITHOUT_NAME);

  static orig_func_type orig_func;

#ifdef __USE_TERN_RUNTIME
  if (Space::isApp() && options::DMT) {
#ifdef PRINT_DEBUG
    fprintf(stdout, "%04d: FUNC_NAME is hooked.\n", (int)pthread_self());
    print_stack();
    fflush(stdout);
#endif

#ifdef __NEED_INPUT_INSID
    void *eip = 0;
    FUNC_RET_TYPE ret;
    if (options::dync_geteip) {
      Space::enterSys();
      eip = get_eip();
      Space::exitSys();
    }
    ret = tern_FUNC_NAME((unsigned)(uint64_t)eip, ARGS_ONLY_NAME);
#else
    ret = tern_FUNC_NAME(ARGS_ONLY_NAME);
#endif

#ifdef PRINT_DEBUG
    fprintf(stdout, "%04d: FUNC_NAME returned.\n", (int)pthread_self());
    fflush(stdout);
#endif
    return ret;
  }
#endif

  if (!orig_func) {
    void * handle;
    if (!(handle = dlopen("LIB_PATH", RTLD_LAZY))) {
      perror("dlopen");
      abort();
    }

    orig_func = (orig_func_type)dlsym(handle, "FUNC_NAME");

    if (dlerror()) {
      perror("dlsym");
      abort();
    }
    dlclose(handle);
  }

#ifdef PRINT_DEBUG
  fprintf(stdout, "%04d: FUNC_NAME is called.\n", (int)pthread_self());
  fflush(stdout);
#endif

  return orig_func(ARGS_ONLY_NAME);
}
#endif
