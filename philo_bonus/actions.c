/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 13:46:31 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 13:58:48 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

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

void	acquire_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	console_log(philo, "has picked up a fork\n");
	sem_wait(philo->forks);
	console_log(philo, "has picked up a fork\n");
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->forks);
	sem_post(philo->forks);
}

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

	acquire_forks(p);
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
	release_forks(p);
}
