# Operating Systems and Networks 
## Assignment 4
### File Contents
1) q1.c - concurrent merge sort using processes
2) q1_normal.c - Normal merge sort 
3) q1_threads.c - Concurrent merge sort using threads
### Observations (n = number of input numbers  and time is measured in CPU clock ticks)
1) When n=10, q1.c took 350 (+ or - 100) CPU clock ticks, normal merge sort took 71 such ticks
2) When n=100, q1.c took 423 CPU clock ticks, while normal merge sort took 220 such ticks
3) When n=1000, q1.c took 477 CPU clock ticks, while normal merge sort took 1071 such ticks
4) When n=10000, q1.c took approximately 2733 (+ or - 1000) CPU clock ticks, while normal merge sort took 7869 (+ or - 1000)such ticks
5) When n=100000, q1.c was not able to spawn such a huge number of processes (Error statement: Resource temporarily unavailable), hence couldn't compare.
6) Note: All the aforementioned observations are approximations as CPU took variable amount of clock ticks for the same input but the variance is not as great as the measurent itself (i.e variance can be ignored)
### Explanation for above observations
1) When n=10 and 100, normal merge sort ran faster than the concurrent merge sort (as opposed to our expectation). Reason: Since n is really small, it takes very less time to compute the answer, usually around 50 clock ticks. So, dividing this into multiple processes and taking advantage of multiple cores would generally not give a big difference. But, the overhead involved in spawning child processes that needs to be computed by the cpu takes reasonably good amount of time. And, this amount of time is greater than the actual run time of normal merge sort, and hence the result. Conclusion: Though q1.c sorts fast, the overhead associated with creating processes is the one that kills time.

2) When, n=1000 and 10000, we see that q1.c is much faster than normal merge sort. So, here, sorting 1000 integers takes much more time than the overhead required to create processes. And once we have several such processes, our multi-core system aids it by dividing the job and hence gets the work done in very less amount of time (Infact time taken by normal merge sort is exponentially more than that taken by concurrent merge sort).