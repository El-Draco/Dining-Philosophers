/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/01 21:11:21 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"


/**
 * @brief 					Function to let the philospher eat and reset time of last meal
 *
 * @param p					Pointer to array of philosophers
 * @param id				ID of the philosopher about to eat
 */
void	eat(t_philo *p)
{
	long	time_start;
	long	timer;

	timer = 0;
	time_start = time_stamp();
	sem_wait(p->soul);
	p->last_meal = time_start;
	sem_post(p->soul);
	console_log(p, "is eating\n");
	while (timer < p->dna.time_to_eat)
	{
		timer = time_stamp() - time_start;
		usleep(100);
	}
	sem_wait(p->soul);
	p->plates++;
	sem_post(p->soul);
}

/**
 * @brief					Function to let the philospher sleep
 *
 * @param p					Pointer to array of philosphers
 * @param id				ID of the philospher who's going to sleep
 * @param time_stone		Mutex to prevent dataraces while getting timestamps and
 * 							printing logs
 * @param dna				Common simulation data for all philosphers
 */
void	p_sleep(t_philo *p)
{
	long	slept_for;
	long	t_start;

	t_start = time_stamp();
	slept_for = 0;
	console_log(p, "is sleeping\n");
	while (slept_for < p->dna.time_to_sleep)
	{
		slept_for = time_stamp() - t_start;
		usleep(100);
	}
}

int	get_min_interval(t_dna dna)
{
	int min;

	min = dna.time_to_eat;
	if (min < dna.time_to_sleep)
		min = dna.time_to_sleep;
	return (min);
}

void	*ft_check_death(void *arg)
{
	t_philo *philo;
	long t_now;

	philo = (t_philo *)arg;
	while (2)
	{
		sem_wait(philo->soul);
		t_now = time_stamp();
		if (t_now - philo->last_meal > philo->dna.time_to_die)
		{
			philo->alive = 0;
			philo->dead = t_now - philo->birth;
			sem_post(philo->soul);
			sem_post(philo->sim_status_sem);
			break ;
		}
		sem_post(philo->soul);
		sem_wait(philo->soul);
		if (philo->plates >= philo->dna.meals && philo->dna.meals != -1)
		{
			philo->done = 1;
			sem_post(philo->sim_status_sem);
			sem_post(philo->soul);
			break ;
		}
		sem_post(philo->soul);
	}
	return (NULL);
}

void acquire_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	console_log(philo, "has picked up a fork\n");
	sem_wait(philo->forks);
	console_log(philo, "has picked up a fork\n");
}

void release_forks(t_philo *philo)
{
	sem_post(philo->forks);
	sem_post(philo->forks);
}

int	gave_up_on_life(t_philo *philo)
{
	sem_wait(philo->soul);
	if (philo->done == 1 || philo->dead != 0)
	{
		sem_post(philo->soul);
		return (1);
	}
	sem_post(philo->soul);
	return (0);
}

/**
 * @brief					The thread routine for each philosopher (eat - sleep - think)
 *
 * @param	arg				Data peratining to a particular philospher (thread)
 * @return	void*			Return value (always NULL)
 */
void *life_cycle(t_philo *philo)
{
	pthread_t	death;

	pthread_create(&death, NULL, ft_check_death, philo);
	pthread_detach(death);
	while (2)
	{
		if (gave_up_on_life(philo))
			return (NULL);
		acquire_forks(philo);
		eat(philo);
		release_forks(philo);
		if (gave_up_on_life(philo))
			return (NULL);
		p_sleep(philo);
		console_log(philo, "is thinking\n");
	}
	return (NULL);
}


void create_philos(t_table *table, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		usleep(50);
		table->philos[i]->pid = fork();
		if (table->philos[i]->pid == 0)
		{
			life_cycle(table->philos[i]);
			exit(0);
		}
		i++;
	}
}

void	call_waiter(t_table *table,	int n)
{
	int i;

	i = 0;
	if (n == -1)
		sem_wait(table->sim_status_sem);
	else
	{
		while (i < n)
		{
			sem_wait(table->sim_status_sem);
			i++;
		}
	}
}

int	main(int argc, char **argv)
{
	t_dna	dna;
	t_table	*table;
	t_philo	**philos;

	if (parse_args(argc, argv, &dna) == -1)
		return (0);
	table = initialize_simulation(&dna);
	philos = table->philos;
	sem_wait(table->sim_status_sem);
	create_philos(table, dna.gene_pool);
	call_waiter(table, dna.gene_pool);
	clean_table(table, dna.gene_pool);
	return (0);
}