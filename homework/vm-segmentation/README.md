## 1

```
python3 ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0

ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000006c (decimal:  108) --> 492
  VA  1: 0x00000061 (decimal:   97) --> violation
  VA  2: 0x00000035 (decimal:   53) --> violation
  VA  3: 0x00000021 (decimal:   33) --> violation
  VA  4: 0x00000041 (decimal:   65) --> violation
```

```
python3 ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1

ARG seed 1
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x00000011 (decimal:   17) --> 17
  VA  1: 0x0000006c (decimal:  108) --> 492
  VA  2: 0x00000061 (decimal:   97) --> violation
  VA  3: 0x00000020 (decimal:   32) --> violation
  VA  4: 0x0000003f (decimal:   63) --> violation
```

```
python3 ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2

ARG seed 2
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000007a (decimal:  122) --> 506
  VA  1: 0x00000079 (decimal:  121) --> 505
  VA  2: 0x00000007 (decimal:    7) --> 7
  VA  3: 0x0000000a (decimal:   10) --> 10
  VA  4: 0x0000006a (decimal:  106) --> violation
```

## 2

What is the highest legal virtual address in segment 0?

19

What is the lowest legal virtual address in segment 1?

108

What are the lowest and highest illegal addresses in this entire address space?

20..107

Test with:

```
python3 ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 -A 19,20,107,108 -c
```

## 3

```
# specific values for b0 and b1 don't matter
segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 55 --l0 2 --b1 70 --l1 2
```

## 4

```
l0 + l1 = 0.9 * a (where l0,l1 - limits, a - address space)
```

## 5

Set l0=0 and l1=0
