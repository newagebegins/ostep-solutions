1. Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.

```
python3 mlfq.py -j 2 -n 2 -m 30 -M 0 -s 555

Job List:
  Job  0: startTime   0 - runTime   6 - ioFreq   0
  Job  1: startTime   0 - runTime  17 - ioFreq   0

Q1 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1
Q0                                 1 1 1 1 1 1 1

python3 mlfq.py -j 2 -n 2 -m 30 -M 0 -s 514

Job List:
  Job  0: startTime   0 - runTime  11 - ioFreq   0
  Job  1: startTime   0 - runTime   7 - ioFreq   0

Q1 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1
Q0                                   0

python3 mlfq.py -j 2 -n 2 -m 30 -M 0 -s 333

Job List:
  Job  0: startTime   0 - runTime  17 - ioFreq   0
  Job  1: startTime   0 - runTime  29 - ioFreq   0

Q1 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1
Q0                                         0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 ...
```

2. How would you run the scheduler to reproduce each of the examples in the chapter?

```
Figure 8.2:
python3 mlfq.py -l 0,200,0 -n 3 -S -c

Figure 8.3:
python3 mlfq.py -l 0,200,0:100,20,0 -n 3 -S -c

Figure 8.4:
python3 mlfq.py -l 0,200,0:50,20,1 -n 3 -S -c

Figure 8.5 (Without Priority Boost):
python3 mlfq.py -l 0,200,0:100,50,5:100,50,5 -n 3 -S -c

Figure 8.5 (With Priority Boost):
python3 mlfq.py -l 0,200,0:100,20,5:100,20,5 -n 3 -S -B 50 -c

Figure 8.6 (Without Gaming Tolerance):
python3 mlfq.py -l 0,200,0:70,100,9 -i 1 -S -c

Figure 8.6 (With Gaming Tolerance):
python3 mlfq.py -l 0,200,0:70,100,9 -i 1 -c

Figure 8.7:
python3 mlfq.py -l 0,140,0:0,140,0 -n 3 -Q 10,20,40 -c
```

3. How would you configure the scheduler parameters to behave just like a round-robin scheduler?

Use one queue: `python3 mlfq.py -l 0,140,0:0,140,0 -n 1 -S -c`

4. Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.

```
python3 mlfq.py -l 0,200,0:70,100,9 -i 1 -S -c
```

5. Given a system with a quantum length of 10ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?

```
5 / 100 = 10 / 200
-B 200
```

6. One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.

Without -I when job A becomes unblocked job B contunues until the end of time slice:
```
python3 mlfq.py -n 1 -l 0,50,5:0,50,0 -S -c
```

Without I when job A becomes unblocked job B stops and A continues:
```
python3 mlfq.py -n 1 -l 0,50,5:0,50,0 -S -I -c
```
