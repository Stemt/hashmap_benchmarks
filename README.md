# Hashmap Benchmarks

This project is intended to test and compare different hashmap implementations.
But mainly to gauge the performance of [my own library](https://github.com/Stemt/hm.h), so note that there may be bias.

A focus is laid on the datastructures themselves, NOT the hash function used and or provided by a library.
The rationale behind is that ideally every applicaton of a hashmap should have a custom hash function engineered specifically for that application.
e.g. if you're working with zipcodes, make a custom hash func for zipcodes, so that you can minimize the amount of collisions and optimize the performance of any given hashmap implementation.

## Requirements 

- A C compiler available under the `cc` alias
- `python` 3.10 or greater
- `psutil` 5.9.8 or greater

## Running the Benchmarks

Simply run the python script `bench.py`.
```
python bench.py
```

## Tests and Rationale

### uint32 init

Tests how long it takes to initialize a given hashmap for an uint32 value and key.

### uint32 set

Tests how long it takes to insert [BENCH_INSERTIONS](./src/bench_config.h) amount of uint32 keys and values into the hashmap.
Important if a hashmap will experience a lot insertions.

### uint32 get

Tests how long it takes to retrieve [BENCH_INSERTIONS](./src/bench_config.h) amount of values into the hashmap.
Important if a hashmap will experience a lot retrievals.

### uint32 remove

Tests how long it takes to remove [BENCH_INSERTIONS](./src/bench_config.h) amount of keys and values from the hashmap.
Important if a hashmap will experience a lot removals.

### uint32 re set (reinsertions)

Tests how long it takes to reinsert [BENCH_INSERTIONS](./src/bench_config.h) amount of uint32 keys and values into the hashmap, after the same keys/values had previously been inserted and removed.

Some hashmaps may experience faster insertions after previously having had a lot of insertions and removals.
This is because initial insertions are more likely to trigger collisions and grow/rehashing events.
This test should mainly give an indication of if the hashmap shrinks after a bunch of removals and how this impacts performance compared to hashmaps that don't shrink.

### memory usage at end

Inside the benchmarks the hashmap structures have deliberately not been cleaned up before the program exits.
This is to make it easier for the python script to gauge the benchmark's memory usage using `psutil`.
This is an useful stat if you plan to use the hashmap in a constrained environment such as an embedded context.

## Libraries Compared

- [hm.h by me](https://github.com/Stemt/hm.h)
- [hashmap.h by sheredom](https://github.com/sheredom/hashmap.h)
- [hashmap.c by tidwall](https://github.com/tidwall/hashmap.c/blob/master/hashmap.c)
- [c-hashmap by Mashpoe](https://github.com/Mashpoe/c-hashmap)

## Results as of Writing

These results were generated on the following hardware:
```
AMD Ryzen 5 7640U w/ Radeon 760M Graphics (12) @ 4.97 GHz
```

The first column contains hashmap implementation used, the second column contains the time a test took to complete and
the last column contains a factor of time compared to hm.h (my own implementation).

```
--- uint32_init ---
stemt_HM            :    2060.00 ns :       1.00x
sheredom_hashmap_h  :     220.00 ns :       0.11x
tidwall_hashmap_c   :     470.00 ns :       0.23x
mashpoe_c_hashmap   :     240.00 ns :       0.12x

--- uint32_set ---
stemt_HM            :       8.56 ms :       1.00x
sheredom_hashmap_h  :      26.97 ms :       3.15x
tidwall_hashmap_c   :       9.63 ms :       1.12x
mashpoe_c_hashmap   :      57.53 ms :       6.72x

--- uint32_get ---
stemt_HM            :  265880.00 ns :       1.00x
sheredom_hashmap_h  :       3.70 ms :      13.90x
tidwall_hashmap_c   :       5.58 ms :      21.00x
mashpoe_c_hashmap   :       2.75 ms :      10.36x

--- uint32_remove ---
stemt_HM            :  659920.00 ns :       1.00x
sheredom_hashmap_h  :       7.03 ms :      10.66x
tidwall_hashmap_c   :       8.35 ms :      12.66x
mashpoe_c_hashmap   :       3.27 ms :       4.96x

--- uint32_re_set ---
stemt_HM            :       1.38 ms :       1.00x
sheredom_hashmap_h  :      11.57 ms :       8.36x
tidwall_hashmap_c   :       8.89 ms :       6.42x
mashpoe_c_hashmap   :      56.41 ms :      40.76x

--- memory usage at end ---
stemt_HM            :      77.02 MB
sheredom_hashmap_h  :      85.48 MB
tidwall_hashmap_c   :      35.37 MB
mashpoe_c_hashmap   :     278.32 MB
```
