/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:46:47 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 13:44:35 by rriyas           ###   ########.fr       */
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
