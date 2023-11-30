/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/30 21:33:46 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function for the main thread to check for any deaths in the sim
 *
 * @param	table			Struct with access to all philosphers
 * @param	dna				Common shared data for all the philosphers
 */
void	call_waiter(t_table *table, t_dna dna)
{
	t_philo	**p;
	int		ate;
	int		i;

	p = table->philos;
	ate = 0;
	while (check_sim_status(p[0]) != -1 && ate != 1)
	{
		i = 0;
		ate = 1;
		usleep(100);
		while (check_sim_status(p[0]) != -1 && i < dna.gene_pool)
		{
			// pthread_mutex_lock(&p[i]->soul);
			sem_wait(&p[i]->soul);
			if (p[i]->plates < dna.meals || dna.meals == -1)
				ate &= 0;
			sem_post(&p[i]->soul);
			// pthread_mutex_unlock(&p[i]->soul);
			if (check_death(p[i]) == -1)
				kill_simulation(p[i]);
			i++;
		}
		if (ate == 1 && dna.meals != -1)
			kill_simulation(p[0]);
	}
}

/**
 * @brief 					Function to let the philospher eat and reset time of last meal
 *
 * @param p					Pointer to array of philosophers
 * @param id				ID of the philosopher about to eat
 */
void	eat(t_philo **p, int id)
{
	long	time_start;
	long	timer;

	timer = 0;
	time_start = time_stamp();
	// pthread_mutex_lock(&p[id]->soul)
	sem_wait(&p[id]->soul);
	p[id]->last_meal = time_start;
	sem_post(&p[id]->soul);
	// pthread_mutex_unlock(&p[id]->soul);
	if (check_death(p[id]) == -1 || check_sim_status(p[id]) == -1)
		return ;
	console_log(p[id], "is eating\n");
	while (timer < p[id]->dna.time_to_eat && check_sim_status(p[id]) != -1)
	{
		timer = time_stamp() - time_start;
		if (check_death(p[id]) == -1)
		{
			unlock_forks(p[id]->forks, id, p[id]->dna.gene_pool);
			kill_simulation(p[id]);
			break ;
		}
		usleep(100);
	}
	// pthread_mutex_lock(&p[id]->soul);
	sem_wait(&p[id]->soul);
	p[id]->plates++;
	sem_post(&p[id]->soul);
	// pthread_mutex_unlock(&p[id]->soul);
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
void	p_sleep(t_philo **p, int id, t_dna dna)
{
	long	slept_for;
	long	t_start;

	t_start = time_stamp();
	slept_for = 0;
	if (check_death(p[id]) == -1)
		return ;
	console_log(p[id], "is sleeping\n");
	while (slept_for < dna.time_to_sleep && check_sim_status(p[id]) != -1)
	{
		slept_for = time_stamp() - t_start;
		if (check_death(p[id]) == -1)
		{
			kill_simulation(p[id]);
			return ;
		}
		usleep(100);
	}
}

/**
 * @brief					The thread routine for each philosopher (eat - sleep - think)
 *
 * @param	arg				Data peratining to a particular philospher (thread)
 * @return	void*			Return value (always NULL)
 */
void	*life_cycle(void *arg)
{
	t_philo	*p;
	int		status;

	p = ((t_philo *)arg);
	while (check_death(p) != -1 && check_sim_status(p) != -1)
	{
		status = 0;
		while (status != 1 && check_death(p) != -1 && check_sim_status(p) != -1)
		{
			usleep(50);
			status = try_to_eat(p->others, p->forks, p->id, p->dna);
			if (status == -1)
			{
				kill_simulation(p);
				return (NULL);
			}
		}
		if (check_death(p) == -1 || check_sim_status(p) == -1)
			break ;
		p_sleep(&*(p->others), p->id, p->dna);
		console_log(p, "is thinking\n");
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	int		i;
	t_dna	dna;
	t_table	*table;
	t_philo	**philos;

	if (parse_args(argc, argv, &dna) == -1)
	{
		return (0);
	}
	table = initialize_simulation(&dna);
	philos = table->philos;
	i = -1;
	while (++i < dna.gene_pool)
	{
		pthread_create(&(philos[i]->life), NULL, &life_cycle,
			(void **)(&*(philos[i])));
		usleep(50);
	}
	call_waiter(table, dna);
	i = -1;
	while (++i < dna.gene_pool)
		pthread_join(philos[i]->life, NULL);
	clean_table(table, dna.gene_pool);
	return (0);
}
