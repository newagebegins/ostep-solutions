# Comparison of different implementations of MapReduce

* `wordcount` (uses `mapreduce.c`) — stores keys and values in linked lists
* `wordcount_array` (uses `mapreduce_array.c`) — same as `wordcount`, but stores keys in growable arrays
* `wordcount_array_values` (uses `mapreduce_array_values.c`) — same as `wordcount_array`, but also stores values in growable arrays
* `wordcount_hash (uses mapreduce_hash.c)` — same as `wordcount_array`, but uses key hashes instead of `strcmp` (`strcmp` is still used for sorting)

Example test command:
```
time ./wordcount moby-dick.txt hamlet.txt > tmp.txt
```

All programs are compiled with `-O2` optimizations, use 4 mappers and 4 reducers.

| Program                | Time |
|------------------------|------|
| wordcount              | 20s  |
| wordcount_array        | 4.5s |
| wordcount_array_values | 4s   |
| wordcount_hash         | 0.6s |

When using hashed keys, there were only 11 collisions (22 words) in a file with 33569 different words (`moby-dick.txt`).
