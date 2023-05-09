## 1

```
python3 relocation.py -s 1

Base-and-Bounds register information:

  Base   : 0x0000363c (decimal 13884)
  Limit  : 290

Virtual Address Trace
  VA  0: 0x0000030e (decimal:  782) --> violation
  VA  1: 0x00000105 (decimal:  261) --> 261+13884=14145
  VA  2: 0x000001fb (decimal:  507) --> violation
  VA  3: 0x000001cc (decimal:  460) --> violation
  VA  4: 0x0000029b (decimal:  667) --> violation
```

```
python3 relocation.py -s 2

Base-and-Bounds register information:

  Base   : 0x00003ca9 (decimal 15529)
  Limit  : 500

Virtual Address Trace
  VA  0: 0x00000039 (decimal:   57) --> 57+15529=15586
  VA  1: 0x00000056 (decimal:   86) --> 86+15529=15615
  VA  2: 0x00000357 (decimal:  855) --> violation
  VA  3: 0x000002f1 (decimal:  753) --> violation
  VA  4: 0x000002ad (decimal:  685) --> violation
```

```
python3 relocation.py -s 3

Base-and-Bounds register information:

  Base   : 0x000022d4 (decimal 8916)
  Limit  : 316

Virtual Address Trace
  VA  0: 0x0000017a (decimal:  378) --> violation
  VA  1: 0x0000026a (decimal:  618) --> violation
  VA  2: 0x00000280 (decimal:  640) --> violation
  VA  3: 0x00000043 (decimal:   67) --> 67+8916=8983
  VA  4: 0x0000000d (decimal:   13) --> 13+8916=8929
```

## 2

```
python3 relocation.py -s 0 -n 10 -l 930 -c
```

## 3

```
max base = 16k - 1k = 15k
```
