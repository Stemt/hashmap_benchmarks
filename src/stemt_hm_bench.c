#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bench_config.h"

#include "utest.h"

#define HM_IMPLEMENTATION
#include "external/stemt_hm_h/hm.h"

size_t uint32_hash(const char* key, size_t key_len){
  (void)(key_len);
  HM_ASSERT(key_len == sizeof(uint32_t));
  return *(uint32_t*)(key);
}

HM_GEN_WRAPPER_PROTOTYPE(int);
HM_GEN_WRAPPER_IMPLEMENTATION(int);


static HM hm_uint32 = {0};

UTEST(stemt_HM_Bench, uint32_init){
  //fprintf(stdout, "BENCH_INSERTIONS: %zu\n", BENCH_INSERTIONS);
  ASSERT_TRUE(HM_init(&hm_uint32, sizeof(int32_t), 0));
  HM_override_hash_func(&hm_uint32, uint32_hash);
}

UTEST(stemt_HM_Bench, uint32_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    ASSERT_TRUE(HM_sk_set(&hm_uint32, i, &i));
  }
}

UTEST(stemt_HM_Bench, uint32_get){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* res = HM_sk_get(&hm_uint32, i);
    ASSERT_NE(res, NULL);
    ASSERT_EQ(*res, i);
  }
}

UTEST(stemt_HM_Bench, uint32_remove){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    HM_sk_remove(&hm_uint32, i);
  }
  ASSERT_EQ(hm_uint32.count, 0UL);
}

UTEST(stemt_HM_Bench, uint32_re_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    ASSERT_TRUE(HM_sk_set(&hm_uint32, i, &i));
  }
}

