# Chapter 38

## 1

### RAID-0 (striping)

```
 0  1  2  3 (disk)
-----------
 0  1  2  3
 4  5  6  7
 8  9 10 11
12 13 14 15
```

Mappings for RAID-0:
```
disk   = addr % num_disks
offset = addr / num_disks
```

```
python3 ./raid.py -L 0 -s 1 -n 3

1343 1
LOGICAL READ from addr:1343 size:4096
  disk = 1343 % 4 = 3, offset = 1343 / 4 = 335

7637 1
LOGICAL READ from addr:7637 size:4096
  disk = 7637 % 4 = 1, offset = 7637 / 4 = 1909

4954 1
LOGICAL READ from addr:4954 size:4096
  disk = 4954 % 4 = 2, offset = 4954 / 4 = 1238
```

### RAID-1 (mirroring)

```
0 1 2 3 (disk)
-------
0 0 1 1
2 2 3 3
4 4 5 5
6 6 7 7
8 8 9 9

Offsets:
0 -> 0
1 -> 0
2 -> 1
3 -> 1
4 -> 2
5 -> 2
6 -> 3
7 -> 3
8 -> 4
9 -> 4

Disks:
0 -> 0
1 -> 2
2 -> 1
3 -> 3
4 -> 0
5 -> 2
6 -> 1
7 -> 3
8 -> 0
9 -> 2
```

Mappings for RAID-1:
```
offset = addr / 2
disk   = 0 if addr % 4 == 0
         2 if addr % 4 == 1
         1 if addr % 4 == 2
         3 if addr % 4 == 3
```

```
python3 ./raid.py -L 1 -s 1 -n 3

1343 1
LOGICAL READ from addr:1343 size:4096
  1343 % 4 = 3
  disk = 3
  offset = 1343/2 = 671

7637 1
LOGICAL READ from addr:7637 size:4096
  7637 % 4 = 1
  disk = 2
  offset = 7637/2 = 3818

4954 1
LOGICAL READ from addr:4954 size:4096
  4954 % 4 = 2
  disk = 1
  offset = 4954/2 = 2477
```

### RAID-4

```
 0  1  2  3  4 (disk)
--------------
 0  1  2  3 P0
 4  5  6  7 P1
 8  9 10 11 P2
12 13 14 15 P3
```

Mappings for RAID-4:
```
disk   = addr % 4
offset = addr / 4
```

```
python3 ./raid.py -L 4 -s 723 -n 4 -R 1000 -D 5 -c

542 1
LOGICAL READ from addr:542 size:4096
  disk   = 542 % 4 = 2
  offset = 542 / 4 = 135

193 1
LOGICAL READ from addr:193 size:4096
  disk   = 193 % 4 = 1
  offset = 193 / 4 = 48

763 1
LOGICAL READ from addr:763 size:4096
  disk   = 763 % 4 = 3
  offset = 763 / 4 = 190

465 1
LOGICAL READ from addr:465 size:4096
  disk   = 465 % 4 = 1
  offset = 465 / 4 = 116
```

### RAID-5 left-symmetric

D0 | D1 | D2 | D3 | D4
---|----|----|----|---
0  | 1  | 2  | 3  | P0
5  | 6  | 7  | P1 | 4
10 | 11 | P2 | 8  | 9
15 | P3 | 12 | 13 | 14
P4 | 16 | 17 | 18 | 19

addr | disk | offset
-----|------|-------
0    | 0    | 0
1    | 1    | 0
2    | 2    | 0
3    | 3    | 0
4    | 4    | 1
5    | 0    | 1
6    | 1    | 1
7    | 2    | 1
8    | 3    | 2
9    | 4    | 2
10   | 0    | 2
11   | 1    | 2
12   | 2    | 3
13   | 3    | 3
14   | 4    | 3
15   | 0    | 3
16   | 1    | 4
17   | 2    | 4
18   | 3    | 4
19   | 4    | 4

Mappings for RAID-4:
```
disk   = addr % num_disks       = addr % 5
offset = addr / (num_disks - 1) = addr / 4
```

```
python3 ./raid.py -L 5 -s 1 -n 4 -R 1000 -D 5 

134 1
LOGICAL READ from addr:134 size:4096
  disk   = 134 % 5 = 4
  offset = 134 / 4 = 33

763 1
LOGICAL READ from addr:763 size:4096
  disk   = 763 % 5 = 3
  offset = 763 / 4 = 190

495 1
LOGICAL READ from addr:495 size:4096
  disk   = 495 % 5 = 0
  offset = 495 / 4 = 123

651 1
LOGICAL READ from addr:651 size:4096
  disk   = 651 % 5 = 1
  offset = 651 / 4 = 162
```

### RAID-5 left-asymmetric

| D0 | D1 | D2 | D3 | D4 |
|---:|---:|---:|---:|---:|
|  0 |  1 |  2 |  3 | P0 |
|  4 |  5 |  6 | P1 |  7 |
|  8 |  9 | P2 | 10 | 11 |
| 12 | P3 | 13 | 14 | 15 |
| P4 | 16 | 17 | 18 | 19 |
| 20 | 21 | 22 | 23 | P5 |
| 24 | 25 | 26 | P6 | 27 |

| addr | disk | offset |
|-----:|-----:|-------:|
|    0 |    0 |      0 |
|    1 |    1 |      0 |
|    2 |    2 |      0 |
|    3 |    3 |      0 |
|    4 |    0 |      1 |
|    5 |    1 |      1 |
|    6 |    2 |      1 |
|    7 |    4 |      1 |
|    8 |    0 |      2 |
|    9 |    1 |      2 |
|   10 |    3 |      2 |
|   11 |    4 |      2 |
|   12 |    0 |      3 |
|   13 |    2 |      3 |
|   14 |    3 |      3 |
|   15 |    4 |      3 |
|   16 |    1 |      4 |
|   17 |    2 |      4 |
|   18 |    3 |      4 |
|   19 |    4 |      4 |
|   20 |    0 |      5 |
|   21 |    1 |      5 |
|   22 |    2 |      5 |
|   23 |    3 |      5 |
|   24 |    0 |      6 |
|   25 |    1 |      6 |
|   26 |    2 |      6 |
|   27 |    4 |      6 |

```
offset = addr / 4
disk = ?
```

## 2

### RAID-0, chunk size 2

| D0 | D1 | D2 | D3 |
|---:|---:|---:|---:|
|  0 |  2 |  4 |  6 |
|  1 |  3 |  5 |  7 |
|  8 | 10 | 12 | 14 |
|  9 | 11 | 13 | 15 |

Mappings:
```
disk   = (A/C)%N
offset = A%C + (A/(N*C))*C

where: A - address, N - #disks, C - chunk size
```

```
python3 ./raid.py -L 0 -s 1 -n 3 -C 8k

1343 1
LOGICAL READ from addr:1343 size:4096
  disk = (1343/2)%4 = 671%4 = 3
  offs = 1343%2 + (1343/8)*2 = 1 + 167*2 = 335

7637 1
LOGICAL READ from addr:7637 size:4096
  disk = (7637/2)%4 = 2
  offs = 7637%2 + (7637/8)*2 = 1909

4954 1
LOGICAL READ from addr:4954 size:4096
  disk = (4954/2)%4 = 1
  offs = 4954%2 + (4954/8)*2 = 1238
```

### RAID-1, chunk size 2

| D0 | D1 | D2 | D3 |
|----|----|----|----|
| 0  | 0  | 2  | 2  |
| 1  | 1  | 3  | 3  |
| 4  | 4  | 6  | 6  |
| 5  | 5  | 7  | 7  |

```
python3 ./raid.py -L 1 -s 1 -n 100 -C 8k -c -R 10 
```

### RAID-4, chunk size 2

| D0 | D1 | D2 | D3 | D4 |
|----|----|----|----|----|
| 0  | 2  | 4  | 6  | P0 |
| 1  | 3  | 5  | 7  | P1 |
| 8  | 10 | 12 | 14 | P2 |
| 9  | 11 | 13 | 15 | P3 |

```
python3 ./raid.py -L 4 -s 1 -n 100 -C 8k -c -R 10 -D 5
```

### RAID-5 left-symmetric, chunk size 2

D0 | D1 | D2 | D3 | D4
---|----|----|----|---
0  | 2  | 4  | 6  | P0
1  | 3  | 5  | 7  | P1
10 | 12 | 14 | P2 | 8
11 | 13 | 15 | P3 | 9
20 | 22 | P4 | 16 | 18
21 | 23 | P5 | 17 | 19

```
python3 ./raid.py -L 5 -s 1 -n 4 -R 30 -n 100 -D 5 -C 8k -c
```

### RAID-5 left-asymmetric, chunk size 2

D0 | D1 | D2 | D3 | D4
---|----|----|----|---
0  | 2  | 4  | 6  | P0
1  | 3  | 5  | 7  | P1
8  | 10 | 12 | P2 | 14
9  | 11 | 13 | P3 | 15
16 | 18 | P4 | 20 | 22
17 | 19 | P5 | 21 | 23

```
python3 ./raid.py -L 5 -s 1 -n 4 -R 30 -n 100 -D 5 -C 8k -c -5 LA
```

## 3

### RAID-0

```
 0  1  2  3 (disk)
-----------
 0  1  2  3
 4  5  6  7
 8  9 10 11
12 13 14 15
```

```
addr = disk + offset * num_disks
```

```
python3 ./raid.py -L 0 -s 1 -n 3 -r

LOGICAL OPERATION is 3+335*4 = 1343
  read  [disk 3, offset 335]  

LOGICAL OPERATION is 1+1909*4 = 7637
  read  [disk 1, offset 1909]  

LOGICAL OPERATION is 2+1238*4 = 4954
  read  [disk 2, offset 1238]  
```

## 5

RAID-0:
```
python3 ./raid.py -s 1 -n 100 -t -c -L 0

disk:0  busy:  90.21  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:1  busy:  88.13  I/Os:    23 (sequential:0 nearly:0 random:23)
disk:2  busy:  88.54  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:3  busy: 100.00  I/Os:    27 (sequential:0 nearly:1 random:26)

STAT totalTime 263.5999999999999
```

RAID-1:
```
python3 ./raid.py -s 1 -n 100 -t -c -L 1

disk:0  busy:  91.95  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:1  busy:  88.62  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:2  busy:  87.79  I/Os:    23 (sequential:0 nearly:0 random:23)
disk:3  busy: 100.00  I/Os:    27 (sequential:0 nearly:1 random:26)

STAT totalTime 264.5999999999999
```

RAID-4:
```
python3 ./raid.py -s 1 -n 100 -t -c -L 4

disk:0  busy:  66.58  I/Os:    28 (sequential:0 nearly:2 random:26)
disk:1  busy:  75.61  I/Os:    31 (sequential:0 nearly:0 random:31)
disk:2  busy: 100.00  I/Os:    41 (sequential:0 nearly:0 random:41)
disk:3  busy:   0.00  I/Os:     0 (sequential:0 nearly:0 random:0)

STAT totalTime 414.10000000000025
```

RAID-5:
```
python3 ./raid.py -s 1 -n 100 -t -c -L 5

disk:0  busy:  90.75  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:1  busy:  88.03  I/Os:    23 (sequential:0 nearly:0 random:23)
disk:2  busy:  88.59  I/Os:    25 (sequential:0 nearly:2 random:23)
disk:3  busy: 100.00  I/Os:    27 (sequential:0 nearly:1 random:26)

STAT totalTime 263.8999999999999
```

RAID-4 is slower because only 3 disks are used for reading (4th is used to store parity data).

## 6

Time to complete 100 **random** reads depending on L (RAID level) and D (number of disks):

|     | D=4 | D=8 | D=16 | D=32 |
|-----|-----|-----|------|------|
| L=0 | 263 | 140 | 75   | 46   |
| L=1 | 264 | 142 | 82   | 54   |
| L=4 | 414 | 187 | 97   | 72   |
| L=5 | 263 | 141 | 76   | 47   |

Example run:
```
python3 ./raid.py -s 1 -n 100 -t -c -L 5 -D 32
```

Performance scales approximately linearly.

## 7

Time to complete 100 **random** writes depending on L (RAID level) and D (number of disks):

|     | D=4 | D=8 | D=16 | D=32 |
|-----|-----|-----|------|------|
| L=0 | 263 | 140 | 75   | 46   |
| L=1 | 492 | 263 | 140  | 76   |
| L=4 | 977 | 957 | 885  | 756  |
| L=5 | 552 | 296 | 185  | 89   |

Example run:
```
python3 ./raid.py -s 1 -n 100 -t -c -w 100 -L 0 -D 4
```

RAID-0 has the best write performance. RAID-1 is 2 times slower than RAID-0, because of redundant writes. RAID-4 is slowest, because of parity disk bottleneck. RAID-5 has similar performance to RAID-1.

## 8

Time to complete 100 **sequential** reads depending on L (RAID level) and D (number of disks):

|     | D=4 | D=8 | D=16 | D=32 |
|-----|-----|-----|------|------|
| L=0 | 12  | 11  | 10   | 10   |
| L=1 | 14  | 12  | 11   | 10   |
| L=4 | 13  | 11  | 10   | 10   |
| L=5 | 13  | 11  | 10   | 10   |

Example run:
```
python3 ./raid.py -W seq -s 1 -n 100 -t -c -L 0 -D 4
```

Time to complete 100 **sequential** writes depending on L (RAID level) and D (number of disks):

|     | D=4 | D=8 | D=16 | D=32 |
|-----|-----|-----|------|------|
| L=0 | 12  | 11  | 10   | 10   |
| L=1 | 14  | 12  | 11   | 10   |
| L=4 | 13  | 11  | 10   | 10   |
| L=5 | 13  | 11  | 10   | 10   |

Example run:
```
python3 ./raid.py -W seq -s 1 -n 100 -t -c -w 100 -L 0 -D 4
```

For sequential reads/writes performance is similar for all RAID levels.

---

Time to complete 100 **sequential** reads depending on L (RAID level) and S (size of request), with 4 disks:

|     | S=4k | S=8k | S=16k | S=32k |
|-----|------|------|-------|-------|
| L=0 | 12   | 14   | 20    | 30    |
| L=1 | 14   | 19   | 29    | 49    |
| L=4 | 13   | 16   | 23    | 36    |
| L=5 | 13   | 16   | 23    | 36    |

Example run:
```
python3 ./raid.py -W seq -s 1 -n 100 -t -c -L 0 -D 4 -S 4k
```

Time to complete 100 **sequential** writes depending on L (RAID level) and S (size of request), with 4 disks:

|     | S=4k | S=8k | S=16k | S=32k |
|-----|------|------|-------|-------|
| L=0 | 12   | 14   | 20    | 30    |
| L=1 | 14   | 20   | 30    | 50    |
| L=4 | 13   | 16   | 23    | 36    |
| L=5 | 13   | 16   | 23    | 36    |

Example run:
```
python3 ./raid.py -w 100 -W seq -s 1 -n 100 -t -c -L 0 -D 4 -S 4k
```

It seems like bigger request sizes are better for performance. If we compare 4k and 8k request sizes for L=5, we see that we get 2 times as much data, but spend only 20% more time.
