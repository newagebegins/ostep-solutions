# Chapter 41 (Locality and The Fast File System)

## 1

```
python3 ./ffs.py -f in.largefile -L 4

group inodes    data
    0 /a-------- /aaaa-------------------------
    1 ---------- aaaa--------------------------
    2 ---------- aaaa--------------------------
    3 ---------- aaaa--------------------------
    4 ---------- aaaa--------------------------
    5 ---------- aaaa--------------------------
    6 ---------- aaaa--------------------------
    7 ---------- aaaa--------------------------
    8 ---------- aaaa--------------------------
    9 ---------- aaaa--------------------------
```

## 2

```
python3 ./ffs.py -f in.largefile -L 30

group inodes    data
    0 /a-------- /aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    1 ---------- aaaaaaaaaaa-------------------
    2 ---------- ------------------------------
    3 ---------- ------------------------------
    4 ---------- ------------------------------
    5 ---------- ------------------------------
    6 ---------- ------------------------------
    7 ---------- ------------------------------
    8 ---------- ------------------------------
    9 ---------- ------------------------------
```

## 3

File span of /a for `-L 4` is 372:

```
python3 ./ffs.py -f in.largefile -L 4 -T -c
```

File span of /a for `-L 100` is 59:

```
python3 ./ffs.py -f in.largefile -L 100 -T -c
```

> What difference do you expect in filespan as the large-file exception parameter changes from low values to high values?

File span decreases.

## 4

```
python3 ./ffs.py -f in.manyfiles

group inodes    data
    0 /abcdefghi /aabbccddeeffgghhii-----------
    1 jlmnopqrC- jlmnopqrCCC-------------------
    2 tuvwxyzAB- tuuuvvvwwwxxxyyyzzzAAABBB-----
    3 ---------- ------------------------------
    4 ---------- ------------------------------
    5 ---------- ------------------------------
    6 ---------- ------------------------------
    7 ---------- ------------------------------
    8 ---------- ------------------------------
    9 ---------- ------------------------------
```

## 5

```
python3 ./ffs.py -f in.manyfiles -c -T

span: directories
  dir:           /  dirspan:  28
  dir:          /j  dirspan:  20
  dir:          /t  dirspan:  34
               avg  dirspan:  27.33
```

## 6

```
python3 ./ffs.py -f in.manyfiles -c -T -i 5

group inodedata
    0 /abcd /aabbccdd- ---------- ----------
    1 efghi eeffgghhii ---------- ----------
    2 jlmno jlmno----- ---------- ----------
    3 tuvwx tuuuvvvwww xxx------- ----------
    4 ypzqA yyypzzzqAA A--------- ----------
    5 rBC-- rBBBCCC--- ---------- ----------
    6 ----- ---------- ---------- ----------
    7 ----- ---------- ---------- ----------
    8 ----- ---------- ---------- ----------
    9 ----- ---------- ---------- ----------

span: directories
  dir:           /  dirspan:  49
  dir:          /j  dirspan: 116
  dir:          /t  dirspan:  78
               avg  dirspan:  81.00
```

Dirspan increases.

## 7

```
python3 ./ffs.py -f in.manyfiles -c -T -i 5 -A 2   

group inodedata
    0 /abcd /aabbccdd- ---------- ----------
    1 efghi eeffgghhii ---------- ----------
    2 jlmno jlmno----- ---------- ----------
    3 pqrC- pqrCCC---- ---------- ----------
    4 tuvwx tuuuvvvwww xxx------- ----------
    5 yzAB- yyyzzzAAAB BB-------- ----------
    6 ----- ---------- ---------- ----------
    7 ----- ---------- ---------- ----------
    8 ----- ---------- ---------- ----------
    9 ----- ---------- ---------- ----------

span: directories
  dir:           /  dirspan:  49
  dir:          /j  dirspan:  45
  dir:          /t  dirspan:  51
               avg  dirspan:  48.33
```

Dirspan becomes less. This policy keeps file inodes and data near the directory inode, thus decreasing seek times.

## 8

```
python3 ./ffs.py -f in.fragmented -v            
op create /i [size:8] ->success

group inodes    data
    0 /ib-d-f-h- /ibidifihiiii-----------------
    1 ---------- ------------------------------
    2 ---------- ------------------------------
    3 ---------- ------------------------------
    4 ---------- ------------------------------
    5 ---------- ------------------------------
    6 ---------- ------------------------------
    7 ---------- ------------------------------
    8 ---------- ------------------------------
    9 ---------- ------------------------------

span: files
  file:         /b  filespan:  10
  file:         /d  filespan:  10
  file:         /f  filespan:  10
  file:         /h  filespan:  10
  file:         /i  filespan:  21
               avg  filespan:  12.20

span: directories
  dir:           /  dirspan:  22
               avg  dirspan:  22.00
```

File `/i` data is not in consecutive blocks (data is fragmented). This increases seek times.

## 9

```
python3 ./ffs.py -f in.fragmented -c -T -C 2

group inodes    data
    0 /ib-d-f-h- /-b-d-f-hi iiiiiii--- ----------
    1 ---------- ---------- ---------- ----------
    2 ---------- ---------- ---------- ----------
    3 ---------- ---------- ---------- ----------
    4 ---------- ---------- ---------- ----------
    5 ---------- ---------- ---------- ----------
    6 ---------- ---------- ---------- ----------
    7 ---------- ---------- ---------- ----------
    8 ---------- ---------- ---------- ----------
    9 ---------- ---------- ---------- ----------

span: files
  file:         /b  filespan:  10
  file:         /d  filespan:  10
  file:         /f  filespan:  10
  file:         /h  filespan:  10
  file:         /i  filespan:  25
               avg  filespan:  13.00

span: directories
  dir:           /  dirspan:  26
               avg  dirspan:  26.00
```

> Finally, how does -C affect filespan and dirspan?

Filespan and dirspan increase with C.
