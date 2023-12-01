/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:48:21 by rriyas            #+#    #+#             */
/*   Updated: 2022/09/23 19:06:56 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function to end simulation when the need arises
 *
 * @param	p				Pointer to philospher calling the function
 */
void	kill_simulation(t_philo *p)
{
	pthread_mutex_lock(p->time_stone);
	*(p->sim_status) = 0;
	pthread_mutex_unlock(p->time_stone);
}

/**
 * @brief					Function to destroy all the mutexes in the program
 *
 * @param table				Struct containing all required simulation data
 * @param n					Number of philosphers in the simulation
 */
void	destroy_mutexes(t_table *table, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&(table->forks[i]->fork_mutex));
		pthread_mutex_destroy(&(table->philos[i]->soul));
		i++;
	}
	pthread_mutex_destroy(&(table->time_stone_mutex));
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
	destroy_mutexes(table, n);
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
