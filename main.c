/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/28 20:17:03 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "philosophers.h"
#include <sys/time.h>

/**
 * @brief				Function to return the current time of day in milliseconds
 *
 * @param	time_stone	mutex to ensure no other threads alter local variables
 * @return	long		return the current time of day in milliseconds
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
 * @param time_stone		mutex to ensure no other threads (philosphers)
 * 							print while a certain philo is printing
 * @param time_of_action	time in ms when action was performed
 * @param phil				id of philosopher that performed an action
 * @param action			the action performed by the philosopher
 */
void kiraman_katibin(pthread_mutex_t *time_stone, long time_of_action, int phil, char *action)
{
	pthread_mutex_lock(&*time_stone);
	usleep(100);
	printf("%lu %d %s", time_of_action, phil, action);
	pthread_mutex_unlock(&*time_stone);
}

/**
 * @brief				Function for the main thread to check for any deaths in the simulation
 *
 * @param time_stone	mutex to ensure no other thread performs any action as a death check is being performed
 * @param arg			all data pertaining to the simulation
 * @param dna			common shared data for all the philosphers
 */
void call_waiter(pthread_mutex_t *time_stone, void *arg, t_dna dna)
{
	int i;
	int status;
	int all_ate;

	all_ate = 0;
	t_table *table = (t_table *)(arg);
	t_philo **p;
	status = 1;
	p = (table->philos);
	while (2)
	{
		i = 0;
		all_ate = 1;
		while (i < dna.gene_pool)
		{
			// pthread_mutex_lock(&*time_stone);
			pthread_mutex_lock(&p[i]->soul);
			if (p[i]->alive == 0)
			{
				status = 0;
				// pthread_mutex_unlock(&table->time_stone_mutex);
				kiraman_katibin(&*time_stone, p[i]->dead - p[i]->alive, i, "has died\n");
				pthread_mutex_unlock(&p[i]->soul);
				exit(0);
			}
			if (p[i]->plates < p[i]->dna.meals)
				all_ate &= 0;
			pthread_mutex_unlock(&p[i]->soul);
			// pthread_mutex_unlock(&*time_stone);
			if (status == 0)
				return;
			i++;
			// pthread_mutex_unlock(&*time_stone);
		}
		if (dna.meals != -1 && all_ate == 1)
			exit(0);
	}
}

/**
 * @brief				Function to attempt to pick up the fork to the left of the philosopher
 *
 * @param	f			pointer to the array of forks
 * @param	id			id of the fork to pick up
 * @return	int			status of the attempt to pick up the fork ( '0' for fail and '1' for success)
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
 * @brief			Function to attempt to pick up the fork to the right of the philosopher
 *
 * @param	f		Pointer to the array of forks
 * @param	id		id of the fork to pick up
 * @param	n		number of philosphers in the simulation
 * @return	int		status of the attempt to pick up the fork ('0' for fail and '1' for success)
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

// int think(t_philo **p, t_fork **f, int id, t_dna dna);

// deprecated eat:
/*
int eat(t_philo **p, t_fork **f, int id, t_dna dna)
{
	int first;
	int second;
	pthread_mutex_lock(&p[id]->soul);
	first = id;
	second = (id + 1) % dna.gene_pool;
	if (id % 2 == 1)
	{
		first = (id + 1) % dna.gene_pool;
		second = id;
	}
	long time_start;
	struct timeval t;
	// pthread_mutex_lock(&(*p[id]->time_stone));
	pthread_mutex_lock(&f[first]->fork_mutex);
	f[first]->fork = 0;
	pthread_mutex_lock(&f[second]->fork_mutex);
	f[second]->fork = 0;
	// pthread_mutex_unlock(&*p[id]->time_stone);
	long timer = 0;
	time_start = time_stamp();
	p[id]->last_meal = time_start;
	pthread_mutex_lock(&(*p[id]->time_stone));
	kiraman_katibin(time_start - p[id]->birth, id, "is eating\n");
	pthread_mutex_unlock(&*p[id]->time_stone);
	// usleep(p[id]->dna.time_to_die);
	while (timer < p[id]->dna.time_to_eat)
	{
		usleep(50);
		timer = time_stamp() - time_start;
		if (timer + time_start - p[id]->last_meal >= dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = timer + time_start - p[id]->birth;
			break;
		}
	}
	pthread_mutex_lock(&*p[id]->time_stone);
	kiraman_katibin(time_stamp() - p[id]->birth, id, "finished eating\n");
	p[id]->plates++;
	pthread_mutex_unlock(&*p[id]->time_stone);
	f[id]->fork = 1;
	pthread_mutex_unlock(&f[first]->fork_mutex);
	f[(id + 1) % dna.gene_pool]->fork = 1;
	pthread_mutex_unlock(&f[second]->fork_mutex);
	pthread_mutex_unlock(&p[id]->soul);
	return (0);
}
*/

void new_eat(t_philo **p, t_fork **f, int id, t_dna dna)
{
	int first;
	int second;
	long time_start;
	struct timeval t;
	long timer = 0;

	first = id;
	second = (id + 1) % dna.gene_pool;
	if (id % 2 == 1)
	{
		first = (id + 1) % dna.gene_pool;
		second = id;
	}
	pthread_mutex_lock(&*p[id]->time_stone);
	p[id]->hungry[id] = id;
	p[id]->hungry[(id + 1) % dna.gene_pool] = id;
	pthread_mutex_unlock(&*p[id]->time_stone);
	pthread_mutex_lock(&f[first]->fork_mutex);
	f[first]->fork = 0;
	pthread_mutex_lock(&f[second]->fork_mutex);
	f[second]->fork = 0;
	time_start = time_stamp(&*p[id]->time_stone);
	p[id]->last_meal = time_start;
	kiraman_katibin(&*p[id]->time_stone, time_start - p[id]->birth, id, "is eating\n");

	while (timer < p[id]->dna.time_to_eat)
	{
		usleep(50);
		timer = time_stamp(&*p[id]->time_stone) - time_start;
		if (timer + time_start - p[id]->last_meal >= dna.time_to_die)
		{
			pthread_mutex_lock(&p[id]->soul);
			p[id]->alive = 0;
			p[id]->dead = timer + time_start - p[id]->birth;
			pthread_mutex_unlock(&p[id]->soul);
			break;
		}

	}
	p[id]->plates++;
	f[second]->fork = 1;
	pthread_mutex_unlock(&f[second]->fork_mutex);
	f[first]->fork = 1;
	pthread_mutex_unlock(&f[first]->fork_mutex);

}

/**
 * @brief 		Function that waits until forks are available for thw philos to eat
 *
 * @param p		Pointer to array of philosophers
 * @param f		Pointer to array of forks
 * @param id	ID of the philospher that is picking up forks
 * @param dna	common shared data for all the philosphers
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
		// usleep(50);
	}

}


void pick_up_both_forks(t_philo **p, t_fork **f, int id, t_dna dna)
{
	while (2)
	{
		try_to_pick_up_forks(p, f, id, dna);
		pthread_mutex_lock(&*p[id]->time_stone);
		if (p[id]->hungry[id] != id && p[id]->hungry[(id + 1) % dna.gene_pool] != id)
		{
			pthread_mutex_unlock(&*p[id]->time_stone);
			new_eat(p,f,id,dna);
			return ;
		}
		pthread_mutex_unlock(&*p[id]->time_stone);
		usleep(50);
	}
}

// deprecated:
/*
int try_to_eat(t_philo **p, t_fork **f, int id, t_dna dna)
{
	int status;

	pthread_mutex_lock(&*p[id]->time_stone);
	if (id % 2 == 0)
	{
		status = pick_left_fork(p, id, f);
		status &= pick_right_fork(p, id, f, dna.gene_pool);
	}
	else
	{
		status = pick_right_fork(p, id, f, dna.gene_pool);
		status &= pick_left_fork(p, id, f);
	}
	if (status == 0)
	{
		pthread_mutex_unlock(&*p[id]->time_stone);
		think(p, f, id, dna);
	}
	else
	{
		pthread_mutex_unlock(&*p[id]->time_stone);
		while (2)
		{
			pthread_mutex_lock(&p[id]->soul);
			if (p[id]->hungry[id] != id && p[id]->hungry[(id + 1) % dna.gene_pool] != id)
			{
				pthread_mutex_unlock(&p[id]->soul);
				break;
			}
			usleep(50);
			pthread_mutex_unlock(&p[id]->soul);
		}
		pthread_mutex_lock(&p[id]->soul);
		pthread_mutex_lock(&*p[id]->time_stone);
		p[id]->hungry[id] = id;
		p[id]->hungry[(id + 1) % dna.gene_pool] = id;
		pthread_mutex_unlock(&*p[id]->time_stone);
		pthread_mutex_unlock(&p[id]->soul);
		eat(p, f, id, dna);
	}
	return (1);
}
*/

// deprecated:
/*
int think(t_philo **p, t_fork **f, int id, t_dna dna)
{
	long thought_for = 0;
	long t_start;
	struct timeval t;
	int f1;
	int f2;
	t_start = time_stamp();
	if (id % 2 == 0)
	{
		f1 = id;
		f2 = (id + 1) % dna.gene_pool;
	}
	else
	{
		f1 = (id + 1) % dna.gene_pool;
		f2 = id;
	}
	pthread_mutex_lock(&p[id]->soul);
	while (2)
	{
		pthread_mutex_lock(&f[id]->fork_mutex);
		pthread_mutex_lock(&f[(id + 1) % dna.gene_pool]->fork_mutex);
		if (f[f1]->fork == 1 && f[f2]->fork == 1)
		{
			pthread_mutex_unlock(&f[id]->fork_mutex);
			pthread_mutex_unlock(&f[(id + 1) % dna.gene_pool]->fork_mutex);
			pthread_mutex_unlock(&p[id]->soul);
			eat(p, f, id, dna);
			break;
		}
		pthread_mutex_unlock(&f[id]->fork_mutex);
		pthread_mutex_unlock(&f[(id + 1) % dna.gene_pool]->fork_mutex);
		usleep(50);
		thought_for = time_stamp() - t_start;
		if (thought_for + p[id]->last_meal - t_start >= p[id]->dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = t_start + thought_for - p[id]->birth;
			pthread_mutex_unlock(&p[id]->soul);
			break;
		}
	}
	return (0);
}
*/


/**
 * @brief				Function to let the philospher sleep
 *
 * @param p				Pointer to array of philosphers
 * @param id			id of the philospher who's going to sleep
 * @param time_stone	mutex to prevent dataraces while getting timestamps and printing logs
 * @param dna			common simulation data for all philosphers
 */
void p_sleep(t_philo **p, int id, pthread_mutex_t *time_stone, t_dna dna)
{
	long slept_for;
	long t_start;
	long time_to_sleep;

	t_start = time_stamp(&*time_stone);
	slept_for = 0;
	kiraman_katibin(time_stone, t_start - p[id]->birth, id, "is sleeping\n");
	pthread_mutex_lock(&p[id]->soul);
	while (slept_for < dna.time_to_sleep)
	{
		usleep(50);
		slept_for = time_stamp(&*time_stone) - t_start;
		if (slept_for + t_start - p[id]->last_meal >= dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = t_start + slept_for - p[id]->birth;
			break;
		}
	}
	pthread_mutex_unlock(&p[id]->soul);
}

/**
 * @brief			The thread routine
 *
 * @param	arg		Philospher data
 * @return	void*	Return value (always NULL)
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
		// new_eat(p->others, p->forks, p->id, p->dna);
		p_sleep(&*(p->others), p->id, &*p->time_stone, p->dna);
		kiraman_katibin(&*(p->time_stone), time_stamp(&*(p->time_stone)) - p->birth, p->id, "is thinking\n");
	}
	return (NULL);
}

void clean_table(t_table *table)
{
}

int main(int argc, char **argv)
{
	int flag;
	t_table table_1;
	t_dna dna;
	t_philo **philos;
	t_philo *temp;
	parse_args(argc, argv, &dna);
	// First we prepare the table with al the philos:
	philos = malloc(sizeof(t_philo *) * dna.gene_pool);
	int i;
	for (i = 0; i < dna.gene_pool; i++)
	{
		temp = (t_philo *)malloc(sizeof(t_philo));
		temp->id = i;
		temp->dna = dna;
		philos[i] = temp;
	}
	table_1.philos = philos;
	// now we prep the forks - one per philo:
	t_fork **forks;
	forks = malloc(dna.gene_pool * sizeof(t_fork));
	for (i = 0; i < dna.gene_pool; i++)
	{
		forks[i] = (t_fork *)malloc(sizeof(t_fork));
		forks[i]->fork = 1;
	}
	long time_now;
	struct timeval t;
	gettimeofday(&t, NULL);
	time_now = t.tv_sec * 1000 + t.tv_usec / 1000;
	int h[210] = {-1};
	for (i = 0; i < dna.gene_pool; i++)
	{
		philos[i]->others = philos;
		philos[i]->forks = forks;
		philos[i]->last_meal = time_now;
		philos[i]->id = i;
		philos[i]->birth = time_now;
		philos[i]->alive = 1;
		philos[i]->plates = 0;
		philos[i]->hungry = h;
	}
	pthread_mutex_init(&(table_1.time_stone_mutex), NULL);
	for (int i = 0; i < dna.gene_pool; i++)
	{
		pthread_mutex_init(&(forks[i]->fork_mutex), NULL);
		pthread_mutex_init(&(philos[i]->soul), NULL);
		philos[i]->time_stone = &table_1.time_stone_mutex;
	}
	table_1.philos = philos;
	table_1.forks = forks;

	for (int i = 0; i < dna.gene_pool; i++)
		pthread_create(&(philos[i]->life), NULL, &life_cycle, (void **)(&*(philos[i])));
	call_waiter(&(table_1.time_stone_mutex), &table_1, dna);
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_join(philos[i]->life, NULL);
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_detach(philos[i]->life);
	for (int i = 0; i < dna.gene_pool; i++)
	{
		pthread_mutex_destroy(&(forks[i]->fork_mutex));
		pthread_mutex_destroy(&(philos[i]->soul));
	}
	pthread_mutex_destroy(&table_1.time_stone_mutex);
	// clean_table(&table_1);
	return (0);
}
