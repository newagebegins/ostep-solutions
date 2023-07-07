## 1

```sh
python3 ./disk.py -a 0 -G
```

Block | Seek | Rotate | Transfer | Total
------|------|--------|----------|------
0     | 0    | 165    | 30       | 195

```sh
python3 ./disk.py -a 6 -G
```

Block | Seek | Rotate | Transfer | Total
------|------|--------|----------|------
6     | 0    | 345    | 30       | 375

```sh
python3 ./disk.py -a 30 -G
```

Block | Seek | Rotate | Transfer | Total
------|------|--------|----------|------
30    | 80   | 265    | 30       | 375

```sh
python3 ./disk.py -a 7,30,8 -G
```

Block | Seek | Rotate | Transfer | Total
------|------|--------|----------|------
7     |   0  |  15    | 30       |  45
30    |  80  | 220    | 30       | 330
8     |  80  | 310    | 30       | 420
TOTAL | 160  | 545    | 90       | 795

```sh
python3 ./disk.py -a 10,11,12,13 -G
```

Block | Seek | Rotate | Transfer | Total
------|------|--------|----------|------
10    |  0   | 105    |  30      | 135
11    |  0   |   0    |  30      |  30
12    | 40   | 320    |  30      | 390
13    |  0   |   0    |  30      |  30
TOTAL | 40   | 425    | 120      | 585

## 5

```
python3 ./disk.py -a 7,5,22 -c -p SSTF

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   5  Seek:  0  Rotate:270  Transfer: 30  Total: 300
Block:  22  Seek: 40  Rotate: 80  Transfer: 30  Total: 150

TOTALS      Seek: 40  Rotate:365  Transfer: 90  Total: 495
```

```
python3 ./disk.py -a 7,5,22 -c -p SATF

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  22  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
Block:   5  Seek: 40  Rotate:140  Transfer: 30  Total: 210

TOTALS      Seek: 80  Rotate:175  Transfer: 90  Total: 345
```

## 6

Skew of 2 maximizes preformance for the default seek rate.

```
python3 ./disk.py -a 10,11,12,13 -c -o 2

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:125  Transfer:120  Total: 285
```

## 8

A window of ~200 is needed to maximize performance.

```
python3 ./disk.py -A 1000,-1,0 -c -p SATF -w 200 -s 25
```

When scheduling window is set to 1, the policy doesn't matter.

## 9

Starvation example (block 33 is executed last):

```
python3 ./disk.py -a 33,1,2,3,4,5,6,7,1,2,3,4,5,6,7,1,2,3,4,5,6,7 -p SATF -c
```
