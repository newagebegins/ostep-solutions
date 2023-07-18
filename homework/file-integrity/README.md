# Chapter 45 (Data Integrity and Protection)

## 1

```
python3 ./checksum.py

Decimal:          216        194        107         66 
Hex:             0xd8       0xc2       0x6b       0x42 
Bin:       0b11011000 0b11000010 0b01101011 0b01000010 

Add:      (216+194+107+66) % 256 = 583 % 256 = 71
Xor:      11011000
          11000010
          01101011
          01000010
          --------
          00110011 = 1+2+16+32 = 51
Fletcher: s1 = 0, s2 = 0
          s1 = (0 + 216) % 255 = 216
          s2 = (0 + 216) % 255 = 216
          s1 = (216 + 194) % 255 = 155
          s2 = (216 + 155) % 255 = 116
          s1 = (155 + 107) % 255 = 7
          s2 = (116 + 7) % 255 = 123
          s1 = (7 + 66) % 255 = 73
          s2 = (123 + 73) % 255 = 196
```

## 2

```
python3 ./checksum.py -s 123

Decimal:           13         22        104         27 
Hex:             0x0d       0x16       0x68       0x1b 
Bin:       0b00001101 0b00010110 0b01101000 0b00011011 

Add:      (13+22+104+27)%256 = 166
Xor:      0b00001101
          0b00010110
          0b01101000
          0b00011011
          ----------
          0b01101000 = 8+32+64 = 104
Fletcher: s1 = (0+13)%255 = 13
          s2 = (0+13)%255 = 13
          s1 = (13+22)%255 = 35
          s2 = (13+35)%255 = 48
          s1 = (35+104)%255 = 139
          s2 = (48+139)%255 = 187
          s1 = (139+27)%255 = 166
          s2 = (187+166)%255 = 98
```

```
python3 ./checksum.py -s 124   

Decimal:          245        140         45        247 
Hex:             0xf5       0x8c       0x2d       0xf7 
Bin:       0b11110101 0b10001100 0b00101101 0b11110111 

Add:      (245+140+45+247)%256 = 165
Xor:      0b11110101
          0b10001100
          0b00101101
          0b11110111 
          ----------
          0b10100011 = 1+2+32+128 = 163
Fletcher: s1 = (0+245)%255 = 245
          s2 = (0+245)%255 = 245
          s1 = (245+140)%255 = 130
          s2 = (245+130)%255 = 120
          s1 = (130+45)%255 = 175
          s2 = (120+175)%255 = 40
          s1 = (175+247)%255 = 167
          s2 = (40+167)%255 = 207
```

## 3

```
python3 ./checksum.py -D 1,2,4,8 -c

Decimal:            1          2          4          8 
Hex:             0x01       0x02       0x04       0x08 
Bin:       0b00000001 0b00000010 0b00000100 0b00001000 

Add:             15       (0b00001111)
Xor:             15       (0b00001111)
Fletcher(a,b):   15, 26   (0b00001111,0b00011010)
```

> In general, when does this occur?

When carry is 0 for each binary place.

## 4

```
python3 ./checksum.py -D 1,2,4,9 -c

OPTIONS seed 0
OPTIONS data_size 4
OPTIONS data 1,2,4,9

Decimal:            1          2          4          9 
Hex:             0x01       0x02       0x04       0x09 
Bin:       0b00000001 0b00000010 0b00000100 0b00001001 

Add:             16       (0b00010000)
Xor:             14       (0b00001110)
Fletcher(a,b):   16, 27   (0b00010000,0b00011011)
```

> In general, when does this occur?

When at least one carry is 1 for some binary place.

## 5

```
python3 ./checksum.py -D 6,5,3,4 -c

Decimal:            6          5          3          4 
Hex:             0x06       0x05       0x03       0x04 
Bin:       0b00000110 0b00000101 0b00000011 0b00000100 

Add:             18       (0b00010010)
Xor:              4       (0b00000100)
Fletcher(a,b):   18, 49   (0b00010010,0b00110001)
```

```
python3 ./checksum.py -D 10,1,2,5 -c

Decimal:           10          1          2          5 
Hex:             0x0a       0x01       0x02       0x05 
Bin:       0b00001010 0b00000001 0b00000010 0b00000101 

Add:             18       (0b00010010)
Xor:             12       (0b00001100)
Fletcher(a,b):   18, 52   (0b00010010,0b00110100)
```

> In general, when will the additive checksum be the same, even though the data values are different?

When:

```
(a1 + b1 + c1 + d1) % 256 = (a2 + b2 + c2 + d2) % 256
```

Or equivalently:

```
a1 + b1 + c1 + d1 = a2 + b2 + c2 + d2 (mod 256)
```

## 6

```
python3 ./checksum.py -D 1,2,4,8 -c    

Decimal:            1          2          4          8 
Hex:             0x01       0x02       0x04       0x08 
Bin:       0b00000001 0b00000010 0b00000100 0b00001000 

Add:             15       (0b00001111)
Xor:             15       (0b00001111)
Fletcher(a,b):   15, 26   (0b00001111,0b00011010)
```

```
python3 ./checksum.py -D 7,131,6,141 -c

Decimal:            7        131          6        141 
Hex:             0x07       0x83       0x06       0x8d 
Bin:       0b00000111 0b10000011 0b00000110 0b10001101 

Add:             29       (0b00011101)
Xor:             15       (0b00001111)
Fletcher(a,b):   30, 64   (0b00011110,0b01000000)
```

> In general, when will the XOR checksum be the same, even though the data values are different?

When each binary position has the same parity (odd or even number of ones).

## 7

```
python3 ./checksum.py -D 1,2,3,4       

Decimal:            1          2          3          4 
Hex:             0x01       0x02       0x03       0x04 
Bin:       0b00000001 0b00000010 0b00000011 0b00000100 

Add:      (1+2+3+4)%256 = 10
Xor:      0b00000001
          0b00000010
          0b00000011
          0b00000100
          ----------
          0b00000100 = 4
Fletcher: s1 = (0+1)%255 = 1
          s2 = (0+1)%255 = 1
          s1 = 1+2 = 3
          s2 = 1+3 = 4
          s1 = 3+3 = 6
          s2 = 4+6 = 10
          s1 = 6+4 = 10
          s2 = 10+10 = 20
```

```
python3 ./checksum.py -D 4,3,2,1   

Decimal:            4          3          2          1 
Hex:             0x04       0x03       0x02       0x01 
Bin:       0b00000100 0b00000011 0b00000010 0b00000001 

Add:      10
Xor:      4
Fletcher: s1 = 0+4 = 4
          s2 = 0+4 = 4
          s1 = 4+3 = 7
          s2 = 4+7 = 11
          s1 = 7+2 = 9
          s2 = 11+9 = 20
          s1 = 9+1 = 10
          s2 = 20+10 = 30
```

> What do you notice about these three checksums? How does Fletcher compare to the other two?

For Add and Xor order of the bytes doesn't matter, for Fletcher it does matter.

> How is Fletcher generally “better” than something like the simple additive checksum?

Fletcher can detect more corruptions: when bytes have correct value, but are in the wrong order.

## 8

```
python3 ./checksum.py -D 0,1,2,3 -c

Decimal:            0          1          2          3 
Hex:             0x00       0x01       0x02       0x03 
Bin:       0b00000000 0b00000001 0b00000010 0b00000011 

Add:              6       (0b00000110)
Xor:              0       (0b00000000)
Fletcher(a,b):    6, 10   (0b00000110,0b00001010)
```

```
python3 ./checksum.py -D 255,1,2,3 -c

Decimal:          255          1          2          3 
Hex:             0xff       0x01       0x02       0x03 
Bin:       0b11111111 0b00000001 0b00000010 0b00000011 

Add:              5       (0b00000101)
Xor:            255       (0b11111111)
Fletcher(a,b):    6, 10   (0b00000110,0b00001010)
```
