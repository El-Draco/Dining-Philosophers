/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/24 16:30:15 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>

//timestamp
void	kiraman_katibin(long time_of_action, int phil, char *action)
{
	printf("%lu %d %s", time_of_action, phil, action);
}

void call_waiter(t_queue *table)
{

}
int pick_left_fork(t_philo *p, int id)
{
	pthread_mutex_lock(&(p->soul));
	pthread_mutex_unlock(&(p->soul));
}

int pick_right_fork(t_philo *p, int id)
{
	pthread_mutex_lock(&(p->soul));
	pthread_mutex_unlock(&(p->soul));
}

int keep_left_fork(t_philo *p, int id)
{
	pthread_mutex_lock(&(p->soul));
	pthread_mutex_unlock(&(p->soul));
}

int keep_right_fork(t_philo *p, int id)
{
	pthread_mutex_lock(&(p->soul));
	pthread_mutex_unlock(&(p->soul));
}

int	eat(void *arg)
{
	t_philo *p;

	p = ((t_philo *)arg);

	//pick left_fork - mutexlock
	pick_left_fork(p,p->id);
	//pick right_fork - mutexlock
	pick_right_fork(p, (p->id + 1) % p->dna.gene_pool);
	//change philo mood
	p->mood = ATE;
	//put back left_fork - mutexunclock
	//put back right fork - mutex-unlock
	//print timestamp
	kiraman_katibin();

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
	parse_args(argc, argv, &dna);

	t_philo *philos;

	//First we prepare the table with al the philos:
	t_queue *table;
	table->size = ft_atoi(argv[1], &flag);
	table = newQueue(table->size);
	for (int i = 0; i < table->size; i++)
	{
		philos = malloc(sizeof(t_philo));
		philos->id = i;
		philos->dna = dna;
		philos->mood = HUNGRY;
		enqueue(table, philos);
	}

	//now we prep the forks - one per philo:
	t_fork *forks;
	forks = malloc(table->size * sizeof(t_fork));
	for (int i = 0; i < table->size; i++)
		forks[i].fork = 0;
	table->philos = philos;
	table->forks = forks;

	//now we can get our threads ready for each philo:
	for (int i = 0; i < table->size; i++)
	{
		pthread_create(&(philos[i].life), NULL, &life_cycle, (void **)(&philos[i]));
	}
	call_waiter(table);
	for (int i = 0; i < table->size; i++)
	{
		pthread_join(philos[i].life, NULL);
	}
	clean_table();
	return (0);
}
