1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

```
FIFO:
response time = (0 + 200 + 400) / 3 = 200
turnaround time = (200 + 400 + 600) / 3 = 400

SJF:
response time = (0 + 200 + 400) / 3 = 200
turnaround time = (200 + 400 + 600) / 3 = 400
```

2. Now do the same but with jobs of different lengths: 100, 200, and 300.

```
FIFO:
response time = (0 + 100 + 300) / 3 = 133.3
turnaround time = (100 + 300 + 600) / 3 = 333.3

SJF:
response time = (0 + 100 + 300) / 3 = 133.3
turnaround time = (100 + 300 + 600) / 3 = 333.3
```

3. Now do the same, but also with the RR scheduler and a time-slice of 1.

```
response time = (0 + 1 + 2) / 3 = 1
turnaround time = (300 + 500 + 600) / 3 = 467
```

4. For what types of workloads does SJF deliver the same turnaround times as FIFO?

Workloads that are sorted by length in increasing order.

5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

For workloads like q,q,...,q,q (where q is quantum length).

6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?

Response time increases.

7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?

Response time increases.

```
resp. time = q*(0 + 1 + 2 + 3 + ... + N-1) / N
           = q*N*(N-1) / (2*N)
           = q*(N-1)/2
```
