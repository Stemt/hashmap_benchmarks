#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bench_config.h"

#include "utest.h"

// sheredom/hashmap.h (https://github.com/sheredom/hashmap.h)
#include "external/sheredom_hashmap_h/hashmap.h"


uint32_t hashmap_h_uint32_hash(uint32_t seed, const void* key, uint32_t key_len){
  (void)(seed);
  (void)(key_len);
  return *(uint32_t*)(key);
}

int hashmap_h_uint32_compare(const void* a, uint32_t a_len, const void* b, uint32_t b_len){
  (void)(a_len);
  (void)(b_len);
  return *(uint32_t*)a == *(uint32_t*)b;
}

static struct hashmap_s hashmap = {0};
static struct hashmap_create_options_s hashmap_options = {0};

UTEST(sheredom_hashmap_h_Bench, uint32_init){
  hashmap_options.hasher = hashmap_h_uint32_hash;
  hashmap_options.comparer = hashmap_h_uint32_compare;
  hashmap_options.initial_capacity = 0;//BENCH_INSERTIONS*2;
  ASSERT_EQ(hashmap_create_ex(hashmap_options, &hashmap),0);
}

UTEST(sheredom_hashmap_h_Bench, uint32_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* val = calloc(1, sizeof(i));
    *val = i;
    ASSERT_EQ(hashmap_put(&hashmap, val, sizeof(*val), val),0);
  }
}

UTEST(sheredom_hashmap_h_Bench, uint32_get){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* res = hashmap_get(&hashmap, &i, sizeof(i));
    ASSERT_NE(res, NULL);
    ASSERT_EQ(*res, i);
  }
}

UTEST(sheredom_hashmap_h_Bench, uint32_remove){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* res = hashmap_get(&hashmap, &i, sizeof(i));
    ASSERT_NE(res, NULL);
    ASSERT_EQ(hashmap_remove(&hashmap, &i, sizeof(i)),0);
    free(res);
  }
  ASSERT_EQ(hashmap_num_entries(&hashmap), 0UL);
}

UTEST(sheredom_hashmap_h_Bench, uint32_re_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* val = calloc(1, sizeof(i));
    *val = i;
    ASSERT_EQ(hashmap_put(&hashmap, val, sizeof(*val), val),0);
  }
}

