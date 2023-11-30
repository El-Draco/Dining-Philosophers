# **Dining Philosophers**

## This project is essentially a variant of the infamous Dining Philosphers Problem to illustrate the importance of avoiding data races in a multi-threaded program (by avoiding death of philosophers). Mutexes are used when necessary to avoid race conditions and efficently schedule threads to perform their actions.
---
## The program is done entirely in C using pthreads and mutexes. Semaphores were not used. Extensive testing was done using valgrind to confirm the absence of memory leaks, invalid reads, data races, deadlocks, and mutex lock order violations.
---
## To run the simulation, first clone the repo and run the follwing command:
```
make philo
```
## A binary named 'philo' will be created inside the directory.
## Then execute the program as follows:
```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
```

- ### number_of_philosophers: The number of philosophers and also the number of forks.
- ### time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- ### time_to_eat (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
- ### time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
- ### number_of_times_each_philosopher_must_eat (optional argument): If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

---
## The rules of the program are as follows:

- ## One or more philosophers sit at a round table.
- ## There is a large bowl of spaghetti in the middle of the table.
- ## The philosophers alternatively eat, think, or sleep.
- ## While they are eating, they are not thinking nor sleeping;
- ## while thinking, they are not eating nor sleeping;
- ## and, of course, while sleeping, they are not eating nor thinking.
- ## There are also forks on the table. There are as many forks as philosophers.
- ## Because serving and eating spaghetti with only one fork is very
- ## inconvenient, a philosopher takes their right and their left forks to eat, one in each hand.
- ## When a philosopher has finished eating, they put their forks back on
- ## the table and start sleeping. Once awake, they start thinking again.
- ## The simulation stops when a philosopher dies of starvation.
- ## Every philosopher needs to eat and should never starve.
- ## Philosophers don’t speak with each other.
- ## Philosophers don’t know if another philosopher is about to die.

- ## Each philosopher has a number ranging from 1 to number_of_philosophers.
- ## Philosopher number 1 sits next to philosopher number number_of_philosophers.
- ## Any other philosopher number N sits between philosopher number N - 1 and philosopher number N + 1.
---

## Any state change of a philosopher is formatted as follows:
- ## timestamp_in_ms X has taken a fork
- ## timestamp_in_ms X is eating
- ## timestamp_in_ms X is sleeping
- ## timestamp_in_ms X is thinking
- ## timestamp_in_ms X died