
#include "bench_config.h"

#include "utest.h"

// Mashpoe/c-hashmap (https://github.com/Mashpoe/c-hashmap)
#define __HASHMAP_REMOVABLE
#include "external/mashpoe_c_hashmap/map.h"
#include "external/mashpoe_c_hashmap/map.c"
#include <stdint.h>
#include <stdio.h>


static hashmap* map;

UTEST(mashpoe_c_hashmap_Bench, uint32_init){
  map = hashmap_create();
  ASSERT_NE(map,NULL);
}

UTEST(mashpoe_c_hashmap_Bench, uint32_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* val = calloc(1,sizeof(i));
    *val = i;
    int error = hashmap_set(map, val, sizeof(*val), *val);
    ASSERT_NE(error, -1);
  }
}

UTEST(mashpoe_c_hashmap_Bench, uint32_get){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uintptr_t res = 0;
    int found = hashmap_get(map, &i, sizeof(i), &res);
    ASSERT_TRUE(found);
    ASSERT_EQ(res, i);
  }
}

UTEST(mashpoe_c_hashmap_Bench, uint32_remove){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    hashmap_remove(map, &i, sizeof(i));
  }
  ASSERT_EQ(map->count-map->tombstone_count, 0);
}

UTEST(mashpoe_c_hashmap_Bench, uint32_re_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    uint32_t* val = calloc(1,sizeof(i));
    *val = i;
    int error = hashmap_set(map, val, sizeof(*val), *val);
    ASSERT_NE(error, -1);
  }
}

