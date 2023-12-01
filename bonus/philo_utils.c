/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:46:47 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/01 14:50:28 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function to return the current time of day in milliseconds
 *
 * @param	time_stone		Mutex to ensure no other threads alter local variables
 * @return	long			Return the current time of day in milliseconds
 */
long	time_stamp(void)
{
	long			t_now;
	struct timeval	t;

	gettimeofday(&t, NULL);
	t_now = t.tv_sec * 1000 + t.tv_usec / 1000;
	return (t_now);
}

/**
 * @brief 					Function to check if a philsopher is alive/dead
 *
 * @param	p				Pointer to philosopher
 * @param	id				ID of the philospher to check the death status of
 * @return	int				Status code of philospher (-1 for dead and 0 for alive)
 */

int	check_death(t_philo *p)
{
	long	t_now;

	// pthread_mutex_lock(&p->soul);
	sem_wait(p->soul);
	t_now = time_stamp();
	if (t_now - p->last_meal > p->dna.time_to_die)
	{
		p->alive = 0;
		p->dead = t_now - p->birth;
		sem_post(p->soul);
		// pthread_mutex_unlock(&p->soul);
		kill_simulation(p);
		return (-1);
	}
	if ((p->alive) == 0 || (p->plates > p->dna.meals && p->dna.meals != -1))
	{
		sem_post(p->soul);
		// pthread_mutex_unlock(&p->soul);
		return (-1);
	}
	sem_post(p->soul);
	// pthread_mutex_unlock(&p->soul);
	return (0);
}

/**
 * @brief					Function to check the status of the simulation when called
 *
 * @param	p				Pointer to philospher calling the function
 * @return	int				Status of the simulation (-1 for END and 0 for ONGOING)
 */
int	check_sim_status(t_philo *p)
{
	// pthread_mutex_lock(p->time_stone);
	sem_wait(p->time_stone_sem);
	if (*(p->sim_status) == 0)
	{
		sem_post(p->time_stone_sem);
		// pthread_mutex_unlock(p->time_stone);
		return (-1);
	}
	sem_post(p->time_stone_sem);
	// pthread_mutex_unlock(p->time_stone);
	return (0);
}

/**
 * @brief					Function to print log entries of action sundertaken by philos
 *
 * @param p					Pointer to philospher printing the event log
 * @param action			The action performed by the philosopher (eating, sleeping,
 * 							thinking)
 */
void	console_log(t_philo *p, char *action)
{
	// pthread_mutex_lock(p->time_stone);
	sem_wait(p->time_stone_sem);
	if (*(p->sim_status) == 0)
	{
		sem_post(p->time_stone_sem);
		// pthread_mutex_unlock(p->time_stone);
		return ;
	}
	printf("%ld \t%d %s", time_stamp() - p->birth, p->id + 1, action);
	sem_post(p->time_stone_sem);
	// pthread_mutex_unlock(p->time_stone);
}

/**
 * @brief					Update fork status array to ebsure there are no greedy philosphers
 *
 * @param p					Pointer to array of philosophers
 * @param id				ID of philosopher who just ate
 * @param n					Total number of philosphers in the simulation
 */
void	update_fork_stat(t_philo **p, int id, int n)
{
	// pthread_mutex_lock(&*p[id]->time_stone);
	sem_wait(&*p[id]->time_stone_sem);
	p[id]->hungry[id] = id;
	p[id]->hungry[(id + 1) % n] = id;
	sem_post(&*p[id]->time_stone_sem);
	// pthread_mutex_unlock(&*p[id]->time_stone);
}
