# Hashmap Benchmarks

This project is intended to test and compare different hashmap implementations.
But mainly to gauge the performance of [my own library](https://github.com/Stemt/hm.h), so note that there may be bias.

A focus is laid on the datastructures themselves, NOT the hash function used and or provided by a library.
The rationale behind this is that ideally every applicaton of a hashmap should have a custom hash function engineered specifically for that application.
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

## Results as of Writing

These results were generated on the following hardware:
```
AMD Ryzen 7 5800X (16) @ 4.85 GHz
```

The first column contains hashmap implementation used, the second column contains the time a test took to complete and
the last column contains a factor of time compared to hm.h (my own implementation).

```
--- uint32_init ---
stemt_HM            :     720.00 ns :       1.00x
sheredom_hashmap_h  :     150.00 ns :       0.21x
tidwall_hashmap_c   :     210.00 ns :       0.29x

--- uint32_set ---
stemt_HM            :     168.34 ms :       1.00x
sheredom_hashmap_h  :     241.91 ms :       1.44x
tidwall_hashmap_c   :      71.90 ms :       0.43x

--- uint32_get ---
stemt_HM            :      74.96 ms :       1.00x
sheredom_hashmap_h  :      69.92 ms :       0.93x
tidwall_hashmap_c   :      70.15 ms :       0.94x

--- uint32_remove ---
stemt_HM            :      72.02 ms :       1.00x
sheredom_hashmap_h  :     131.17 ms :       1.82x
tidwall_hashmap_c   :      70.59 ms :       0.98x

--- uint32_re_set ---
stemt_HM            :      73.98 ms :       1.00x
sheredom_hashmap_h  :      75.15 ms :       1.02x
tidwall_hashmap_c   :      72.09 ms :       0.97x

--- memory usage at end ---
stemt_HM            :     118.51 MB
sheredom_hashmap_h  :     437.64 MB
tidwall_hashmap_c   :      35.29 MB
```
