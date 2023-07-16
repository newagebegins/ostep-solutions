# Chapter 43 (Log-structured File Systems)

## 1

```
python3 ./lfs.py -n 3 -s 1  

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /tg4
write to /tg4
create file /lt0

FINAL file system contents:
[   0 ] live checkpoint: 13 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [tg4,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ] live type:reg size:6 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   9 ]      chunk(imap): 5 8 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  10 ] live [.,0] [..,0] [tg4,1] [lt0,2] -- -- -- -- 
[  11 ] live type:dir size:1 refs:2 ptrs: 10 -- -- -- -- -- -- -- 
[  12 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  13 ] live chunk(imap): 11 8 12 -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

```
python3 ./lfs.py -n 3 -s 2      

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /vt6
write to /vt6
create file /ks9

FINAL file system contents:
[   0 ] live checkpoint: 17 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [vt6,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ] live y0y0y0y0y0y0y0y0y0y0y0y0y0y0y0y0
[   9 ] live o1o1o1o1o1o1o1o1o1o1o1o1o1o1o1o1
[  10 ] live l2l2l2l2l2l2l2l2l2l2l2l2l2l2l2l2
[  11 ] live g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3
[  12 ] live type:reg size:8 refs:1 ptrs: -- -- -- -- 8 9 10 11 
[  13 ]      chunk(imap): 5 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  14 ] live [.,0] [..,0] [vt6,1] [ks9,2] -- -- -- -- 
[  15 ] live type:dir size:1 refs:2 ptrs: 14 -- -- -- -- -- -- -- 
[  16 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  17 ] live chunk(imap): 15 12 16 -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

```
python3 ./lfs.py -n 5 -s 3      

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /jp6
create file /vg2
link /jp6 to /mq1
mkdir /nt6
create file /nt6/ph0

FINAL file system contents:
[   0 ] live checkpoint: 24 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [jp6,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ]      [.,0] [..,0] [jp6,1] [vg2,2] -- -- -- -- 
[   9 ]      type:dir size:1 refs:2 ptrs: 8 -- -- -- -- -- -- -- 
[  10 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  11 ]      chunk(imap): 9 6 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  12 ]      [.,0] [..,0] [jp6,1] [vg2,2] [mq1,1] -- -- -- 
[  13 ]      type:dir size:1 refs:2 ptrs: 12 -- -- -- -- -- -- -- 
[  14 ] live type:reg size:0 refs:2 ptrs: -- -- -- -- -- -- -- -- 
[  15 ]      chunk(imap): 13 14 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  16 ] live [.,0] [..,0] [jp6,1] [vg2,2] [mq1,1] [nt6,3] -- -- 
[  17 ]      [.,3] [..,0] -- -- -- -- -- -- 
[  18 ] live type:dir size:1 refs:3 ptrs: 16 -- -- -- -- -- -- -- 
[  19 ]      type:dir size:1 refs:2 ptrs: 17 -- -- -- -- -- -- -- 
[  20 ]      chunk(imap): 18 14 10 19 -- -- -- -- -- -- -- -- -- -- -- -- 
[  21 ] live [.,3] [..,0] [ph0,4] -- -- -- -- -- 
[  22 ] live type:dir size:1 refs:2 ptrs: 21 -- -- -- -- -- -- -- 
[  23 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  24 ] live chunk(imap): 18 14 10 22 23 -- -- -- -- -- -- -- -- -- -- -- 
```

```
python3 ./lfs.py -n 5 -s 4      

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /ke0
write to /ke0
create file /he1
create file /vu8
create file /qt8

FINAL file system contents:
[   0 ] live checkpoint: 23 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [ke0,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ] live w0w0w0w0w0w0w0w0w0w0w0w0w0w0w0w0
[   9 ] live c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1
[  10 ] live type:reg size:8 refs:1 ptrs: -- -- -- -- -- -- 8 9 
[  11 ]      chunk(imap): 5 10 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  12 ]      [.,0] [..,0] [ke0,1] [he1,2] -- -- -- -- 
[  13 ]      type:dir size:1 refs:2 ptrs: 12 -- -- -- -- -- -- -- 
[  14 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  15 ]      chunk(imap): 13 10 14 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  16 ]      [.,0] [..,0] [ke0,1] [he1,2] [vu8,3] -- -- -- 
[  17 ]      type:dir size:1 refs:2 ptrs: 16 -- -- -- -- -- -- -- 
[  18 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  19 ]      chunk(imap): 17 10 14 18 -- -- -- -- -- -- -- -- -- -- -- -- 
[  20 ] live [.,0] [..,0] [ke0,1] [he1,2] [vu8,3] [qt8,4] -- -- 
[  21 ] live type:dir size:1 refs:2 ptrs: 20 -- -- -- -- -- -- -- 
[  22 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  23 ] live chunk(imap): 21 10 14 18 22 -- -- -- -- -- -- -- -- -- -- -- 
```

> How much harder does the task become for you as you increase the number of commands issued (i.e., change -n 3 to -n 5)

Not much harder, just longer.

## 2

```
python3 ./lfs.py -n 3 -i -s 5   

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

mkdir /uy7

[   0 ] ?    checkpoint: 8 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   4 ] ?    [.,0] [..,0] [uy7,1] -- -- -- -- -- 
[   5 ] ?    [.,1] [..,0] -- -- -- -- -- -- 
[   6 ] ?    type:dir size:1 refs:3 ptrs: 4 -- -- -- -- -- -- -- 
[   7 ] ?    type:dir size:1 refs:2 ptrs: 5 -- -- -- -- -- -- -- 
[   8 ] ?    chunk(imap): 6 7 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 


create file /yq9

[   0 ] ?    checkpoint: 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   9 ] ?    [.,0] [..,0] [uy7,1] [yq9,2] -- -- -- -- 
[  10 ] ?    type:dir size:1 refs:3 ptrs: 9 -- -- -- -- -- -- -- 
[  11 ] ?    type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  12 ] ?    chunk(imap): 10 7 11 -- -- -- -- -- -- -- -- -- -- -- -- -- 


create file /go5

[   0 ] ?    checkpoint: 16 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[  13 ] ?    [.,0] [..,0] [uy7,1] [yq9,2] [go5,3] -- -- -- 
[  14 ] ?    type:dir size:1 refs:3 ptrs: 13 -- -- -- -- -- -- -- 
[  15 ] ?    type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  16 ] ?    chunk(imap): 14 7 11 15 -- -- -- -- -- -- -- -- -- -- -- -- 



FINAL file system contents:
[   0 ] live checkpoint: 16 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [uy7,1] -- -- -- -- -- 
[   5 ] live [.,1] [..,0] -- -- -- -- -- -- 
[   6 ]      type:dir size:1 refs:3 ptrs: 4 -- -- -- -- -- -- -- 
[   7 ] live type:dir size:1 refs:2 ptrs: 5 -- -- -- -- -- -- -- 
[   8 ]      chunk(imap): 6 7 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   9 ]      [.,0] [..,0] [uy7,1] [yq9,2] -- -- -- -- 
[  10 ]      type:dir size:1 refs:3 ptrs: 9 -- -- -- -- -- -- -- 
[  11 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  12 ]      chunk(imap): 10 7 11 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  13 ] live [.,0] [..,0] [uy7,1] [yq9,2] [go5,3] -- -- -- 
[  14 ] live type:dir size:1 refs:3 ptrs: 13 -- -- -- -- -- -- -- 
[  15 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  16 ] live chunk(imap): 14 7 11 15 -- -- -- -- -- -- -- -- -- -- -- -- 
```

## 3

```
python3 ./lfs.py -o -F -s 100

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /us7

[   0 ] live checkpoint: 7 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   4 ] live [.,0] [..,0] [/us7,1] -- -- -- -- -- 
[   5 ] live type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ] live chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

write file  /us7 offset=4 size=0 

[   0 ] live checkpoint: 9 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   8 ] live type:reg size:4 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   9 ] live chunk(imap): 5 8 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

write file  /us7 offset=7 size=7 

[   0 ] live checkpoint: 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[  10 ] live i0i0i0i0i0i0i0i0i0i0i0i0i0i0i0i0
[  11 ] live type:reg size:8 refs:1 ptrs: -- -- -- -- -- -- -- 10 
[  12 ] live chunk(imap): 5 11 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

FINAL file system contents:
[   0 ] live checkpoint: 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ] live [.,0] [..,0] [/us7,1] -- -- -- -- -- 
[   5 ] live type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ]      type:reg size:4 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   9 ]      chunk(imap): 5 8 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  10 ] live i0i0i0i0i0i0i0i0i0i0i0i0i0i0i0i0
[  11 ] live type:reg size:8 refs:1 ptrs: -- -- -- -- -- -- -- 10 
[  12 ] live chunk(imap): 5 11 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

```
python3 ./lfs.py -o -F -s 101   

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /ym6

[   0 ] live checkpoint: 7 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   4 ] live [.,0] [..,0] [ym6,1] -- -- -- -- -- 
[   5 ] live type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ] live chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /hs2 

[   0 ] live checkpoint: 11 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[   8 ] live [.,0] [..,0] [ym6,1] [hs2,2] -- -- -- -- 
[   9 ] live type:dir size:1 refs:2 ptrs: 8 -- -- -- -- -- -- -- 
[  10 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  11 ] live chunk(imap): 9 6 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 

link file   /ym6 /cu3

[   0 ] live checkpoint: 15 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
...
[  12 ] live [.,0] [..,0] [ym6,1] [hs2,2] [cu3,1] -- -- -- 
[  13 ] live type:dir size:1 refs:2 ptrs: 12 -- -- -- -- -- -- -- 
[  14 ] live type:reg size:0 refs:2 ptrs: -- -- -- -- -- -- -- -- 
[  15 ] live chunk(imap): 13 14 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 

FINAL file system contents:
[   0 ] live checkpoint: 15 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [ym6,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ]      [.,0] [..,0] [ym6,1] [hs2,2] -- -- -- -- 
[   9 ]      type:dir size:1 refs:2 ptrs: 8 -- -- -- -- -- -- -- 
[  10 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  11 ]      chunk(imap): 9 6 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  12 ] live [.,0] [..,0] [ym6,1] [hs2,2] [cu3,1] -- -- -- 
[  13 ] live type:dir size:1 refs:2 ptrs: 12 -- -- -- -- -- -- -- 
[  14 ] live type:reg size:0 refs:2 ptrs: -- -- -- -- -- -- -- -- 
[  15 ] live chunk(imap): 13 14 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

## 4

```
python3 ./lfs.py -n 20 -s 1   

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /tg4
write to /tg4
create file /lt0
write to /lt0
link /tg4 to /oy3
create file /af4
write to /tg4 or /oy3
write to /lt0
write to /lt0
unlink /tg4
write to /af4
write to /af4
write to /af4
write to /lt0
write to /lt0
create dir /ln7
write to /oy3
create file /ln7/zp3
create file /ln7/zu5
unlink /oy3

FINAL file system contents:
[   0 ] live checkpoint: 99 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ]      [.,0] [..,0] [tg4,1] -- -- -- -- -- 
[   5 ]      type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ]      type:reg size:6 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   9 ]      chunk(imap): 5 8 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  10 ]      [.,0] [..,0] [tg4,1] [lt0,2] -- -- -- -- 
[  11 ]      type:dir size:1 refs:2 ptrs: 10 -- -- -- -- -- -- -- 
[  12 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  13 ]      chunk(imap): 11 8 12 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  14 ]      n0n0n0n0n0n0n0n0n0n0n0n0n0n0n0n0
[  15 ]      y1y1y1y1y1y1y1y1y1y1y1y1y1y1y1y1
[  16 ]      p2p2p2p2p2p2p2p2p2p2p2p2p2p2p2p2
[  17 ]      l3l3l3l3l3l3l3l3l3l3l3l3l3l3l3l3
[  18 ]      h4h4h4h4h4h4h4h4h4h4h4h4h4h4h4h4
[  19 ]      o5o5o5o5o5o5o5o5o5o5o5o5o5o5o5o5
[  20 ]      y6y6y6y6y6y6y6y6y6y6y6y6y6y6y6y6
[  21 ]      type:reg size:8 refs:1 ptrs: -- 14 15 16 17 18 19 20 
[  22 ]      chunk(imap): 11 8 21 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  23 ]      [.,0] [..,0] [tg4,1] [lt0,2] [oy3,1] -- -- -- 
[  24 ]      type:dir size:1 refs:2 ptrs: 23 -- -- -- -- -- -- -- 
[  25 ]      type:reg size:6 refs:2 ptrs: -- -- -- -- -- -- -- -- 
[  26 ]      chunk(imap): 24 25 21 -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  27 ]      [.,0] [..,0] [tg4,1] [lt0,2] [oy3,1] [af4,3] -- -- 
[  28 ]      type:dir size:1 refs:2 ptrs: 27 -- -- -- -- -- -- -- 
[  29 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  30 ]      chunk(imap): 28 25 21 29 -- -- -- -- -- -- -- -- -- -- -- -- 
[  31 ]      a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0
[  32 ]      type:reg size:6 refs:2 ptrs: -- 31 -- -- -- -- -- -- 
[  33 ]      chunk(imap): 28 32 21 29 -- -- -- -- -- -- -- -- -- -- -- -- 
[  34 ] live u0u0u0u0u0u0u0u0u0u0u0u0u0u0u0u0
[  35 ]      v1v1v1v1v1v1v1v1v1v1v1v1v1v1v1v1
[  36 ]      x2x2x2x2x2x2x2x2x2x2x2x2x2x2x2x2
[  37 ]      t3t3t3t3t3t3t3t3t3t3t3t3t3t3t3t3
[  38 ]      v4v4v4v4v4v4v4v4v4v4v4v4v4v4v4v4
[  39 ]      n5n5n5n5n5n5n5n5n5n5n5n5n5n5n5n5
[  40 ]      type:reg size:8 refs:1 ptrs: 34 35 36 37 38 39 19 20 
[  41 ]      chunk(imap): 28 32 40 29 -- -- -- -- -- -- -- -- -- -- -- -- 
[  42 ]      o0o0o0o0o0o0o0o0o0o0o0o0o0o0o0o0
[  43 ]      l1l1l1l1l1l1l1l1l1l1l1l1l1l1l1l1
[  44 ]      b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2
[  45 ]      w3w3w3w3w3w3w3w3w3w3w3w3w3w3w3w3
[  46 ]      o4o4o4o4o4o4o4o4o4o4o4o4o4o4o4o4
[  47 ]      f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5
[  48 ]      n6n6n6n6n6n6n6n6n6n6n6n6n6n6n6n6
[  49 ]      type:reg size:8 refs:2 ptrs: -- 42 43 44 45 46 47 48 
[  50 ]      chunk(imap): 28 49 40 29 -- -- -- -- -- -- -- -- -- -- -- -- 
[  51 ]      [.,0] [..,0] -- [lt0,2] [oy3,1] [af4,3] -- -- 
[  52 ]      type:dir size:1 refs:2 ptrs: 51 -- -- -- -- -- -- -- 
[  53 ]      type:reg size:8 refs:1 ptrs: -- 42 43 44 45 46 47 48 
[  54 ]      chunk(imap): 52 53 40 29 -- -- -- -- -- -- -- -- -- -- -- -- 
[  55 ]      m0m0m0m0m0m0m0m0m0m0m0m0m0m0m0m0
[  56 ]      j1j1j1j1j1j1j1j1j1j1j1j1j1j1j1j1
[  57 ]      i2i2i2i2i2i2i2i2i2i2i2i2i2i2i2i2
[  58 ]      type:reg size:8 refs:1 ptrs: -- -- -- -- -- 55 56 57 
[  59 ]      chunk(imap): 52 53 40 58 -- -- -- -- -- -- -- -- -- -- -- -- 
[  60 ] live a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0
[  61 ]      f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1
[  62 ]      type:reg size:8 refs:1 ptrs: -- -- -- -- -- 60 61 57 
[  63 ]      chunk(imap): 52 53 40 62 -- -- -- -- -- -- -- -- -- -- -- -- 
[  64 ] live e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0
[  65 ] live p1p1p1p1p1p1p1p1p1p1p1p1p1p1p1p1
[  66 ] live type:reg size:8 refs:1 ptrs: -- -- -- -- -- 60 64 65 
[  67 ]      chunk(imap): 52 53 40 66 -- -- -- -- -- -- -- -- -- -- -- -- 
[  68 ] live u0u0u0u0u0u0u0u0u0u0u0u0u0u0u0u0
[  69 ] live v1v1v1v1v1v1v1v1v1v1v1v1v1v1v1v1
[  70 ] live g2g2g2g2g2g2g2g2g2g2g2g2g2g2g2g2
[  71 ]      v3v3v3v3v3v3v3v3v3v3v3v3v3v3v3v3
[  72 ]      r4r4r4r4r4r4r4r4r4r4r4r4r4r4r4r4
[  73 ]      c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5
[  74 ]      type:reg size:8 refs:1 ptrs: 34 68 69 70 71 72 73 20 
[  75 ]      chunk(imap): 52 53 74 66 -- -- -- -- -- -- -- -- -- -- -- -- 
[  76 ] live a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0
[  77 ] live a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1
[  78 ] live t2t2t2t2t2t2t2t2t2t2t2t2t2t2t2t2
[  79 ] live g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3
[  80 ] live type:reg size:8 refs:1 ptrs: 34 68 69 70 76 77 78 79 
[  81 ]      chunk(imap): 52 53 80 66 -- -- -- -- -- -- -- -- -- -- -- -- 
[  82 ]      [.,0] [..,0] [ln7,4] [lt0,2] [oy3,1] [af4,3] -- -- 
[  83 ]      [.,4] [..,0] -- -- -- -- -- -- 
[  84 ]      type:dir size:1 refs:3 ptrs: 82 -- -- -- -- -- -- -- 
[  85 ]      type:dir size:1 refs:2 ptrs: 83 -- -- -- -- -- -- -- 
[  86 ]      chunk(imap): 84 53 80 66 85 -- -- -- -- -- -- -- -- -- -- -- 
[  87 ]      type:reg size:8 refs:1 ptrs: -- 42 43 44 45 46 47 48 
[  88 ]      chunk(imap): 84 87 80 66 85 -- -- -- -- -- -- -- -- -- -- -- 
[  89 ]      [.,4] [..,0] [zp3,5] -- -- -- -- -- 
[  90 ]      type:dir size:1 refs:2 ptrs: 89 -- -- -- -- -- -- -- 
[  91 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  92 ]      chunk(imap): 84 87 80 66 90 91 -- -- -- -- -- -- -- -- -- -- 
[  93 ] live [.,4] [..,0] [zp3,5] [zu5,6] -- -- -- -- 
[  94 ] live type:dir size:1 refs:2 ptrs: 93 -- -- -- -- -- -- -- 
[  95 ] live type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[  96 ]      chunk(imap): 84 87 80 66 94 91 95 -- -- -- -- -- -- -- -- -- 
[  97 ] live [.,0] [..,0] [ln7,4] [lt0,2] -- [af4,3] -- -- 
[  98 ] live type:dir size:1 refs:3 ptrs: 97 -- -- -- -- -- -- -- 
[  99 ] live chunk(imap): 98 -- 80 66 94 91 95 -- -- -- -- -- -- -- -- -- 

Live directories: /, /ln7
Live files: /lt0, /af4, /ln7/zp3, ln7/zu5
```

## 5

```
python3 ./lfs.py -L c,/foo:w,/foo,0,1:w,/foo,1,1:w,/foo,2,1:w,/foo,3,1 -o

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /foo 
write file  /foo offset=0 size=1 
write file  /foo offset=1 size=1 
write file  /foo offset=2 size=1 
write file  /foo offset=3 size=1 

FINAL file system contents:
[   0 ] live checkpoint: 19 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ] live [.,0] [..,0] [foo,1] -- -- -- -- -- 
[   5 ] live type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ] live v0v0v0v0v0v0v0v0v0v0v0v0v0v0v0v0
[   9 ]      type:reg size:1 refs:1 ptrs: 8 -- -- -- -- -- -- -- 
[  10 ]      chunk(imap): 5 9 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  11 ] live t0t0t0t0t0t0t0t0t0t0t0t0t0t0t0t0
[  12 ]      type:reg size:2 refs:1 ptrs: 8 11 -- -- -- -- -- -- 
[  13 ]      chunk(imap): 5 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  14 ] live k0k0k0k0k0k0k0k0k0k0k0k0k0k0k0k0
[  15 ]      type:reg size:3 refs:1 ptrs: 8 11 14 -- -- -- -- -- 
[  16 ]      chunk(imap): 5 15 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[  17 ] live g0g0g0g0g0g0g0g0g0g0g0g0g0g0g0g0
[  18 ] live type:reg size:4 refs:1 ptrs: 8 11 14 17 -- -- -- -- 
[  19 ] live chunk(imap): 5 18 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

## 6

```
python3 ./lfs.py -o -L c,/foo:w,/foo,0,4                                    

INITIAL file system contents:
[   0 ] live checkpoint: 3 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ] live [.,0] [..,0] -- -- -- -- -- -- 
[   2 ] live type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ] live chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

create file /foo 
write file  /foo offset=0 size=4 

FINAL file system contents:
[   0 ] live checkpoint: 13 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   1 ]      [.,0] [..,0] -- -- -- -- -- -- 
[   2 ]      type:dir size:1 refs:2 ptrs: 1 -- -- -- -- -- -- -- 
[   3 ]      chunk(imap): 2 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   4 ] live [.,0] [..,0] [foo,1] -- -- -- -- -- 
[   5 ] live type:dir size:1 refs:2 ptrs: 4 -- -- -- -- -- -- -- 
[   6 ]      type:reg size:0 refs:1 ptrs: -- -- -- -- -- -- -- -- 
[   7 ]      chunk(imap): 5 6 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
[   8 ] live v0v0v0v0v0v0v0v0v0v0v0v0v0v0v0v0
[   9 ] live t1t1t1t1t1t1t1t1t1t1t1t1t1t1t1t1
[  10 ] live k2k2k2k2k2k2k2k2k2k2k2k2k2k2k2k2
[  11 ] live g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3g3
[  12 ] live type:reg size:4 refs:1 ptrs: 8 9 10 11 -- -- -- -- 
[  13 ] live chunk(imap): 5 12 -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

> What is the main difference between writing a file all at once (as we do here) versus doing it one block at a time (as above)?

When writing all at once, less garbage is left.

> What does this tell you about the importance of buffering updates in main memory as the real LFS does?

Buffering updates is important for performance.

## 7

> First,run the following:./lfs.py -L c,/foo:w,/foo,0,1. What does this set of commands do?

Write 1 data block and assign it to pointer 0 of /foo inode.

> Now, run ./lfs.py -L c,/foo:w,/foo,7,1. What does this set of commands do?

Write 1 data block and assign it to pointer 7 of /foo inode.

> How are the two different?

They change different pointers in the inode.

> What can you tell about the size field in the inode from these two sets of commands?

Size specifies how many blocks are going to be written to the file, how many pointers are going to be assigned.

## 8

> Run simulations ./lfs.py -L c,/foo and ./lfs.py -L d,/foo to create a file and then a directory. What is similar about these runs, and what is different?

Similar:
* New entry is added to the root directory
* New inode is created

Different:
* New inode type
* A data block is allocated for directory, but not for file

## 9

> The LFS simulator supports hard links as well. Run the following to study how they work:
./lfs.py -L c,/foo:l,/foo,/bar:l,/foo,/goo -o -i. What blocks are written out when a hard link is created?

Blocks written out:
1. Checkpoint
2. Directory data block
3. Directory inode
4. Linked file inode
5. imap

> How is this similar to just creating a new file, and how is it different?

Similar:
* Directory data and inode are updated
* File inode is written

Different:
* When a new file is created, a new inode is allocated in imap. For links no new inode is allocated, only refcount is updated in the source file inode.

> How does the reference count field change as links are created?

Refcount goes up by one with every new link.

## 10

> What on-disk differences does a random policy versus a sequential policy result in?

For sequential policy a single imap is sufficient. For random policy multiple imaps are created.

> What does this say about the importance of choosing inode numbers in a real LFS?

It's better to use sequential inode numbers for more effective space usage.

## 11

> What would happen if the checkpoint region is never updated?

OS won't see changes to the file system that the commands made.

> What if it is updated periodically?

If the computer crashes before the CR is updated, some changes to the FS may be lost.

> Could you figure out how to recover the file system to the latest state by rolling forward in the log?

Start at the first imap that CR is pointing at, move forward to the next imap and update the CR accordingly.
