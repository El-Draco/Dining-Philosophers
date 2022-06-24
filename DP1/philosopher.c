/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 13:16:57 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/21 15:21:35 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

#define PHILO 2

int p_eat(void *arg)
{
	t_philo *p;
	p = ((t_philo *)arg);
	int time_to_eat;

	time_to_eat = p->dna.time_to_eat;
	printf("Philo %d is eating with %d and %d's forks\n", p->id, p->id, p->prev->id);
	usleep(time_to_eat);
	printf("Philo %d is DONE eating with %d and %d's forks\n", p->id, p->id, p->prev->id);

	return (0);
}
int p_think(void *arg)
{
	int count;

	count = 0;
	t_philo *p;
	p = ((t_philo *)arg);
	printf("Philosopher %d is thinking!\n", p->id);
	while (p->fork == -1 && count < p->dna.time_to_die)
	{
		usleep(100);
		count += 100;
	}
	if (count >= p->dna.time_to_die)
	{
		printf("Philosopher %d is done with life lol!\n", p->id);
		exit(0);
	}

	printf("Philosopher %d is done thinking!\n", p->id);
}
int p_sleep(void *arg)
{
	t_philo *p;
	p = ((t_philo *)arg);
	int time_to_sleep;

	time_to_sleep = p->dna.time_to_sleep;
	usleep(time_to_sleep);
	printf("Philo %d is sleeping\n", p->id);
	return (0);
}

void *life_cycle(void *arg)
{
	t_philo *p;
	p = ((t_philo *)arg);
	if (p->prev->fork == -1)
		p_think(arg);
	pthread_mutex_lock(&p->fork_mutex);
	p->fork = -1;
	pthread_mutex_lock(&p->prev->fork_mutex);
	p->prev->fork = -1;

	p_eat(arg);

	pthread_mutex_unlock(&p->fork_mutex);
	p->fork = 1;
	pthread_mutex_unlock(&p->prev->fork_mutex);
	p->prev->fork = 1;
	// sleep:
	p_sleep(arg);
	return (life_cycle(arg));
}

int main(int argc, char **argv)
{
	t_queue *dining_table = NULL;
	t_philo *ps = NULL;
	pthread_mutex_t *mutexes;
	pthread_t *th;
	t_philo *temp;
	// get the philo structures ready and the dining table:
	dining_table = newQueue(PHILO);
	dining_table->size = 0;
	for (int i = 0; i < PHILO; i++)
	{
		ps = malloc(sizeof(t_philo));
		ps->id = i;
		ps->fork = 1; // fork not used
		ps->dna.time_to_eat = 3000000;
		ps->dna.time_to_sleep = 3000000;
		ps->dna.time_to_die = 1000;
		enqueue(dining_table, ps);
	}

	// get the forks ready!
	temp = dining_table->first;
	mutexes = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * PHILO);
	for (int i = 0; i < PHILO; i++)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		temp->fork_mutex = mutexes[i];
		temp = temp->next;
	}
	dining_table->m = mutexes;

	// start the philo threads:
	temp = dining_table->first;
	th = (pthread_t *)malloc(sizeof(pthread_t) * PHILO);
	for (int i = 0; i < PHILO; i++)
	{
		// pthread_create(&(temp->life), NULL, &life_cycle, NULL)
		pthread_create(&th[i], NULL, &life_cycle, (void **)temp);
		temp = temp->next;
	}
	temp = dining_table->first;
	for (int i = 0; i < PHILO; i++)
	{
		// pthread_create(&(temp->life), NULL, &life_cycle, NULL)
		pthread_join(th[i], NULL);
		temp = temp->next;
	}

	for (int i = 0; i < PHILO; i++)
	{
		pthread_mutex_destroy(&mutexes[i]);
	}
	return (0);
}
