## 1

> Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.

```
python3 ./lottery.py -j 3 -s 1

Here is the job list, with the run time of each job:
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )

#tickets = 84+25+44 = 153

Random 651593 mod 153 = 119

Run 2 -> 1,7,3

Random 788724 mod 153 = 9

Run 0 -> 0,7,3

  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )

#tickets = 25+44 = 69

Random 93859 mod 69 = 19

Run 1 -> 6,4

Random 28347 mod 69 = 57

Run 2 -> 6,3

Random 835765 mod 69 = 37

Run 2 -> 6,2

Random 432767 mod 69 = 68

Run 2 -> 6,1

Random 762280 mod 69 = 37

Run 2 -> 6,0

  Job 1 ( length = 7, tickets = 25 )

#tickets = 25

Run 1 -> 6
Run 1 -> 5
Run 1 -> 4
Run 1 -> 3
Run 1 -> 2
Run 1 -> 1
Run 1 -> 0
```

```
python3 ./lottery.py -j 3 -s 2

Here is the job list, with the run time of each job:
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )

#tickets = 94+73+30 = 197

Random 605944 mod 197 = 169
Run 2 -> 9,8,5

Random 606802 mod 197 = 42
Run 0 -> 8,8,5

Random 581204 mod 197 = 54
Run 0 -> 7,8,5

Random 158383 mod 197 = 192
Run 2 -> 7,8,4

Random 430670 mod 197 = 28
Run 0 -> 6,8,4

Random 393532 mod 197 = 123
Run 1 -> 6,7,4

Random 723012 mod 197 = 22
Run 0 -> 5,7,4

Random 994820 mod 197 = 167
Run 2 -> 5,7,3

Random 949396 mod 197 = 53
Run 0 -> 4,7,3

Random 544177 mod 197 = 63
Run 0 -> 3,7,3

Random 444854 mod 197 = 28
Run 0 -> 2,7,3

Random 268241 mod 197 = 124
Run 1 -> 2,6,3

Random 35924 mod 197 = 70
Run 0 -> 1,6,3

Random 27444 mod 197 = 61
Run 0 -> 0,6,3

  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )

#tickets = 73+30 = 103

Random 464894 mod 103 = 55
Run 1 -> 5,3

Random 318465 mod 103 = 92
Run 2 -> 5,2

Random 380015 mod 103 = 48
Run 1 -> 4,2

Random 891790 mod 103 = 16
Run 1 -> 3,2

Random 525753 mod 103 = 41
Run 1 -> 2,2

Random 560510 mod 103 = 87
Run 2 -> 2,1

Random 236123 mod 103 = 47
Run 1 -> 1,1

Random 23858 mod 103 = 65
Run 1 -> 0,1

  Job 2 ( length = 6, tickets = 30 )
#tickets = 30

Random 325143 mod 30 = 3
Run 2 -> 0
```

```
python3 ./lottery.py -j 3 -s 3

Here is the job list, with the run time of each job:
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )

#tickets = 54+60+6 = 120

Here is the set of random numbers you will need (at most):
Random 13168 mod 120 = 88
Run 1 -> 2,2,6

Random 837469 mod 120 = 109
Run 1 -> 2,1,6

Random 259354 mod 120 = 34
Run 0 -> 1,1,6

Random 234331 mod 120 = 91
Run 1 -> 1,0,6

  Job 0 ( length = 1, tickets = 54 )
  Job 2 ( length = 6, tickets = 6 )

#tickets = 54+6 = 60

Random 995645 mod 60 = 5
Run 0 -> 0,6

  Job 2 ( length = 6, tickets = 6 )

#tickets = 6

Random 470263
Run 6
Random 836462
Run 6
Random 476353
Run 6
Random 639068
Run 6
Random 150616
Run 6
Random 634861
```

## 2

> Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., -l 10:1,10:100).

> What happens when the number of tickets is so imbalanced?

Job 0 almost never runs before job 1 is finished.

> Will job 0 ever run before job 1 completes? How often?

There is 1/101 chance the job 0 runs before job 1 completes.

> In general, what does such a ticket imbalance do to the behavior of lottery scheduling?

Jobs with few tickets run after the other jobs have finished.

## 3

> When running with two jobs of length 100 and equal ticket allocations of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.

```
python3 ./lottery.py -l 100:100,100:100 -s 1 -c
F = 196/200 = 0,98

python3 ./lottery.py -l 100:100,100:100 -s 2 -c
F = 190/200 = 0,95

python3 ./lottery.py -l 100:100,100:100 -s 3 -c
F = 196/200 = 0,98
```

Scheduler is rather fair.

## 4

> How does your answer to the previous question change as the quantum size (-q) gets larger?

```
python3 ./lottery.py -l 100:100,100:100 -s 2334 -q 5 -c
F = 155/200 = 0,775

python3 ./lottery.py -l 100:100,100:100 -s 2334 -q 10 -c
F = 170/200 = 0,85

python3 ./lottery.py -l 100:100,100:100 -s 233 -q 30 -c
F = 180/240 = 0,75
```

Fairness becomes worse.
