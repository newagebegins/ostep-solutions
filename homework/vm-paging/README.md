## 1

As the address space grows page-table size grows.

As the page size grows page-table size shrinks.

Big pages cause more internal fragmentation.


## 2

```
python3 ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0

Page Table (from entry 0 down to the max size)
  [       0]  0x00000000
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x00000000
  [       4]  0x00000000
  [       5]  0x00000000
  [       6]  0x00000000
  [       7]  0x00000000
  [       8]  0x00000000
  [       9]  0x00000000
  [      10]  0x00000000
  [      11]  0x00000000
  [      12]  0x00000000
  [      13]  0x00000000
  [      14]  0x00000000
  [      15]  0x00000000

  VA 0x00003a39 (decimal:    14905) --> invalid
  VA 0x00003ee5 (decimal:    16101) --> invalid
  VA 0x000033da (decimal:    13274) --> invalid
  VA 0x000039bd (decimal:    14781) --> invalid
  VA 0x000013d9 (decimal:     5081) --> invalid
```

```
python3 ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25
ARG seed 0
ARG address space size 16k
ARG phys mem size 32k
ARG page size 1k
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x80000018
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x00000000
  [       4]  0x00000000
  [       5]  0x80000009
  [       6]  0x00000000
  [       7]  0x00000000
  [       8]  0x80000010
  [       9]  0x00000000
  [      10]  0x80000013
  [      11]  0x00000000
  [      12]  0x8000001f
  [      13]  0x8000001c
  [      14]  0x00000000
  [      15]  0x00000000

Virtual Address Trace
  VA 0x00003986 (decimal:    14726) --> invalid
  VA 0x00002bc6 (decimal:    11206) --> 0x4FC6
  VA 0x00001e37 (decimal:     7735) --> invalid
  VA 0x00000671 (decimal:     1649) --> invalid
  VA 0x00001bc9 (decimal:     7113) --> invalid
```

```
python3 ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50
ARG seed 0
ARG address space size 16k
ARG phys mem size 32k
ARG page size 1k
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x80000018
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x8000000c
  [       4]  0x80000009
  [       5]  0x00000000
  [       6]  0x8000001d
  [       7]  0x80000013
  [       8]  0x00000000
  [       9]  0x8000001f
  [      10]  0x8000001c
  [      11]  0x00000000
  [      12]  0x8000000f
  [      13]  0x00000000
  [      14]  0x00000000
  [      15]  0x80000008

Virtual Address Trace
  VA 0x00003385 (decimal:    13189) --> 0x3F85
  VA 0x0000231d (decimal:     8989) --> invalid
  VA 0x000000e6 (decimal:      230) --> 0x60E6
  VA 0x00002e0f (decimal:    11791) --> invalid
  VA 0x00001986 (decimal:     6534) --> 0x7586
```

```
python3 ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75
ARG seed 0
ARG address space size 16k
ARG phys mem size 32k
ARG page size 1k
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x80000018
  [       1]  0x80000008
  [       2]  0x8000000c
  [       3]  0x80000009
  [       4]  0x80000012
  [       5]  0x80000010
  [       6]  0x8000001f
  [       7]  0x8000001c
  [       8]  0x80000017
  [       9]  0x80000015
  [      10]  0x80000003
  [      11]  0x80000013
  [      12]  0x8000001e
  [      13]  0x8000001b
  [      14]  0x80000019
  [      15]  0x80000000

Virtual Address Trace
  VA 0x00002e0f (decimal:    11791) --> (11) 0x4E0F
  VA 0x00001986 (decimal:     6534) --> (6)  0x7D86
  VA 0x000034ca (decimal:    13514) --> (13) 0x6CCA
  VA 0x00002ac3 (decimal:    10947) --> (10) 0xEC3
  VA 0x00000012 (decimal:       18) --> (0)  0x6012
```

```
python3 ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100
ARG seed 0
ARG address space size 16k
ARG phys mem size 32k
ARG page size 1k
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x80000018
  [       1]  0x80000008
  [       2]  0x8000000c
  [       3]  0x80000009
  [       4]  0x80000012
  [       5]  0x80000010
  [       6]  0x8000001f
  [       7]  0x8000001c
  [       8]  0x80000017
  [       9]  0x80000015
  [      10]  0x80000003
  [      11]  0x80000013
  [      12]  0x8000001e
  [      13]  0x8000001b
  [      14]  0x80000019
  [      15]  0x80000000

Virtual Address Trace
  VA 0x00002e0f (decimal:    11791) --> (11) 0x4E0F
  VA 0x00001986 (decimal:     6534) --> (6)  0x7D86
  VA 0x000034ca (decimal:    13514) --> (13) 0x6CCA
  VA 0x00002ac3 (decimal:    10947) --> (10) 0xEC3
  VA 0x00000012 (decimal:       18) --> (0)  0x6012
```

The more virtual pages are allocated, the more virtual addresses become valid.

## 3

```
python3 ./paging-linear-translate.py -P 8 -a 32 -p 1024 -v -s 1
ARG seed 1
ARG address space size 32
ARG phys mem size 1024
ARG page size 8
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x00000000
  [       1]  0x80000061
  [       2]  0x00000000
  [       3]  0x00000000

11  111
VPN offset

Virtual Address Trace
  VA 0x0000000e (decimal:       14) --> (1) 0x30E
  VA 0x00000014 (decimal:       20) --> (2) invalid
  VA 0x00000019 (decimal:       25) --> (3) invalid
  VA 0x00000003 (decimal:        3) --> (0) invalid
  VA 0x00000000 (decimal:        0) --> (0) invalid
```

```
python3 ./paging-linear-translate.py -P 8k -a 32k -p 1m -v -s 2
ARG seed 2
ARG address space size 32k
ARG phys mem size 1m
ARG page size 8k
ARG verbose True
ARG addresses -1

Page Table (from entry 0 down to the max size)
  [       0]  0x80000079
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x8000005e

(2) (13)
VPN offset

Virtual Address Trace
  VA 0x000055b9 (decimal:    21945) --> (VPN 2) [010] [1 0101 1011 1001] invalid
  VA 0x00002771 (decimal:    10097) --> (VPN 1) [001] [0 0111 0111 0001] invalid
  VA 0x00004d8f (decimal:    19855) --> (VPN 2) [010] [0 1101 1000 1111] invalid
  VA 0x00004dab (decimal:    19883) --> (VPN 2) [010] [0 1101 1010 1011] invalid
  VA 0x00004a64 (decimal:    19044) --> (VPN 2) [010] [0 1010 0110 0100] invalid
```

```
python3 ./paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3

[8] [20 = 0x14]
VPN offset

Virtual Address Trace
  VA 0x0308b24d (decimal: 50901581) --> (VPN 48) 0x1F68B24D
  VA 0x042351e6 (decimal: 69423590) --> (VPN 66) invalid
  VA 0x02feb67b (decimal: 50247291) --> (VPN 47) 0xA9EB67B
  VA 0x0b46977d (decimal: 189175677) --> (VPN 180) invalid
  VA 0x0dbcceb4 (decimal: 230477492) --> (VPN 219) 0x1F2CCEB4
```

## 4

Simulation doesn't work when address space is bigger than physical memory space.
