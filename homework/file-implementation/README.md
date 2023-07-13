# Chapter 40 (File System Implementation)

## 1

### -s 17

```
python3 ./vsfs.py -s 17
ARG seed 17
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse False
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

mkdir("/u");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1)][(.,1) (..,0)][][][][][][]

creat("/a");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1) (a,2)][(.,1) (..,0)][][][][][][]

unlink("/a");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1)][(.,1) (..,0)][][][][][][]

mkdir("/z");

inode bitmap  11100000
inodes       [d a:0 r:4][d a:1 r:2][d a:2 r:2][][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (u,1) (z,2)][(.,1) (..,0)][(.,2) (..,0)][][][][][]

mkdir("/s");

inode bitmap  11110000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0)][(.,3) (..,0)][][][][]

creat("/z/x");

inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]

link("/z/x", "/u/b");

inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:2][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0) (b,4)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]

unlink("/u/b");

inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]

fd=open("/z/x", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:4 r:1][][][]
data bitmap   11111000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][v][][][]

creat("/u/b");

inode bitmap  11111100
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:4 r:1][f a:-1 r:1][][]
data bitmap   11111000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0) (b,5)][(.,2) (..,0) (x,4)][(.,3) (..,0)][v][][][]
```

### -s 18

```
python3 ./vsfs.py -s 18
ARG seed 18
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse False
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

mkdir("/f");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (f,1)][(.,1) (..,0)][][][][][][]

creat("/s");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (f,1) (s,2)][(.,1) (..,0)][][][][][][]

mkdir("/h");

inode bitmap  11110000
inodes       [d a:0 r:4][d a:1 r:2][f a:-1 r:1][d a:2 r:2][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (f,1) (s,2) (h,3)][(.,1) (..,0)][(.,3) (..,0)][][][][][]

fd=open("/s", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11110000
inodes       [d a:0 r:4][d a:1 r:2][f a:3 r:1][d a:2 r:2][][][][]
data bitmap   11110000
data         [(.,0) (..,0) (f,1) (s,2) (h,3)][(.,1) (..,0)][(.,3) (..,0)][f][][][][]

creat("/f/o");

inode bitmap  11111000
inodes       [d a:0 r:4][d a:1 r:2][f a:3 r:1][d a:2 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (f,1) (s,2) (h,3)][(.,1) (..,0) (o,4)][(.,3) (..,0)][f][][][][]

creat("/c");

inode bitmap  11111100
inodes       [d a:0 r:4][d a:1 r:2][f a:3 r:1][d a:2 r:2][f a:-1 r:1][f a:-1 r:1][][]
data bitmap   11110000
data         [(.,0) (..,0) (f,1) (s,2) (h,3) (c,5)][(.,1) (..,0) (o,4)][(.,3) (..,0)][f][][][][]

unlink("/c");

inode bitmap  11111000
inodes       [d a:0 r:4][d a:1 r:2][f a:3 r:1][d a:2 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (f,1) (s,2) (h,3)][(.,1) (..,0) (o,4)][(.,3) (..,0)][f][][][][]

fd=open("/f/o", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11111000
inodes       [d a:0 r:4][d a:1 r:2][f a:3 r:1][d a:2 r:2][f a:4 r:1][][][]
data bitmap   11111000
data         [(.,0) (..,0) (f,1) (s,2) (h,3)][(.,1) (..,0) (o,4)][(.,3) (..,0)][f][q][][][]

unlink("/s");

inode bitmap  11011000
inodes       [d a:0 r:4][d a:1 r:2][][d a:2 r:2][f a:4 r:1][][][]
data bitmap   11101000
data         [(.,0) (..,0) (f,1) (h,3)][(.,1) (..,0) (o,4)][(.,3) (..,0)][][q][][][]

unlink("/f/o");

inode bitmap  11010000
inodes       [d a:0 r:4][d a:1 r:2][][d a:2 r:2][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (f,1) (h,3)][(.,1) (..,0)][(.,3) (..,0)][][][][][]
```

### -s 19

```
python3 ./vsfs.py -s 19   
ARG seed 19
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse False
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

creat("/k");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:1][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (k,1)][][][][][][][]

creat("/g");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:-1 r:1][f a:-1 r:1][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (k,1) (g,2)][][][][][][][]

fd=open("/k", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11100000
inodes       [d a:0 r:2][f a:1 r:1][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (k,1) (g,2)][g][][][][][][]

link("/k", "/b");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (k,1) (g,2) (b,1)][g][][][][][][]

link("/k", "/t");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:1 r:3][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (k,1) (g,2) (b,1) (t,1)][g][][][][][][]

unlink("/k");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (g,2) (b,1) (t,1)][g][][][][][][]

mkdir("/r");

inode bitmap  11110000
inodes       [d a:0 r:3][f a:1 r:2][f a:-1 r:1][d a:2 r:2][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (g,2) (b,1) (t,1) (r,3)][g][(.,3) (..,0)][][][][][]

mkdir("/p");

inode bitmap  11111000
inodes       [d a:0 r:4][f a:1 r:2][f a:-1 r:1][d a:2 r:2][d a:3 r:2][][][]
data bitmap   11110000
data         [(.,0) (..,0) (g,2) (b,1) (t,1) (r,3) (p,4)][g][(.,3) (..,0)][(.,4) (..,0)][][][][]

mkdir("/r/d");

inode bitmap  11111100
inodes       [d a:0 r:4][f a:1 r:2][f a:-1 r:1][d a:2 r:3][d a:3 r:2][d a:4 r:2][][]
data bitmap   11111000
data         [(.,0) (..,0) (g,2) (b,1) (t,1) (r,3) (p,4)][g][(.,3) (..,0) (d,5)][(.,4) (..,0)][(.,5) (..,3)][][][]

link("/g", "/s");

inode bitmap  11111100
inodes       [d a:0 r:4][f a:1 r:2][f a:-1 r:2][d a:2 r:3][d a:3 r:2][d a:4 r:2][][]
data bitmap   11111000
data         [(.,0) (..,0) (g,2) (b,1) (t,1) (r,3) (p,4) (s,2)][g][(.,3) (..,0) (d,5)][(.,4) (..,0)][(.,5) (..,3)][][][]
```

### -s 20

```
python3 ./vsfs.py -s 20   
ARG seed 20
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse False
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

creat("/x");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:1][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (x,1)][][][][][][][]

fd=open("/x", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11000000
inodes       [d a:0 r:2][f a:1 r:1][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (x,1)][x][][][][][][]

creat("/k");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:1 r:1][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (x,1) (k,2)][x][][][][][][]

creat("/y");

inode bitmap  11110000
inodes       [d a:0 r:2][f a:1 r:1][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (x,1) (k,2) (y,3)][x][][][][][][]

unlink("/x");

inode bitmap  10110000
inodes       [d a:0 r:2][][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (k,2) (y,3)][][][][][][][]

unlink("/y");

inode bitmap  10100000
inodes       [d a:0 r:2][][f a:-1 r:1][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (k,2)][][][][][][][]

unlink("/k");

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

creat("/p");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:1][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (p,1)][][][][][][][]

fd=open("/p", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11000000
inodes       [d a:0 r:2][f a:1 r:1][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (p,1)][e][][][][][][]

link("/p", "/s");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (p,1) (s,1)][e][][][][][][]
```

## 2

### -r -s 21

```
python3 ./vsfs.py -r -s 21
ARG seed 21
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse True
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

mkdir("/o");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (o,1)][(.,1) (..,0)][][][][][][]

creat("/b");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (o,1) (b,2)][(.,1) (..,0)][][][][][][]

creat("/o/q");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (o,1) (b,2)][(.,1) (..,0) (q,3)][][][][][][]

fd=open("/b", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:2 r:1][f a:-1 r:1][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (o,1) (b,2)][(.,1) (..,0) (q,3)][v][][][][][]

fd=open("/o/q", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:2 r:1][f a:3 r:1][][][][]
data bitmap   11110000
data         [(.,0) (..,0) (o,1) (b,2)][(.,1) (..,0) (q,3)][v][w][][][][]

creat("/o/j");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:2 r:1][f a:3 r:1][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (o,1) (b,2)][(.,1) (..,0) (q,3) (j,4)][v][w][][][][]

unlink("/b");

inode bitmap  11011000
inodes       [d a:0 r:3][d a:1 r:2][][f a:3 r:1][f a:-1 r:1][][][]
data bitmap   11010000
data         [(.,0) (..,0) (o,1)][(.,1) (..,0) (q,3) (j,4)][][w][][][][]

fd=open("/o/j", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11011000
inodes       [d a:0 r:3][d a:1 r:2][][f a:3 r:1][f a:2 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (o,1)][(.,1) (..,0) (q,3) (j,4)][v][w][][][][]

creat("/o/x");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:3 r:1][f a:2 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (o,1)][(.,1) (..,0) (q,3) (j,4) (x,2)][v][w][][][][]

mkdir("/o/t");

inode bitmap  11111100
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:3 r:1][f a:2 r:1][d a:4 r:2][][]
data bitmap   11111000
data         [(.,0) (..,0) (o,1)][(.,1) (..,0) (q,3) (j,4) (x,2) (t,5)][v][w][(.,5) (..,1)][][][]
```

### -r -s 22

```
python3 ./vsfs.py -r -s 22   
ARG seed 22
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse True
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

creat("/z");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:1][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (z,1)][][][][][][][]

fd=open("/z", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11000000
inodes       [d a:0 r:2][f a:1 r:1][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (z,1)][m][][][][][][]

unlink("/z");

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

creat("/y");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:1][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (y,1)][][][][][][][]

link("/y", "/s");

inode bitmap  11000000
inodes       [d a:0 r:2][f a:-1 r:2][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (y,1) (s,1)][][][][][][][]

creat("/e");

inode bitmap  11100000
inodes       [d a:0 r:2][f a:-1 r:2][f a:-1 r:1][][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (y,1) (s,1) (e,2)][][][][][][][]

creat("/u");

inode bitmap  11110000
inodes       [d a:0 r:2][f a:-1 r:2][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   10000000
data         [(.,0) (..,0) (y,1) (s,1) (e,2) (u,3)][][][][][][][]

creat("/q");

inode bitmap  11111000
inodes       [d a:0 r:2][f a:-1 r:2][f a:-1 r:1][f a:-1 r:1][f a:-1 r:1][][][]
data bitmap   10000000
data         [(.,0) (..,0) (y,1) (s,1) (e,2) (u,3) (q,4)][][][][][][][]

fd=open("/q", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11111000
inodes       [d a:0 r:2][f a:-1 r:2][f a:-1 r:1][f a:-1 r:1][f a:1 r:1][][][]
data bitmap   11000000
data         [(.,0) (..,0) (y,1) (s,1) (e,2) (u,3) (q,4)][m][][][][][][]

mkdir("/d");

inode bitmap  11111100
inodes       [d a:0 r:3][f a:-1 r:2][f a:-1 r:1][f a:-1 r:1][f a:1 r:1][d a:2 r:2][][]
data bitmap   11100000
data         [(.,0) (..,0) (y,1) (s,1) (e,2) (u,3) (q,4) (d,5)][m][(.,5) (..,0)][][][][][]
```

### -r -s 23

```
python3 ./vsfs.py -r -s 23   
ARG seed 23
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse True
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

mkdir("/c");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0)][][][][][][]

creat("/c/t");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0) (t,2)][][][][][][]

unlink("/c/t");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0)][][][][][][]

creat("/c/q");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0) (q,2)][][][][][][]

creat("/c/j");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0) (q,2) (j,3)][][][][][][]

link("/c/q", "/c/h");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:2][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0) (q,2) (j,3) (h,2)][][][][][][]

unlink("/c/h");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1)][(.,1) (..,0) (q,2) (j,3)][][][][][][]

link("/c/q", "/r");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:2][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (c,1) (r,2)][(.,1) (..,0) (q,2) (j,3)][][][][][][]

fd=open("/c/q", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:2 r:2][f a:-1 r:1][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (c,1) (r,2)][(.,1) (..,0) (q,2) (j,3)][m][][][][][]

creat("/g");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:2 r:2][f a:-1 r:1][f a:-1 r:1][][][]
data bitmap   11100000
data         [(.,0) (..,0) (c,1) (r,2) (g,4)][(.,1) (..,0) (q,2) (j,3)][m][][][][][]
```

### -r -s 24

```
python3 ./vsfs.py -r -s 24   
ARG seed 24
ARG numInodes 8
ARG numData 8
ARG numRequests 10
ARG reverse True
ARG printFinal False

Initial state

inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]

mkdir("/z");

inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (z,1)][(.,1) (..,0)][][][][][][]

creat("/z/t");

inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (z,1)][(.,1) (..,0) (t,2)][][][][][][]

creat("/z/z");

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:-1 r:1][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (z,1)][(.,1) (..,0) (t,2) (z,3)][][][][][][]

fd=open("/z/z", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11110000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:2 r:1][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (z,1)][(.,1) (..,0) (t,2) (z,3)][v][][][][][]

creat("/y");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:2 r:1][f a:-1 r:1][][][]
data bitmap   11100000
data         [(.,0) (..,0) (z,1) (y,4)][(.,1) (..,0) (t,2) (z,3)][v][][][][][]

fd=open("/y", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][f a:2 r:1][f a:3 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (z,1) (y,4)][(.,1) (..,0) (t,2) (z,3)][v][w][][][][]

fd=open("/z/t", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:4 r:1][f a:2 r:1][f a:3 r:1][][][]
data bitmap   11111000
data         [(.,0) (..,0) (z,1) (y,4)][(.,1) (..,0) (t,2) (z,3)][v][w][x][][][]

link("/y", "/x");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:4 r:1][f a:2 r:1][f a:3 r:2][][][]
data bitmap   11111000
data         [(.,0) (..,0) (z,1) (y,4) (x,4)][(.,1) (..,0) (t,2) (z,3)][v][w][x][][][]

unlink("/x");

inode bitmap  11111000
inodes       [d a:0 r:3][d a:1 r:2][f a:4 r:1][f a:2 r:1][f a:3 r:1][][][]
data bitmap   11111000
data         [(.,0) (..,0) (z,1) (y,4)][(.,1) (..,0) (t,2) (z,3)][v][w][x][][][]

mkdir("/z/w");

inode bitmap  11111100
inodes       [d a:0 r:3][d a:1 r:3][f a:4 r:1][f a:2 r:1][f a:3 r:1][d a:5 r:2][][]
data bitmap   11111100
data         [(.,0) (..,0) (z,1) (y,4)][(.,1) (..,0) (t,2) (z,3) (w,5)][v][w][x][(.,5) (..,1)][][]
```

### Allocation algorithms

Inode and data-block allocation algorithms prefer to allocate the first empty inode or data-block.

## 3

> Now reduce the number of data blocks in the file system, to very low numbers (say two), and run the simulator for a hundred or so requests.

> What types of files end up in the file system in this highly-constrained layout?

Empty files.

> What types of operations would fail?

write(), mkdir().

## 4

> Now do the same, but with inodes.

> With very few inodes, what types of operations can succeed?

link(), unlink(), write().

> Which will usually fail?

mkdir(), creat().

> What is the final state of the file system likely to be?

Very few files and directories.
