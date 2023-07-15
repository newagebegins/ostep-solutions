# Chapter 42 (Crash Consistency: FSCK and Journaling)

## 1

```
python3 ./fsck.py -D -s 0

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out which files and directories exist?

/ - dir
/g - dir
/w - dir
/m - empty file
/z - empty file
/g/s - empty file
```

```
python3 ./fsck.py -D -s 1

Final state of file system:

inode bitmap 1000100110010001
inodes       [d a:0 r:4] [] [] [] [f a:-1 r:1] [] [] [d a:10 r:2] [d a:15 r:2] [] [] [f a:-1 r:3] [] [] [] [f a:-1 r:1] 
data bitmap  1000000000100001
data         [(.,0) (..,0) (m,7) (a,8) (g,11)] [] [] [] [] [] [] [] [] [] [(.,7) (..,0) (m,15) (e,11)] [] [] [] [] [(.,8) (..,0) (r,4) (w,11)] 

Can you figure out which files and directories exist?

/ - dir
/m - dir
/a - dir
/g - file
/m/m - file
/m/e - file
/a/r - file
/a/w - file
```

```
python3 ./fsck.py -D -s 2   

Final state of file system:

inode bitmap 1000000100110101
inodes       [d a:0 r:3] [] [] [] [] [] [] [d a:4 r:3] [] [] [f a:-1 r:1] [f a:-1 r:2] [] [d a:11 r:3] [] [d a:15 r:2] 
data bitmap  1000100000010001
data         [(.,0) (..,0) (c,13)] [] [] [] [(.,7) (..,13) (u,15) (q,11)] [] [] [] [] [] [] [(.,13) (..,0) (o,7)] [] [] [] [(.,15) (..,7) (q,11) (e,10)] 

Can you figure out which files and directories exist?

/ - dir
/c - dir
/c/o - dir
/c/o/u - dir
/c/o/q - file
/c/o/u/q - file
/c/o/u/e - file
```

## 2

```
python3 ./fsck.py -S 1      
ARG seed 0
ARG seedCorrupt 1
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000001
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

inode bitmap lacks 1 at index 13.

## 3

```
python3 ./fsck.py -S 3   
ARG seed 0
ARG seedCorrupt 3
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:2] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 15 says that there are 2 links, but in data blocks there is only one link to inode 15: `/g/s`.

```
python3 ./fsck.py -S 19  
ARG seed 0
ARG seedCorrupt 19
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:1] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Refcount of inode 8 should be 2.

> What is different in these two cases?

In the first case refcount is increased, in the second case - decreased.

## 4

```
python3 ./fsck.py -S 5 -c
ARG seed 0
ARG seedCorrupt 5
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Initial state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

CORRUPTION::INODE 8 with directory [('.', 8), ('..', 0), ('s', 15)]:
  entry ('s', 15) altered to refer to different name (y)

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (y,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 
```

> How hard would it be to fix this problem in an automatic way?

Seems impossible to fix in an automatic way, because we don't know original file name.

```
python3 ./fsck.py -S 38 -c
ARG seed 0
ARG seedCorrupt 38
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Initial state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

CORRUPTION::INODE 4 with directory [('.', 4), ('..', 0)]:
  entry ('..', 0) altered to refer to different name (b)

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (b,0)] [] [] [] 
```

> Then, introduce a similar inconsistency with -S 38; is this harder/possible to detect?

It's easier to detect, because each directory must have a `..` entry.

```
python3 ./fsck.py -S 642  
ARG seed 0
ARG seedCorrupt 642
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (w,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Two entries with the same name: `/w`.

> is this inconsistency detectable?

Yes.

> If so, how would you fix the file system?

Rename one of the entries.

## 5

```
python3 ./fsck.py -S 6     
ARG seed 0
ARG seedCorrupt 6
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [d a:-1 r:1] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 12 should be empty, but it's not.

```
python3 ./fsck.py -S 13  
ARG seed 0
ARG seedCorrupt 13
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [f a:-1 r:1] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 10 - orphan.

> What is the difference across these two cases?

1st case - directory, 2nd case - file.

> What should the repair tool do when encountering such a situation?

Nothing? Orphaned inode can be reused.

## 6

```
python3 ./fsck.py -S 9
ARG seed 0
ARG seedCorrupt 9
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [d a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 13 should be file, not directory.

> Which piece of information should a check-and-repair tool trust in this case?

Address -1, multiple links (`/m`, `/z`).

## 7

```
python3 ./fsck.py -S 15  
ARG seed 0
ARG seedCorrupt 15
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [f a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 0 should be directory, not file.

> What can a repair tool do in this case?

Change type of inode.

> If no repair is possible, how much data is lost?

All links in the directory are lost, but the orphaned files can be placed in `lost+found` directory, so the data can be recovered.

## 8

```
python3 ./fsck.py -S 10
ARG seed 0
ARG seedCorrupt 10
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,3)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Entry in directory data block 12 links to non-existant inode: `(..,3)`.

> Is there redundancy in the file system structure here that can help a repair?

Yes, parent and child directories link to each other.

## 9

```
python3 ./fsck.py -S 16   
ARG seed 0
ARG seedCorrupt 16
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:6 r:2] [] [] [] [] [f a:7 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Address in inode 13 points to unallocated block.

```
python3 ./fsck.py -S 20   
ARG seed 0
ARG seedCorrupt 20
ARG numInodes 16
ARG numData 16
ARG numRequests 15
ARG printFinal False
ARG whichCorrupt -1
ARG dontCorrupt False

Final state of file system:

inode bitmap 1000100010000101
inodes       [d a:0 r:4] [] [] [] [d a:12 r:2] [] [] [] [d a:11 r:2] [] [] [] [] [f a:-1 r:2] [] [f a:-1 r:1] 
data bitmap  1000001000001000
data         [(.,0) (..,0) (g,8) (w,4) (m,13) (z,13)] [] [] [] [] [] [(.,8) (..,0) (s,15)] [] [] [] [] [] [(.,4) (..,0)] [] [] [] 

Can you figure out how the file system was corrupted?
```

Inode 8 points to unallocated block 11.

> How should the repair tool fix the problem?

I see 2 solutions:
1. Allocate a block to get an empty file or directory.
2. Delete the inode that refers to unallocated block (also delete links).
