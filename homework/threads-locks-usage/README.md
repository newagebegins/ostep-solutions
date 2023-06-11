## Hand-over-hand locking tests

40000 numbers:

```
% ./list_st       
time = 4.71359 secs
% ./list_mt_coarse
time = 4.84769 secs
% ./list_mt_hoh   
time = 8.38457 secs
```

80000 numbers:

```
% ./list_st                         
time = 24.9648 secs
% ./list_mt_coarse 
time = 25.189 secs
% ./list_mt_hoh   
time = 34.4732 secs
```

120000 numbers:

```
% ./list_st
time = 62.4617 secs
% ./list_mt_coarse
time = 61.9709 secs
% ./list_mt_hoh   
time = 83.3569 secs
```

Single-threaded and coarse multi-threaded have similar performance. Hand-over-hand locking is slower.
