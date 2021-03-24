#ifndef H_TESTING__
#define H_TESTING__

#define stringify(s) #s

#define run_test(testname)			\
  do {						\
    printf(stringify(testname)"...");		\
    test_##testname();				\
    printf("PASS\n");				\
  } while (0);

#endif
