#ifndef BENCH_CONFIG_H
#define BENCH_CONFIG_H

#include "utest.h"

#define BENCH_INSERTIONS 0xFFFFFUL

UTEST_STATE();

int main(int argc, const char *const *argv){
  int res = utest_main(argc, argv);
  fflush(stdout);
  // wait so that python script can get peak mem usage
  getc(stdin); 
  return res;
}

#endif // BENCH_CONFIG_H
