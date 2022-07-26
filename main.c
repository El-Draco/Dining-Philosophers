/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/26 12:54:52 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
// #include "ft_printf.h"

// timestamp
long time_stamp()
{
	long t_now;
	struct timeval t;

	gettimeofday(&t, NULL);
	t_now = t.tv_sec * 1000 + t.tv_usec/1000;
	return t_now;
}

void kiraman_katibin(long time_of_action, int phil, char *action)
{
	printf("%lu %d %s", time_of_action, phil, action);
}

void call_waiter(void *arg, int argc)
{
	int i;
	int status;
	int all_ate;

	all_ate = 0;
	t_table *table = (t_table*)(arg);
	t_philo **p;
	status = 1;
	p = (table->philos);
	while (2)
	{
		i = 0;
		all_ate = 1;
		while (i < p[0]->dna.gene_pool)
		{
			pthread_mutex_lock(&table->pager_mutex);
			// pthread_mutex_lock(&p[i]->soul);
			if (p[i]->alive == 0)
			{
				status = 0;
				kiraman_katibin(p[i]->dead - p[i]->alive, i, "has died\n");
				// pthread_mutex_unlock(&p[i]->soul);
				exit(0);
			}
			if (p[i]->plates < p[i]->dna.meals)
				all_ate &= 0;
			// pthread_mutex_unlock(&p[i]->soul);
			if (status == 0)
				return ;
			i++;
			pthread_mutex_unlock(&table->pager_mutex);
		}
		if(argc == 6 && all_ate == 1)
			exit(0);
	}
}



int pick_left_fork(t_philo **p, int id, t_fork **f)
{
	int status;

	status = 1;
	pthread_mutex_lock(&f[id]->fork_mutex);
	// pthread_mutex_lock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
	if (f[id]->fork == 0)
		status = 0;
	else
		f[id]->fork = 1;
	pthread_mutex_unlock(&f[id]->fork_mutex);
	// pthread_mutex_unlock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
	return status;
}

// https://www.taskade.com/d/Wo2GH3Ram15Ni6H5?share=edit&edit=2xseuMb6FTFLbsNL

int pick_right_fork(t_philo **p, int id, t_fork **f)
{
	int status;

	status = 1;
	// pthread_mutex_lock(&f[id]->fork_mutex);
	pthread_mutex_lock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
	if (f[(id + 1) % p[id]->dna.gene_pool]->fork == 0)
		status= 0;
	else
		f[(id + 1) % p[id]->dna.gene_pool]->fork = 1;
	// pthread_mutex_unlock(&f[id]->fork_mutex);
	pthread_mutex_unlock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
	return status;
}



int think(t_philo **p, t_fork **f, int id);

int eat(t_philo **p, t_fork **f, int id)
{
	int first;
	int second;

	first = id;
	second = (id + 1) % p[id]->dna.gene_pool;
	if (id % 2 == 1)
	{
		first = (id + 1) % p[id]->dna.gene_pool;
		second = id;
	}
	p[id]->mood = ATE;
	long time_start;
	struct timeval t;
	pthread_mutex_lock(&p[id] ->soul);
	pthread_mutex_lock(&f[first]->fork_mutex);
	f[id]->fork = 0;
	pthread_mutex_lock(&f[second]->fork_mutex);
	f[(id + 1) % p[id]->dna.gene_pool]->fork = 0;
	// gettimeofday(&t, NULL);

	long timer = 0;
	time_start = time_stamp();
	p[id]->last_meal = time_start;
	pthread_mutex_lock(&(*p[id]->pager));
	kiraman_katibin(time_start- p[id]->birth, id, "is eating\n");
	pthread_mutex_unlock(&*p[id]->pager);
	// usleep(p[id]->dna.time_to_die);
	while (timer < p[id]->dna.time_to_eat)
	{
		usleep(50);
		timer  = time_stamp() - time_start;
		if (timer + time_start - p[id]->last_meal >= p[id]->dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = timer + time_start - p[id]->birth;
			break;
		}
	}
	pthread_mutex_lock(&*p[id]->pager);
	kiraman_katibin(time_stamp()- p[id]->birth, id, "finished eating\n");
	p[id]->plates++;
	pthread_mutex_unlock(&*p[id]->pager);
	f[id]->fork = 1;
	pthread_mutex_unlock(&f[first]->fork_mutex);
	f[(id + 1) % p[id]->dna.gene_pool]->fork = 1;
	pthread_mutex_unlock(&f[second]->fork_mutex);
	pthread_mutex_unlock(&p[id]->soul);
	return (0);
}

int try_to_eat(t_philo **p, t_fork **f, int id)
{
	int status;
	int n;

	n = p[id]->dna.gene_pool;
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
		think(p, f,id);
	else
	{
		while (p[id]->hungry[id] == id && p[id]->hungry[(id + 1) % n] == id)
			usleep(50);
		p[id]->hungry[id] = id;
		p[id]->hungry[(id + 1) % n] = id;
		eat(p, f, id);
	}
	return (1);
}

int think(t_philo **p, t_fork **f, int id)
{
	long thought_for = 0;
	long t_start;
	struct timeval t;
	int f1;
	int f2;
	t_start= time_stamp();
	if (id % 2 == 0)
	{
		f1 = id;
		f2 = (id + 1) % p[id]->dna.gene_pool;
	}
	else
	{
		f1 = (id + 1 ) % p[id]->dna.gene_pool;
		f2 = id;
	}
	pthread_mutex_lock(&p[id]->soul);
	while (2)
	{
		pthread_mutex_lock(&f[id]->fork_mutex);
		pthread_mutex_lock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
		if (f[f1]->fork == 1 && f[f2]->fork == 1)
		{
			pthread_mutex_unlock(&f[id]->fork_mutex);
			pthread_mutex_unlock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
			eat(p,f,id);
			break;
		}
		pthread_mutex_unlock(&f[id]->fork_mutex);
		pthread_mutex_unlock(&f[(id + 1) % p[id]->dna.gene_pool]->fork_mutex);
		usleep(50);
		thought_for = time_stamp() - t_start;
		if (thought_for +  p[id]->last_meal - t_start >= p[id]->dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = t_start + thought_for - p[id]->birth;
			break;
		}
	}
	pthread_mutex_unlock(&p[id]->soul);
	return (0);
}

int p_sleep(t_philo **p, int id)
{
	long slept_for;
	long t_start;
	long time_to_sleep;

	slept_for = 0;

	pthread_mutex_lock(&p[id]->soul);
	t_start = time_stamp();
	pthread_mutex_lock(&*p[id]->pager);
	kiraman_katibin(t_start -  p[id]->birth, id, "is sleeping\n\0");
	pthread_mutex_unlock(&*p[id]->pager);
	while (slept_for < p[id]->dna.time_to_sleep)
	{
		usleep(50);
		slept_for = time_stamp() - t_start;
		if (slept_for + t_start - p[id]->last_meal >= p[id]->dna.time_to_die)
		{
			p[id]->alive = 0;
			p[id]->dead = t_start + slept_for - p[id]->birth;
			break;
		}
	}
	// kiraman_katibin(time_stamp() -  p[id]->birth, id, "finished sleeping\n\0");
	p[id]->mood = HUNGRY;
	pthread_mutex_unlock(&p[id]->soul);
	return (0);
}

void *life_cycle(void *arg)
{
	t_philo *p;

	p = ((t_philo*)arg);
	while (2)
	{
		if (p->dna.gene_pool == 1)
		{
			p->alive = 0;
			p->dead = time_stamp() - p->birth;
			break ;
		}
		try_to_eat(p->others, p->forks, p->id);
		p_sleep(p->others, p->id);
		pthread_mutex_lock(&*p->pager);
		kiraman_katibin(time_stamp() - p->birth, p->id, "is thinking\n");
		pthread_mutex_unlock(&*p->pager);
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
	philos = malloc(sizeof(t_philo*) * dna.gene_pool);
	int i;
	for (i = 0; i < dna.gene_pool; i++)
	{
		temp = (t_philo*)malloc(sizeof(t_philo));
		temp->id = i;
		temp->dna = dna;
		temp->mood = HUNGRY;
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
	time_now = t.tv_sec * 1000 + t.tv_usec/1000;
	int h[210] = {-1};
	// h = malloc(sizeof(int) * philos[0]->dna.gene_pool);
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
	pthread_mutex_init(&(table_1.pager_mutex),NULL);
	for (int i = 0; i < dna.gene_pool; i++)
	{
		pthread_mutex_init(&(forks[i]->fork_mutex), NULL);
		pthread_mutex_init(&(philos[i]->soul), NULL);
		philos[i]->pager = &table_1.pager_mutex;
	}
	table_1.philos = philos;
	table_1.forks = forks;

	for (int i = 0; i < dna.gene_pool; i++)
		pthread_create(&(philos[i]->life), NULL, &life_cycle, (void **)(&*(philos[i])));
	call_waiter(&table_1, argc);
	// while (2)
	// {
	// 	i = 0;
	// 	while (i < dna.gene_pool)
	// 	{
	// 		pthread_mutex_lock(&*philos[i]->pager);
	// 		// pthread_mutex_lock(&philos[i]->soul);
	// 		if (philos[i]->alive == 0)
	// 		{
	// 			kiraman_katibin(philos[i]->dead, i, "has died\n");
	// 			pthread_mutex_unlock(&*philos[i]->pager);

	// 			// pthread_mutex_unlock(&philos[i]->soul);
	// 			exit(0);
	// 		}
	// 		// pthread_mutex_unlock(&philos[i]->soul);
	// 		pthread_mutex_unlock(&*philos[i]->pager);

	// 		i++;
	// 	}
	// }
	for (int i = 0; i < dna.gene_pool; i++)
		pthread_join(philos[i]->life, NULL);
	for (int i = 0; i < dna.gene_pool; i++)
	{
		pthread_mutex_destroy(&(forks[i]->fork_mutex));
		pthread_mutex_destroy(&(philos[i]->soul));
	}
	pthread_mutex_destroy(&table_1.pager_mutex);
	// clean_table(&table_1);
	return (0);
}
