## 1

```
python3 ./paging-policy.py -s 0 -n 10
Access: 8  miss  8
Access: 7  miss  8,7
Access: 4  miss  8,7,4
Access: 2  miss  7,4,2
Access: 5  miss  4,2,5
Access: 4  hit   4,2,5
Access: 7  miss  2,5,7
Access: 3  miss  5,7,3
Access: 4  miss  7,3,4
Access: 5  miss  3,4,5
```

```
python3 ./paging-policy.py -s 1 -n 10
Access: 1  miss 1
Access: 8  miss 1,8
Access: 7  miss 1,8,7
Access: 2  miss 8,7,2
Access: 4  miss 7,2,4
Access: 4  hit  7,2,4
Access: 6  miss 2,4,6
Access: 7  miss 4,6,7
Access: 0  miss 6,7,0
Access: 0  hit  6,7,0
```

```
python3 ./paging-policy.py -s 0 -n 10 -p LRU
Access: 8  miss 8
Access: 7  miss 8,7
Access: 4  miss 8,7,4
Access: 2  miss 7,4,2
Access: 5  miss 4,2,5
Access: 4  hit  2,5,4
Access: 7  miss 5,4,7
Access: 3  miss 4,7,3
Access: 4  hit  7,3,4
Access: 5  miss 3,4,5
```

```
python3 ./paging-policy.py -s 0 -n 10 -p OPT
Access: 8  miss 8
Access: 7  miss 8,7
Access: 4  miss 8,7,4
Access: 2  miss 7,4,2
Access: 5  miss 7,4,5
Access: 4  hit  7,4,5
Access: 7  hit  7,4,5
Access: 3  miss 4,5,3
Access: 4  hit  4,5,3
Access: 5  hit  4,5,3
```

## 2

```
python3 ./paging-policy.py -C 5 -p FIFO -a 1,2,3,4,5,6,1,2,3,4,5,6 -c
python3 ./paging-policy.py -C 5 -p LRU -a 1,2,3,4,5,6,1,2,3,4,5,6 -c
python3 ./paging-policy.py -C 5 -p MRU -a 1,2,3,4,5,6,5,6 -c
```

To improve performance cache must be >= 6.
