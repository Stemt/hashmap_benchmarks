
#include "bench_config.h"

#include "utest.h"

// tidwall/hashmap.c (https://github.com/tidwall/hashmap.c/tree/master)
#include "external/tidwall_hashmap_c/hashmap.h"
#include "external/tidwall_hashmap_c/hashmap.c"
#include <stdint.h>
#include <stdio.h>

typedef struct{
  uint32_t key;
  uint32_t value;
} KVPair_uint32;

int KVPair_uint32_compare(const void *ap, const void *bp, void *udata) {
  const KVPair_uint32* a = ap;
  const KVPair_uint32* b = bp;
  return a->key - b->key;
}

static struct hashmap* map;

uint64_t KVPair_uint32_hash(const void *itemp, uint64_t seed0, uint64_t seed1) {
    const KVPair_uint32 *item = itemp;
    uint32_t key = BENCH_UINT32_HASH(item->key);
    return hashmap_sip(&key, sizeof(uint32_t), seed0, seed1);
}

UTEST(tidwall_hashmap_c_Bench, uint32_init){
  map = hashmap_new(sizeof(KVPair_uint32), 0, 0, 0, KVPair_uint32_hash, KVPair_uint32_compare, NULL, NULL);
  ASSERT_NE(map,NULL);
}

UTEST(tidwall_hashmap_c_Bench, uint32_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    hashmap_set(map, &(KVPair_uint32){.key=i,.value=i});
    ASSERT_FALSE(hashmap_oom(map));
  }
}

UTEST(tidwall_hashmap_c_Bench, uint32_get){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    const KVPair_uint32* res = hashmap_get(map, &(KVPair_uint32){.key=i});
    ASSERT_NE(res, NULL);
    ASSERT_EQ(res->value, i);
  }
}

UTEST(tidwall_hashmap_c_Bench, uint32_remove){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    ASSERT_NE(hashmap_delete(map, &(KVPair_uint32){.key=i}),NULL);
  }
  ASSERT_EQ(map->count, 0UL);
}

UTEST(tidwall_hashmap_c_Bench, uint32_re_set){
  for(uint32_t i = 0; i < BENCH_INSERTIONS; ++i){
    hashmap_set(map, &(KVPair_uint32){i,i});
    ASSERT_FALSE(hashmap_oom(map));
  }
}

