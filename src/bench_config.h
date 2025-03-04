#ifndef BENCH_CONFIG_H
#define BENCH_CONFIG_H

#include <stdlib.h>

#include "utest.h"

#define BENCH_INSERTIONS 0xFFFFFUL

static int seed = 0;
// spreads out key distribution based on seed passed by bench.py
#define BENCH_UINT32_HASH(key) (srand(key+seed),rand())


UTEST_STATE();

int main(int argc, const char *const *argv){
  if(argc > 0){
    seed = atoi(argv[0]);
    argv++;
    argc--;
  }
  srand(seed);
  int res = utest_main(argc, argv);
  fflush(stdout);
  // wait so that bench.py can get mem usage
  getc(stdin); 
  return res;
}

#endif // BENCH_CONFIG_H
