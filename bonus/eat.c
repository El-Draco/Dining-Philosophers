/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:51:02 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/30 21:56:09 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function to check if both left ahnd right forks are available
 *
 * @param	f				Pointer to array of forks
 * @param	id				ID of philosopher trying to pick up forks
 * @param	n				Number of philosphers in the simulation
 * @return	int				Status of attempt to pick up 2 forks (1 for SUCCESS and 0
 * 							for FAILED)
 */
int	check_left_and_right_forks(t_fork **f, int id, int n)
{
	int	first;
	int	second;

	first = id;
	second = (id + 1) % n;
	if (id == n - 1)
	{
		second = first;
		first = (id + 1) % n;
	}
	sem_wait(&f[first]->fork_mutex);
	sem_wait(&f[second]->fork_mutex);
	// pthread_mutex_lock(&f[first]->fork_mutex);
	// pthread_mutex_lock(&(f[second]->fork_mutex));
	if (f[first]->fork == 1 && f[second]->fork == 1)
	{
		lock_forks(f, id, n);
		sem_post(&f[second]->fork_mutex);
		sem_post(&f[first]->fork_mutex);
		// pthread_mutex_unlock(&(f[second]->fork_mutex));
		// pthread_mutex_unlock(&f[first]->fork_mutex);
		return (1);
	}
	sem_post(&f[second]->fork_mutex);
	sem_post(&f[first]->fork_mutex);
	// pthread_mutex_unlock(&f[second]->fork_mutex);
	// pthread_mutex_unlock(&f[first]->fork_mutex);
	return (0);
}

/**
 * @brief 					Function to determine the optimum lock order and lock the required
 * 							forks
 *
 * @param f					Pointer to the array of forks
 * @param id				ID of the philosopher who's going to eat
 * @param n					Number of philosophers in the simulation
 */
void	lock_forks(t_fork **f, int id, int n)
{
	int	first;
	int	second;

	first = id;
	second = (id + 1) % n;
	f[first]->fork = 0;
	f[second]->fork = 0;
}

/**
 * @brief 					Function to unlock the required forks in the right order
 *
 * @param f					Pointer to the array of forks
 * @param id				ID of the philosopher who finished eating
 * @param n					Number of philosophers in the simulation
 */
void	unlock_forks(t_fork **f, int id, int n)
{
	int	first;
	int	second;

	first = id;
	second = (id + 1) % n;
	// pthread_mutex_lock(&f[first]->fork_mutex);
	sem_wait(&f[first]->fork_mutex);
	f[first]->fork = 1;
	sem_post(&f[first]->fork_mutex);
	// pthread_mutex_unlock(&f[first]->fork_mutex);
	// pthread_mutex_lock(&f[second]->fork_mutex);
	sem_wait(&f[second]->fork_mutex);
	f[second]->fork = 1;
	sem_post(&f[second]->fork_mutex);
	// pthread_mutex_unlock(&f[second]->fork_mutex);
}

/**
 * @brief 					Function that waits until forks are available for the philosopher
 * 							to eat
 *
 * @param p					Pointer to array of philosophers
 * @param f					Pointer to array of forks
 * @param id				ID of the philospher that is picking up forks
 * @param dna				Common shared data for all the philosphers
 * @param int				Status code after waiting for forks (1 for SUCCESS, 0 for DEATH)
 */
int	pick_up_forks(t_philo **p, t_fork **f, int id, t_dna dna)
{
	while (2)
	{
		if (check_sim_status(p[id]) == -1)
			return (-1);
		else if (check_left_and_right_forks(f, id, dna.gene_pool) == 1
			&& check_death(p[id]) != -1)
		{
			console_log(p[id], "has picked up a fork\n");
			console_log(p[id], "has picked up a fork\n");
			return (1);
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
 * @return int				Status code of the attemt to pick up 2 forks (-1 for 'DEATH',
 * 							1 for 'SUCCESS', and '0' for 'GREEDY')
 */
int	try_to_eat(t_philo **p, t_fork **f, int id, t_dna dna)
{
	if (dna.gene_pool == 1)
		return (0);
	// pthread_mutex_lock(&*p[id]->time_stone);
	sem_wait(&*p[id]->time_stone_sem);
	if (p[id]->hungry[(id + 1) % dna.gene_pool] != id
		&& p[id]->hungry[id] != id)
	{
		sem_post(&*p[id]->time_stone_sem);
		// pthread_mutex_unlock(&*p[id]->time_stone);
		// pthread_mutex_lock(&p[id]->soul);
		sem_wait(&*p[id]->time_stone_sem);
		if (p[id]->plates >= p[id]->dna.meals && p[id]->dna.meals != -1)
		{
			sem_post(&*p[id]->time_stone_sem);
			// pthread_mutex_unlock(&p[id]->soul);
			return (0);
		}
		sem_post(&*p[id]->time_stone_sem);
		// pthread_mutex_unlock(&p[id]->soul);
		if (pick_up_forks(p, f, id, dna) == -1)
			return (-1);
		eat(p, id);
		update_fork_stat(p, id, dna.gene_pool);
		unlock_forks(f, id, dna.gene_pool);
		return (1);
	}
	sem_post(&*p[id]->time_stone_sem);
	// pthread_mutex_unlock(&*p[id]->time_stone);
	return (0);
}
