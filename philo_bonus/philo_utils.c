/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:46:47 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 18:23:51 by rriyas           ###   ########.fr       */
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
 * @brief					Function to print log entries of action sundertaken by philos
 *
 * @param p					Pointer to philospher printing the event log
 * @param action			The action performed by the philosopher (eating, sleeping,
 * 							thinking)
 */
void	console_log(t_philo *p, char *action)
{
	sem_wait(p->time_stone_sem);
	printf("%ld \t%d %s", time_stamp() - p->birth, p->id + 1, action);
	sem_post(p->time_stone_sem);
}

int	check_philo_dead(t_philo *philo)
{
	long	time_now;
	int		i;

	sem_wait(philo->soul);
	time_now = time_stamp();
	i = 0;
	if (time_now - philo->last_meal > philo->dna.time_to_die)
	{
		sem_wait(philo->time_stone_sem);
		printf("%lu \t%d has died\n", time_now - philo->birth, philo->id + 1);
		if (philo->dna.meals != -1)
		{
			while (i < philo->dna.gene_pool)
			{
				sem_post(philo->sim_status_sem);
				i++;
			}
		}
		else
			sem_post(philo->sim_status_sem);
		return (1);
	}
	sem_post(philo->soul);
	return (0);
}

void	*ft_check_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (2)
	{
		if (check_philo_dead(philo) == 1)
			break ;
		sem_wait(philo->soul);
		if (philo->plates >= philo->dna.meals && philo->dna.meals != -1)
		{
			philo->done = 1;
			sem_post(philo->sim_status_sem);
			break ;
		}
		sem_post(philo->soul);
		usleep(50);
	}
	return (NULL);
}
