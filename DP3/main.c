/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/24 19:04:15 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>

void call_waiter()
{

}

//timestamp
void	kiraman_katibin(long time_of_action, int phil, char *action)
{
	printf("%lu %d %s", time_of_action, phil, action);
}

int pick_left_fork(t_philo *p, int id, t_fork *f)
{
	if (f[id].fork == 0)
		return (0);
	pthread_mutex_lock(&f[id].fork_mutex);
	f[id].fork = 1;
	return 0;
}

int pick_right_fork(t_philo *p, int id, t_fork *f)
{
	if (f[(id + 1) % p->dna.gene_pool].fork == 0)
		return (0);
	pthread_mutex_lock(&f[(id + 1) % p->dna.gene_pool].fork_mutex);
	f[(id + 1) % p->dna.gene_pool].fork = 1;
	return 0;
}

int think(t_philo *p)
{

}

int try_to_eat(t_philo *p, t_fork *f)
{
	int id = p->id;
	int status;
	if (id % 2 == 0)
	{
		status = pick_left_fork(p, id, f);
		status &= pick_right_fork(p, id, f);
	}
	else
	{
		status = pick_right_fork(p, id, f);
		status &= pick_left_fork(p, id, f);
	}
	if (status == 0)
		think(p);

	long time_now;
	struct	timeval t;
	gettimeofday(&t,NULL);
	time_now = t.tv_sec * 1000000 + t.tv_usec;
	long start = 0;
	while (start < p->dna.time_to_die && p->alive == 1)
	{
		usleep(50);
		start += 50;
		time_now += 50;
	}


	return (1);
}

int	eat(void *arg)
{
	t_philo *p;
	t_fork *f;
	p = ((t_philo *)arg);

	//pick up fork:
	try_to_eat(p,f);

	//change philo mood
	p->mood = ATE;

	return (0);
}

int p_sleep(void *arg)
{
	long slept_for;
	long t_now;
	long time_to_sleep;
	t_philo *p;
	struct	timeval t;

	p = ((t_philo *)arg);
	gettimeofday(&t,NULL);
	t_now = t.tv_sec * 1000000 + t.tv_usec;
	time_to_sleep = p->dna.time_to_sleep;
	printf("%lu Philo %d is sleeping\n", t_now, p->id);
	slept_for = 0;
	while (t_now + slept_for - p->last_meal < p->dna.time_to_die && slept_for < time_to_sleep)
	{
		usleep(10);
		slept_for += 10;
	}
	return (0);
}

void	*life_cycle(void *arg)
{
	while (2)
	{
		//attempt to eat
		//else think until available
		//sleep
		p_sleep(arg);
	}

}

void clean_table()
{

}
int main(int argc , char **argv)
{
	int flag;
	t_dna dna;
	t_philo *philos;
	parse_args(argc, argv, &dna);

	//First we prepare the table with al the philos:
	for (int i = 0; i < dna.gene_pool; i++)
	{
		philos = malloc(sizeof(t_philo));
		philos->id = i;
		philos->dna = dna;
		philos->mood = HUNGRY;
	}

	//now we prep the forks - one per philo:
	t_fork *forks;
	forks = malloc(dna.gene_pool * sizeof(t_fork));
	for (int i = 0; i < dna.gene_pool; i++)
		forks[i].fork = 0;

	long time_now;
	struct	timeval t;
	gettimeofday(&t,NULL);
	time_now = t.tv_sec * 1000000 + t.tv_usec;
	for (int i = 0; i < dna.gene_pool; i++)
	{
		philos[i].last_meal = time_now;
		philos[i].id = i;
	}

	//now we can get our threads ready for each philo:
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_create(&(philos[i].life), NULL, &life_cycle, (void **)(&philos[i]));

	call_waiter();
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_join(philos[i].life, NULL);
	clean_table();
	return (0);
}
