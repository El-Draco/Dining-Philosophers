/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/30 11:27:26 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "philosophers.h"
#include <sys/time.h>

/**
 * @brief					Function to return the current time of day in milliseconds
 *
 * @param	time_stone		Mutex to ensure no other threads alter local variables
 * @return	long			Return the current time of day in milliseconds
 */
long time_stamp(pthread_mutex_t *time_stone)
{
	long t_now;
	struct timeval t;

	pthread_mutex_lock(&*time_stone);
	gettimeofday(&t, NULL);
	t_now = t.tv_sec * 1000 + t.tv_usec / 1000;
	pthread_mutex_unlock(&*time_stone);
	return t_now;
}

/**
 * @brief					Function to print log entries of action sundertaken by philosphers
 *
 * @param time_stone		Mutex to ensure no other threads (philosphers)
 * 							print while a certain philo is printing
 * @param time_of_action	Time in ms when action was performed
 * @param phil				ID of philosopher that performed an action
 * @param action			The action performed by the philosopher
 */
void kiraman_katibin(pthread_mutex_t *time_stone, long time_of_action, int phil, char *action)
{
	pthread_mutex_lock(&*time_stone);
	usleep(10);
	printf("%lu %d %s", time_of_action, phil, action);
	pthread_mutex_unlock(&*time_stone);
}


void kill_simulation(t_table *table, int n);

/**
 * @brief					Function for the main thread to check for any deaths in the simulation
 *
 * @param time_stone		Mutex to ensure no other thread performs any action as a death check is being performed
 * @param arg				All data pertaining to the simulation
 * @param dna				Common shared data for all the philosphers
 */
void call_waiter(pthread_mutex_t *time_stone, t_table *table, t_dna dna)
{
	int i;
	int status;
	int all_ate;

	all_ate = 0;
	t_philo **p;
	status = 1;
	p = (table->philos);
	while (2)
	{
		i = 0;
		all_ate = 1;
		while (i < dna.gene_pool)
		{
			pthread_mutex_lock(&p[i]->soul);
			if (p[i]->alive == 0)
			{
				status = 0;
				pthread_mutex_unlock(&p[i]->soul);
				kiraman_katibin(&*time_stone, p[i]->dead - p[i]->alive, i, "has died\n");
				kill_simulation(table, dna.gene_pool);
				return ;
			}
			if (p[i]->plates < p[i]->dna.meals)
				all_ate &= 0;
			pthread_mutex_unlock(&p[i]->soul);
			if (status == 0)
				return;
			i++;
		}
		if (dna.meals != -1 && all_ate == 1)
		{
			usleep(50);
			kill_simulation(table, dna.gene_pool);
			return ;
		}
	}
}

/**
 * @brief					Function to attempt to pick up the fork to the left of the philosopher
 *
 * @param	f				Pointer to the array of forks
 * @param	id				ID of the fork to pick up
 * @return	int				Status of the attempt to pick up the fork ( '0' for fail and '1' for success)
 */
int pick_left_fork(t_fork **f, int id)
{
	int status;

	pthread_mutex_lock(&f[id]->fork_mutex);
	status = 1;
	if (f[id]->fork == 0)
		status = 0;
	else
		f[id]->fork = 1;
	pthread_mutex_unlock(&f[id]->fork_mutex);
	return status;
}

/**
 * @brief					Function to attempt	 to pick up the fork to the right of the philosopher
 *
 * @param	f				Pointer to the array of forks
 * @param	id				ID of the fork to pick up
 * @param	n				Number of philosphers in the simulation
 * @return	int				Status of the attempt to pick up the fork ('0' for fail and '1' for success)
 */
int pick_right_fork(t_fork **f, int id, int n)
{
	int status;

	pthread_mutex_lock(&(f[(id + 1) % n]->fork_mutex));
	status = 1;
	if (f[(id + 1) % n]->fork == 0)
		status = 0;
	else
		f[(id + 1) % n]->fork = 1;
	pthread_mutex_unlock(&(f[(id + 1) % n]->fork_mutex));
	return status;
}

/**
 * @brief 					Function to determine the optimum lock order and lock the required forks
 *
 * @param f					Pointer to the array of forks
 * @param id				ID of the philosopher who's going to eat
 * @param n					Number of philosophers in the simulation
 */
void lock_forks(t_fork **f, int id, int n)
{
	int first;
	int second;

	first = id;
	second = (id + 1) % n;
	if (id % 2 == 1)
	{
		first = (id + 1) % n;
		second = id;
	}
	pthread_mutex_lock(&f[first]->fork_mutex);
	f[first]->fork = 0;
	pthread_mutex_lock(&f[second]->fork_mutex);
	f[second]->fork = 0;
}

/**
 * @brief 					Function to determine the correct lock order and unlock the required forks
 *
 * @param f					Pointer to the array of forks
 * @param id				ID of the philosopher who finished eating
 * @param n					Number of philosophers in the simulation
 */
void unlock_forks(t_fork **f, int id, int n)
{
	int first;
	int second;

	first = id;
	second = (id + 1) % n;
	if (id % 2 == 1)
	{
		first = (id + 1) % n;
		second = id;
	}
	f[second]->fork = 1;
	pthread_mutex_unlock(&f[second]->fork_mutex);
	f[first]->fork = 1;
	pthread_mutex_unlock(&f[first]->fork_mutex);
}

/**
 * @brief 					Function to let the philospher eat and reset time of last meal
 *
 * @param p					Pointer to array of philosophers
 * @param f					Pointer to the array of forks
 * @param id				ID of the philosopher about to eat
 * @param dna				Common shared data for all the philosphers
 */
void eat(t_philo **p, t_fork **f, int id, t_dna dna)
{
	long time_start;
	struct timeval t;
	long timer = 0;

	// lock_forks(f, id, dna.gene_pool);
	time_start = time_stamp(&*p[id]->time_stone);
	p[id]->last_meal = time_start;
	kiraman_katibin(&*p[id]->time_stone, time_start - p[id]->birth, id, "is eating\n");
	while (timer < p[id]->dna.time_to_eat)
	{
		usleep(50);
		timer = time_stamp(&*p[id]->time_stone) - time_start;
		pthread_mutex_lock(&p[id]->soul);
		if (timer + time_start - p[id]->last_meal >= dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = timer + time_start - p[id]->birth;
			pthread_mutex_unlock(&p[id]->soul);
			break;
		}
		pthread_mutex_unlock(&p[id]->soul);
	}
	// unlock_forks(f, id, dna.gene_pool);
}

/**
 * @brief 					Function that waits until forks are available for thw philos to eat
 *
 * @param p					Pointer to array of philosophers
 * @param f					Pointer to array of forks
 * @param id				ID of the philospher that is picking up forks
 * @param dna				Common shared data for all the philosphers
 */
void try_to_pick_up_forks(t_philo **p, t_fork **f, int id, t_dna dna)
{
	int status;

	status = 0;
	while (status == 0)
	{
		status = 0;
		if (id % 2 == 0)
		{
			status = pick_left_fork(f, id);
			status &= pick_right_fork(f, id, dna.gene_pool);
		}
		else
		{
			status = pick_right_fork(f, id, dna.gene_pool);
			status &= pick_left_fork(f, id);
		}
	}
}

/**
 * @brief 					Function that waits until 2 forks are available and verifies that
 * 							a philosopher that has already eaten won't attempt to eat again
 *
 * @param p					Pointer to array of philosophers
 * @param f					Pointer to array of forks
 * @param id				ID of the philospher that is picking up forks
 * @param dna				Common shared data for all the philosphers
 */
void pick_up_both_forks(t_philo **p, t_fork **f, int id, t_dna dna)
{
	while (2)
	{
		try_to_pick_up_forks(p, f, id, dna);
		// if (pick_left_fork(f, id) == 0 && pick_right_fork(f, id, dna.gene_pool) == 0)
			// continue;
		lock_forks(f,id,dna.gene_pool);
		pthread_mutex_lock(&*p[id]->time_stone);
		if (p[id]->hungry[id] != id && p[id]->hungry[(id + 1) % dna.gene_pool] != id)
		{
			p[id]->hungry[id] = id;
			p[id]->hungry[(id + 1) % dna.gene_pool] = id;
			pthread_mutex_unlock(&*p[id]->time_stone);
			pthread_mutex_lock(&p[id]->soul);
			p[id]->plates++;
			pthread_mutex_unlock(&p[id]->soul);
			eat(p, f, id, dna);
			unlock_forks(f,id,dna.gene_pool);
			return;
		}
		unlock_forks(f,id,dna.gene_pool);
		pthread_mutex_unlock(&*p[id]->time_stone);
	}
}

/**
 * @brief					Function to let the philospher sleep
 *
 * @param p					Pointer to array of philosphers
 * @param id				ID of the philospher who's going to sleep
 * @param time_stone		Mutex to prevent dataraces while getting timestamps and printing logs
 * @param dna				Common simulation data for all philosphers
 */
void p_sleep(t_philo **p, int id, pthread_mutex_t *time_stone, t_dna dna)
{
	long slept_for;
	long t_start;
	long time_to_sleep;

	t_start = time_stamp(&*time_stone);
	slept_for = 0;
	kiraman_katibin(time_stone, t_start - p[id]->birth, id, "is sleeping\n");
	while (slept_for < dna.time_to_sleep)
	{
		usleep(50);
		slept_for = time_stamp(&*time_stone) - t_start;
		pthread_mutex_lock(&p[id]->soul);
		if (slept_for + t_start - p[id]->last_meal >= dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = t_start + slept_for - p[id]->birth;
			pthread_mutex_unlock(&p[id]->soul);
			break;
		}
		pthread_mutex_unlock(&p[id]->soul);

	}
	pthread_mutex_unlock(&p[id]->soul);
}

/**
 * @brief					The thread routine for each philosopher (eat - sleep - think)
 *
 * @param	arg				Data peratining to a particular philospher (thread)
 * @return	void*			Return value (always NULL)
 */
void *life_cycle(void *arg)
{
	t_philo *p;

	p = ((t_philo *)arg);
	while (2)
	{
		if (p->dna.gene_pool == 1)
		{
			pthread_mutex_lock(&p->soul);
			p->alive = 0;
			p->dead = time_stamp(&*(p->time_stone)) - p->birth;
			pthread_mutex_unlock(&p->soul);
			break;
		}
		pick_up_both_forks(p->others, p->forks, p->id, p->dna);
		p_sleep(&*(p->others), p->id, &*p->time_stone, p->dna);
		kiraman_katibin(&*(p->time_stone), time_stamp(&*(p->time_stone)) - p->birth, p->id, "is thinking\n");
	}
	return (NULL);
}

//malloc philos
t_philo **prepare_philos(t_dna *dna, t_fork **forks, int hungry[])
{
	t_philo **philos;
	long time_now;
	struct timeval t;
	int i;
	philos = malloc(sizeof(t_philo *) * dna->gene_pool);
	gettimeofday(&t, NULL);
	time_now = t.tv_sec * 1000 + t.tv_usec / 1000;
	for (i = 0; i < dna->gene_pool; i++)
	{
		philos[i] = (t_philo *)malloc(sizeof(t_philo));
		philos[i]->id = i;
		philos[i]->dna = *dna;
		philos[i]->others = philos;
		philos[i]->forks = forks;
		philos[i]->last_meal = time_now;
		philos[i]->birth = time_now;
		philos[i]->alive = 1;
		philos[i]->plates = 0;
		philos[i]->hungry = hungry;
	}
	return (philos);
}

t_table *prepare_table(t_dna *dna)
{
	int i;
	int *hungry;
	t_table *table_1;
	t_fork **forks;

	i = -1;
	table_1 = malloc(sizeof(t_table));
	forks = malloc(dna->gene_pool * sizeof(t_fork *));
	while (++i < dna->gene_pool)
	{
		forks[i] = malloc(dna->gene_pool * sizeof(t_fork));
		forks[i]->fork = 1;
	}
	table_1->forks = forks;
	return table_1;
}

void initialize_mutexes(t_table *table, t_dna *dna)
{
	pthread_mutex_init(&(table->time_stone_mutex), NULL);
	for (int i = 0; i < dna->gene_pool; i++)
	{
		pthread_mutex_init(&(table->forks[i]->fork_mutex), NULL);
		pthread_mutex_init(&(table->philos[i]->soul), NULL);
		table->philos[i]->time_stone = &table->time_stone_mutex;
	}
}

void destroy_mutexes(t_table *table, int n)
{
	int i;

	i = 0;
	while(i < n)
	{
		pthread_mutex_destroy(&(table->forks[i]->fork_mutex));
		pthread_mutex_destroy(&(table->philos[i]->soul));
		i++;
	}
	pthread_mutex_destroy(&(table->time_stone_mutex));
}

void clean_table(t_table *table, int n);

void kill_simulation(t_table *table, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		table->philos[i]->alive = 0;
		i++;
	}
	destroy_mutexes(table, n);
	clean_table(table, n);
	// usleep(50);
	exit(0);
}


t_table *initialize_simulation(t_dna *dna)
{
	t_table *table;
	int *hungry;

	table = prepare_table(dna);
	hungry = malloc(dna->gene_pool * (sizeof(int)));
	table->hungry = hungry;
	table->philos = prepare_philos(dna, table->forks, hungry);
	initialize_mutexes(table, dna);
	return (table);
}

void clean_table(t_table *table, int n)
{
	int i;
	i = 0;
	while (i < n)
	{
		free(table->forks[i]);
		free(table->philos[i]);
		i++;
	}
	free(table->forks);
	free(table->hungry);
	free(table->philos);
	free(table);

	// destroy_mutexes(table, n);
}

int main(int argc, char **argv)
{
	int flag;
	t_dna dna;
	t_table *table;
	t_philo **philos;

	parse_args(argc, argv, &dna);
	table = initialize_simulation(&dna);
	philos = table->philos;
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_create(&(philos[i]->life), NULL, &life_cycle, (void **)(&*(philos[i])));
	call_waiter(&(table->time_stone_mutex), table, dna);
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_join(philos[i]->life, NULL);
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_detach(philos[i]->life);
	return (0);
}
