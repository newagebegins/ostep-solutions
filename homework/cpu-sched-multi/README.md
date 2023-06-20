## 1

Job takes all 30 time slices to complete. Working set is bigger than cache size.

## 2

With increased cache size, the job finishes faster (20 time slices).

## 3

After cache has warmed up, the job's performance has increased 2x.

## 4

The cache becomes warm after 10 time slices. Lower `warmup_time` makes the job run faster earlier.

## 5

It takes 150 time-slices. Jobs can't utilize the cache effectively, because after warming up they are being migrated to a different CPU.

## 6

Job "a" finishes first, because it runs alone on CPU0. Jobs "b" and "c" share CPU1, so it takes 10 time slices each (20 total) to warm up their caches, and with warm caches it takes them 90/2 time slices each to finish. Total time is `10*2 + (90/2)*2 = 110`.

To better utilize cache "b" and "c" must run on the same CPU. This way their working sets fit into cache (50+50=100).

## 7

Time to finish, depending on the cache size (M) and the number of CPUs (n):

|     | M=50 | M=100 |
|-----|------|-------|
| n=1 | 300  | 300   |
| n=2 | 150  | 150   |
| n=3 | 100  | 55    |

With M=100,n=3 all jobs run in parallel and use warm cache, so the time is `10 (warm up) + 90/2 (warm) = 55`.

## 8

```sh
python3 ./multi.py -L a:100:100,b:100:50,c:100:50 -c -t -C -T -p -n 2
```
finishes in 100 time slices which is better than 110 in question 6. Time is better because of better load-balancing: when one of the jobs finishes, the other two jobs are placed on different CPUs.

Peek interval influences when load-balancing takes place. If the peek interval is too high, it is more likely that CPUs will do uneven amount of work.

For these 3 jobs the best performance (55 time slices) is with 3 CPUs.
