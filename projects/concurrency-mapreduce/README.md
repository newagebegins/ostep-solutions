# Comparison of different implementations of MapReduce

* `wordcount` (uses `mapreduce.c`) — stores keys and values in linked lists, keys are kept sorted during inserts
* `wordcount_array` (uses `mapreduce_array.c`) — same as `wordcount`, but stores keys in growable arrays
* `wordcount_array_values` (uses `mapreduce_array_values.c`) — same as `wordcount_array`, but also stores values in growable arrays
* `wordcount_hash` (uses `mapreduce_hash.c`) — same as `wordcount_array`, but uses key hashes instead of `strcmp` (`strcmp` is still used for sorting). There were 11 collisions (22 words) in a file with 33569 different words (`moby-dick.txt`).
* `wordcount_big_files_first` (uses `mapreduce_big_files_first.c`) — same as `wordcount_array`, but schedules big files first (observed wins are insignificant)
* `wordcount_hash_table` (uses `mapreduce_hash_table.c`) - like `wordcount`, but uses hash table to drastically reduce the number of string comparisons. Also it sorts keys as a separate step in reducer. Unlike `wordcount_hash`, no data is lost because of hash collisions.

Example test command:
```
time ./wordcount moby-dick.txt hamlet.txt > tmp.txt
```

All programs are compiled with `-O2` optimizations, use 4 mappers and 4 reducers.

| Program                | Time  |
|------------------------|-------|
| wordcount              | 20s   |
| wordcount_array        | 4.5s  |
| wordcount_array_values | 4s    |
| wordcount_hash         | 0.6s  |
| wordcount_hash_table   | 0.2s  |
