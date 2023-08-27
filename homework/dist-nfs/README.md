# Chapter 49: Sun’s Network File System (NFS)

Note: The code was tested on Debian 11.

## Question 1

> A first question for your trace analysis: using the timestamps found in the first column, determine the period of time the traces were taken from. How long is the period? What day/week/month/year was it? (does this match the hint given in the file name?) Hint: Use the tools `head -1` and `tail -1` to extract the first and last lines of the file, and do the calculation.

Period start:

```sh
head -n 1 anon-deasna-021016-1300.txt | awk '{print strftime("%c", $1, 1)}'
```

```
Wed 16 Oct 2002 05:00:00 PM GMT
```

Period end:

```sh
tail -n 1 anon-deasna-021016-1300.txt | awk '{print strftime("%c", $1, 1)}'
```

```
Wed 16 Oct 2002 05:59:59 PM GMT
```

The period is 1 hour.

## Question 2

> Now,let’s do some operation counts. How many of each type of operation occur in the trace? Sort these by frequency; which operation is most frequent? Does NFS live up to its reputation?

```sh
awk '{print $8}' anon-deasna-021016-1300.txt | sort | uniq -c | sort -nr
```

```
1610395 getattr
1043752 read
 619819 write
 131453 lookup
  27699 access
  19485 setattr
  11640 remove
   9924 create
   9135 fsstat
   4290 link
   2297 readdirp
   1321 fsinfo
    918 readdir
    501 rename
    439 readlink
    187 pathconf
    136 symlink
     36 mkdir
     14 rmdir
      4 mknod
```

## Question 3

> Now let’s look at some particular operations in more detail. For example, the GETATTR request returns a lot of information about files, including which user ID the request is being performed for, the size of the file, and so forth. Make a distribution of file sizes accessed within the trace; what is the average file size? Also, how many different users access files in the trace? Do a few users dominate traffic, or is it more spread out? What other interesting information is found within GETATTR replies?

Top 10 most frequent file sizes:

```sh
cat anon-deasna-021016-1300.txt | awk '$8 == "getattr" && $5 == "R3" {printf "%d\n", strtonum("0x"$21)}' | sort | uniq -c | sort -nr | head -n 10
```

```
 145159 3697
 145130 3710
  91427 4096
  73369 1560
  35809 114453394
  35648 114438321
  21492 46048008
  17842 60010712
  17831 59982576
  14226 37360083
```

Average file size:

```sh
cat anon-deasna-021016-1300.txt | awk '$8 == "getattr" && $5 == "R3" {printf "%d\n", strtonum("0x"$21)}' | sort | uniq -c | awk '{n += $1; s += $1*$2} END {printf "%d\n", s/n}'
```

```
17728821
```

Number of different users making requests:

```sh
cat anon-deasna-021016-1300.txt | awk '$5 == "C3" {print $2}' | sort -u | wc -l
```

```
219
```

Top 10 users with most requests:

```sh
cat anon-deasna-021016-1300.txt | awk '$5 == "C3" {print $2}' | sort | uniq -c | sort -nr | head -n 10
```

```
 568181 32.03ff
 436586 31.0320
 239866 40.03fe
 112251 32.03fe
  81760 33.03fe
  34013 37.03ff
  21433 45.0320
  16059 32.03fa
  15144 40.03fd
  13288 37.03fd
```

## Question 4

> You can also look at requests to a given file and determine how files are being accessed. For example, is a given file being read or written sequentially? Or randomly? Look at the details of READ and WRITE requests/replies to compute the answer.

```sh
cat anon-deasna-021016-1300.txt | awk '$5 == "C3" && ($8 == "read" || $8 == "write") {if ($12 == files[$10][$2][$8]) seq++; else rnd++; files[$10][$2][$8] = $12 + $14} END {print "Sequential:", seq, "\nRandom:", rnd}'
```

```
Sequential: 127015
Random: 711980
```

There are more random accesses than sequential.

## Question 5

> Traffic comes from many machines and goes to one server (in this trace). Compute a traffic matrix, which shows how many different clients there are in the trace, and how many requests/replies go to each. Do a few machines dominate, or is it more evenly balanced?

Already answered in Question 3.

## Question 6

> The timing information, and the per-request/reply unique ID, should allow you to compute the latency for a given request. Compute the latencies of all request/reply pairs, and plot them as a distribution. What is the average? Maximum? Minimum?

Ten most frequent latencies:

```sh
cat anon-deasna-021016-1300.txt | awk '{if (time[$6]) {print $1 - time[$6]; delete time[$6]} else time[$6] = $1}' | sort -n | uniq -c | sort -nr | head -n 10
```

```
  46994 0.000108004
  36534 0.000105977
  30564 0.000108957
  28233 0.00010705
  27735 0.000105023
  26655 0.00011003
  25007 0.000110984
  20639 0.000106931
  17388 0.00011301
  16870 0.000109076
```

Average latency:

```sh
cat anon-deasna-021016-1300.txt | awk '{if (time[$6]) {print $1 - time[$6]; delete time[$6]} else time[$6] = $1}' | sort -n | uniq -c | awk '{n += $1; s += $1*$2} END {print s/n}'
```

```
0.00202809
```

Maximum latency:

```sh
cat anon-deasna-021016-1300.txt | awk '{if (time[$6]) {print $1 - time[$6]; delete time[$6]} else time[$6] = $1}' | sort -n | tail -n 1
```

```
10.1498
```

Minimum latency:

```sh
cat anon-deasna-021016-1300.txt | awk '{if (time[$6]) {print $1 - time[$6]; delete time[$6]} else time[$6] = $1}' | sort -n | head -n 1
```

```
0.000100017
```

## Question 7

> Sometimes requests are retried, as the request or its reply could be lost or dropped. Can you find any evidence of such retrying in the trace sample?

As I understood, retried requests have the same ID and are close in time.

Show first 4 retried requests (I chose time between requests to be less than 10 sec):

```sh
cat anon-deasna-021016-1300.txt | awk '$5 == "C3" {if (t[$6] && ($1 - t[$6] < 10)) print $6; t[$6] = $1}' | head -n 4
```

```
dca9f489
fa951970
56b31970
52880dd8
```

And if we grep the first ID, we see that in fact there was a retried request:

```sh
grep dca9f489 anon-deasna-021016-1300.txt 
```

```
1034787618.376464 45.0320 30.0801 U C3 dca9f489 7 write fh eb130a00e6189703200000000009532169578b0464860000a0cf90002e303000 off 6000 count 1000 stable U con = XXX len = XXX
1034787619.051081 45.0320 30.0801 U C3 dca9f489 7 write fh eb130a00e6189703200000000009532169578b0464860000a0cf90002e303000 off 6000 count 1000 stable U con = XXX len = XXX
1034787619.051732 30.0801 45.0320 U R3 dca9f489 7 write OK pre-size aee6 pre-mtime 1034787618.455000 pre-ctime 1034787618.455000 ftype 1 mode 180 nlink 1 uid 18a9d gid 18a8d size aee6 used 8000 rdev 0 rdev2 0 fsid 8664 fileid 95321 atime 1034787618.318005 mtime 1034787619.105000 ctime 1034787619.105000 count 1000 stable F status=XXX pl = XXX con = XXX len = XXX
```
