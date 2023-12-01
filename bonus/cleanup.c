/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:48:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/01 14:57:15 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int ft_strlen(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

/**
 * @brief					Function to end simulation when the need arises
 *
 * @param	p				Pointer to philospher calling the function
 */
void	kill_simulation(t_philo *p)
{
	// pthread_mutex_lock(p->time_stone);
	sem_wait(p->time_stone_sem);
	*(p->sim_status) = 0;
	sem_post(p->time_stone_sem);
	// pthread_mutex_unlock(p->time_stone);
}

/**
 * @brief					Function to destroy all the mutexes in the program
 *
 * @param table				Struct containing all required simulation data
 * @param n					Number of philosphers in the simulation
 */
void	destroy_semaphores(t_table *table, int n)
{
	int	i;

	(void)table;
	i = 0;
	while (i < n)
	{
		// sem_unlink((table->forks[i]->fork_sem));
		sem_unlink(get_sem_name("/fork_", i));
		// sem_unlink((table->philos[i]->soul));
		sem_unlink(get_sem_name("/philo_", i));

		i++;
	}
	// sem_unlink(&(table->time_stone_mutex));
	sem_unlink(get_sem_name("/time_stone", i));
}

/**
 * @brief					Function to deallocate memory in the program and destroy
 * 							mutexes before exiting the program
 *
 * @param table				Struct containing all required simulation data
 * @param n					Number of philosphers in the simulation
 */
void	clean_table(t_table *table, int n)
{
	int	i;

	i = 0;
	log_philo_death(table->philos, n);
	destroy_semaphores(table, n);
	while (i < n)
	{
		free(table->forks[i]);
		free(table->philos[i]);
		i++;
	}
	free(table->time_stone_status);
	free(table->forks);
	free(table->hungry);
	free(table->philos);
	free(table);
}

/**
 * @brief					Function to log the death of the philospher (if any)
 *
 * @param philos			Pointer to array of philosophers
 * @param n					Number of philosphers in the simulation
 */
void	log_philo_death(t_philo **philos, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (philos[i]->alive == 0)
		{
			printf("%lu \t%d has died\n", philos[i]->dead, i + 1);
			break ;
		}
		i++;
	}
}
