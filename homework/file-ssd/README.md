# Chapter 44 (Flash-based SSDs)

## 1

> First, run with flags -T log -s 1 -n 10 -q. Can you figure out which operations took place?

```
python3 ./ssd.py -T log -s 1 -n 10 -q

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   0:: write(12, u) -> success

FTL    12:  0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vEEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  u
Live  +

cmd   1:: write(32, M) -> success

FTL    12:  0  32:  1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM
Live  ++

cmd   2:: read(32) -> M

FTL    12:  0  32:  1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM
Live  ++

cmd   3:: write(38, 0) -> success

FTL    12:  0  32:  1  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0
Live  +++

cmd   4:: write(36, e) -> success

FTL    12:  0  32:  1  36:  3  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  ++++

cmd 5:: trim(36) -> success

FTL    12:  0  32:  1  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  +++

cmd   6:: read(32) -> M

FTL    12:  0  32:  1  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  +++

cmd 7:: trim(32) -> success

FTL    12:  0  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  + +

cmd   8:: read(12) -> u

FTL    12:  0  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  + +

cmd   9:: read(12) -> u

FTL    12:  0  38:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  uM0e
Live  + +
```

> Use different values of -s to generate different random workloads.

```
python3 ./ssd.py -T log -s 123 -n 10 -q

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   0:: write(5, T) -> success

FTL     5:  0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vEEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T
Live  +

cmd   1:: write(45, 5) -> success

FTL     5:  0  45:  1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5
Live  ++

cmd   2:: read(5) -> T

FTL     5:  0  45:  1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5
Live  ++

cmd 3:: trim(45) -> success

FTL     5:  0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5
Live  +

cmd   4:: write(17, N) -> success

FTL     5:  0  17:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5N
Live  + +

cmd   5:: read(17) -> N

FTL     5:  0  17:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5N
Live  + +

cmd   6:: write(15, o) -> success

FTL     5:  0  15:  3  17:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5No
Live  + ++

cmd   7:: write(25, G) -> success

FTL     5:  0  15:  3  17:  2  25:  4
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5NoG
Live  + +++

cmd   8:: write(33, T) -> success

FTL     5:  0  15:  3  17:  2  25:  4  33:  5
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvvEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5NoGT
Live  + ++++

cmd   9:: write(33, n) -> success

FTL     5:  0  15:  3  17:  2  25:  4  33:  6
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvvvEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  T5NoGTn
Live  + +++ +
```

## 2

```
python3 ./ssd.py -T log -s 2 -n 10 -C

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   0:: write(36, F) -> success

FTL   36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vEEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F
Live  +

cmd   1:: write(29, 9) -> success

FTL   29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9
Live  ++

cmd   2:: write(19, I) -> success

FTL   29: 1  19: 2  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9I
Live  +++

cmd   3:: trim(19) -> success

FTL   29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9I
Live  ++

cmd   4:: write(22, g) -> success

FTL   22: 3  29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9Ig
Live  ++ +

cmd   5:: read(29) -> 9

FTL   22: 3  29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9Ig
Live  ++ +

cmd   6:: read(22) -> g

FTL   22: 3  29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9Ig
Live  ++ +

cmd   7:: write(28, e) -> success

FTL   22: 3  28: 4  29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9Ige
Live  ++ ++

cmd   8:: read(36) -> F

FTL   22: 3  28: 4  29: 1  36: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9Ige
Live  ++ ++

cmd   9:: write(49, F) -> success

FTL    22:  3  28:  4  29:  1  36:  0  49:  5
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvvEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  F9IgeF
Live  ++ +++
```

```
python3 ./ssd.py -T log -s 99 -n 10 -C

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   0:: write(10, b) -> success

FTL   10: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vEEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  b
Live  +

cmd   1:: write(26, W) -> success

FTL   10: 0  26: 1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bW
Live  ++

cmd   2:: write(21, C) -> success

FTL   10: 0  21: 2  26: 1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWC
Live  +++

cmd   3:: read(26) -> W

FTL   10: 0  21: 2  26: 1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWC
Live  +++

cmd   4:: read(10) -> b

FTL   10: 0  21: 2  26: 1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWC
Live  +++

cmd   5:: write(42, l) -> success

FTL   10: 0  21: 2  26: 1  42: 3
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWCl
Live  ++++

cmd   6:: trim(21) -> success

FTL   10: 0  26: 1  42: 3
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWCl
Live  ++ +

cmd   7:: write(23, o) -> success

FTL   10: 0  23: 4  26: 1  42: 3
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWClo
Live  ++ ++

cmd   8:: read(10) -> b

FTL   10: 0  23: 4  26: 1  42: 3
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWClo
Live  ++ ++

cmd   9:: trim(42) -> success

FTL    10:  0  23:  4  26:  1
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  bWClo
Live  ++  +
```

## 3

```
python3 ./ssd.py -T log -s 2 -n 10 -C -r 20

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   0:: read(41) -> fail: uninitialized read

FTL   (empty)
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data
Live

cmd   1:: write(33, j) -> success

FTL   33: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vEEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  j
Live  +

cmd   2:: write(30, A) -> success

FTL   30: 1  33: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jA
Live  ++

cmd   3:: read(33) -> j

FTL   30: 1  33: 0
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvEEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jA
Live  ++

cmd   4:: write(49, W) -> success

FTL   30: 1  33: 0  49: 2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvEEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAW
Live  +++

cmd   5:: write(22, g) -> success

FTL   22: 3  30: 1  33: 0  49: 2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAWg
Live  ++++

cmd   6:: read(23) -> fail: uninitialized read

FTL   22: 3  30: 1  33: 0  49: 2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAWg
Live  ++++

cmd   7:: read(22) -> g

FTL   22: 3  30: 1  33: 0  49: 2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAWg
Live  ++++

cmd   8:: write(28, e) -> success

FTL   22: 3  28: 4  30: 1  33: 0  49: 2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAWge
Live  +++++

cmd   9:: read(33) -> j

FTL    22:  3  28:  4  30:  1  33:  0  49:  2
Block 0          1          2          3          4          5          6
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
State vvvvvEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii
Data  jAWge
Live  +++++
```

> What differences does this cause in the commands?

20% of reads fail.

## 4

```
python3 ./ssd.py -T log -s 1 -n 10            

FTL   (empty)
Block 0          1          2          3          4          5          6          
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666 
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii 
Data                                                                               
Live                                                                               


FTL    12:  0  38:  2 
Block 0          1          2          3          4          5          6          
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666 
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 
State vvvvEEEEEE iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii 
Data  uM0e                                                                         
Live  + +                                                                          

1000 + 4*40 + 2*0 + 4*10 = 1200 microseconds
```

## 5

```
python3 ./ssd.py -T direct -s 1 -n 10 -S      

FTL   (empty)
Block 0          1          2          3          4          5          6          
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666 
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 
State iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii iiiiiiiiii 
Data                                                                               
Live                                                                               


FTL    12: 12  38: 38 
Block 0          1          2          3          4          5          6          
Page  0000000000 1111111111 2222222222 3333333333 4444444444 5555555555 6666666666 
      0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 
State iiiiiiiiii EEvEEEEEEE iiiiiiiiii EEvEEEvEvE iiiiiiiiii iiiiiiiiii iiiiiiiiii 
Data               u                     M   e 0                                   
Live               +                           +                                   

Physical Operations Per Block
Erases   0          1          0          3          0          0          0          Sum: 4
Writes   0          1          0          6          0          0          0          Sum: 7
Reads    0          2          0          5          0          0          0          Sum: 7

Logical Operation Sums
  Write count 4 (0 failed)
  Read count  4 (0 failed)
  Trim count  2 (0 failed)

Times
  Erase time 4000.00
  Write time 280.00
  Read time  70.00
  Total time 4350.00
```

> In general, how much better will the log-structured approach perform than the direct one?

Log-structured approach is much better, because it results in less erases and writes.

## 6

> What do you think will happen?

After 60 writes the SSD becomes full and subsequent writes fail.

## 7

> What watermark values do you think will make for a working system?

Theses values work well (start collecting garbage when all blocks become used and free a single block):

```
python3 ./ssd.py -T log -n 1000 -C -F -G 7 -g 6 -S
```

## 8

Garbage collection produces many additional erases, reads and writes, but log-structured approach with GC still provides much better performance than the direct approach.

## 9

> What do you expect will happen?

Skew has bad effect for the direct approach (some blocks wear out much faster than others), but it doesn't affect the log-structured approach.
