/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:48:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 18:13:46 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function to destroy all the mutexes in the program
 *
 * @param table				Struct containing all required simulation data
 * @param n					Number of philosphers in the simulation
 */
void	destroy_semaphores(t_table *table, int n)
{
	int	i;

	i = 0;
	while (i < n)
		kill(table->philos[i++]->pid, SIGKILL);
	usleep(500);
	i = 0;
	while (i < n)
	{
		sem_close(table->philos[i]->soul);
		i++;
	}
	sem_close(table->forks);
	sem_close(table->time_stone_sem);
	sem_close(table->sim_status_sem);
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
	destroy_semaphores(table, n);
	log_philo_death(table->philos, n);
	while (i < n)
	{
		free(table->philos[i]);
		i++;
	}
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
		if (philos[i]->alive == 0 || philos[i]->dead != 0)
		{
			printf("%lu \t%d has died\n", philos[i]->dead, i + 1);
			break ;
		}
		i++;
	}
}

void	call_waiter(t_table *table, t_dna dna)
{
	int	i;

	i = 0;
	if (dna.meals == -1)
		sem_wait(table->sim_status_sem);
	else
	{
		while (i < dna.gene_pool)
		{
			sem_wait(table->sim_status_sem);
			i++;
		}
	}
}
