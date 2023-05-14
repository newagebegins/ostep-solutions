## 1

One register is needed to locate page table directory (for any-level).

```
python3 ./paging-multilevel-translate.py -s 0

Virt addr: 15 bits (5 offset, 10 VPN)
Phys addr: 12 bits (5 offset,  7 PFN)

PDBR: 108  (decimal) [This means the page directory is held in this page]

page 108:
00 83
01 fe
02 e0 [1 (valid) 110 0000 = 0x60 = 96]
03 da
04 7f
05 d4 [1 (valid) 101 0100 = 0x54 = 84]
06 7f
07 eb
08 be
09 9e [1 (valid) 001 1110 = 0x1E = 16+14 = 30]
10 d5
11 ad
12 e4
13 ac
14 90
15 d6 [1 (valid) 101 0110 = 0x56 = 86]
16 92
17 d8
18 c1
19 f8 [1 (valid) 111 1000 = 0x78 = 120]
20 9f
21 e1
22 ed
23 e9
24 a1 [1 (valid) 0100001 = 33]
25 e8
26 c7
27 c2
28 a9
29 d1
30 db
31 ff [1 (valid) 111 1111 = 0x7F = 127]

Virtual Address 611c:
11000 [24] 01000 [8] 11100 [28]
b5 = 1 (valid) 0110101 (pfn = 53)
page  53:0f0c18090e121c0f081713071c1e191b09161b150e030d121c1d0e1a08181100
translates to 0110 1011 1100 = 0x6BC, fetches 08

Virtual Address 3da8:
01111 [15] 01101 [13] 01000
page  86:7f7f7f7f7f7f7fc57f7f7f7f7f7f7f7f7f7f7f7fca7f7fee7f7f7f7f7f7f7f7f
7f = 0111 1111
invalid

Virtual Address 17f5:
00101 [5] 11111 [0x1F = 31] 10101 [0x15 = 21]
page  84:7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f947f7f7f7f7fce
ce = 1 (valid) 1001110 (pfn, 0x4e = 78)
page  78:0e02171b1c1a1b1c100c1508191a1b121d110d141e1c1802120f131a07160306
translates to 1001 1101 0101 = 0x9D5, fetches 1c

Virtual Address 7f6c:
11111 [31] 11011 [0x1B = 27] 01100
page 127:7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fdf7f7f7f7f7f7f7f7f7f7f7f7f957f7f
7f = 0111 1111
invalid

Virtual Address 0bad:
00010 [2] 11101 [0x1D = 29] 01101
page  96:7f7f7f7f7f7f7f7f7f7f7f7f7f7fb67f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f
7f
invalid

Virtual Address 6d60:
11011 [0x1B = 27] 01011 00000
page  27:0000000000000000000000000000000000000000000000000000000000000000
invalid

Virtual Address 2a5b:
01010 [10] 10010 11011
page  10:0000000000000000000000000000000000000000000000000000000000000000
invalid

Virtual Address 4c5e:
10011 [0x13 = 19] 00010 [2] 11110
page 120:7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fcb7f7f7f7f7f7f7f7f7f7f7f7f7f
7f
invalid

Virtual Address 2592:
01001 [9] 01100 [0xC = 12] 10010 [0x12 = 18]
page  30:7f7f7f7f7f847f7f7f7f977fbd7f7ff47f7f7f7f7f7f7f7f7f7f7f7f7f7f9c7f
bd = 1 (valid) 011 1101 = 0x3D = 3*16+13 = 30+30+1 = 61
page  61:010510020c0a0c031c0e1a1e0a0e150d09161b1c130b1e1302021701000c100d
translates to 0111 1011 0010 = 0x7B2, fetches 1b

Virtual Address 3e99:
01111 [15] 10100 [0x14 = 20] 11001 [0x19 = 25]
page  86:7f7f7f7f7f7f7fc57f7f7f7f7f7f7f7f7f7f7f7fca7f7fee7f7f7f7f7f7f7f7f
ca = 1 (valid) 100 1010 = 0x4A = 4*16+10 = 40+24+10 = 74
page  74:0d0b1e08180d0b011a151b0d14030c06011d0604060b10041e1e040c151b0f1c
translates to 1001 0101 1001 = 0x959, fetches 1e
```

Maximum of 3 fetches are needed to perform each lookup:
1. page directory
2. page table
3. data
