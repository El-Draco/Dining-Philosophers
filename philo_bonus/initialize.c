/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:44:31 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/01 23:05:11 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

/**
 * @brief					Function to initalizie data for each philosopher in the simulation
 *
 * @param	dna				Common simulation data for all philosphers
 * @param	forks			Pointer to array of forks
 * @param	hungry			Array to ensure there are no greedy philosophers
 * @param	sim_status		Variable indicating the status of the simulation
 * 							(-1 for END and 0 for ONGOING)
 * @return	t_philo**		Pointer to array of philosophers
 */
t_philo	**prep_philos(t_dna *dna)
{
	t_philo	**philos;
	long	time_now;
	int		i;

	i = -1;
	time_now = time_stamp();
	philos = malloc(sizeof(t_philo *) * dna->gene_pool);
	while (++i < dna->gene_pool)
	{
		philos[i] = (t_philo *)malloc(sizeof(t_philo));
		philos[i]->id = i;
		philos[i]->dna = *dna;
		philos[i]->last_meal = time_now;
		philos[i]->birth = time_now;
		philos[i]->alive = 1;
		philos[i]->plates = 0;
		philos[i]->birth = time_now;
		philos[i]->dead = 0;
		philos[i]->done = 0;
	}
	return (philos);
}

char	*get_sem_name(char *sem_name, int id)
{
	char *ret;
	char *num;

	num = ft_itoa(id);
	ret = ft_strjoin(sem_name, num);
	free(num);
	return (ret);
}

/**
 * @brief					Function to initialize all mutexes in the program
 *
 * @param table				Struct containing all required simulation data
 * @param dna				Common simulation data for all philosphers
 */
void	initialize_semaphores(t_table *table, t_dna *dna)
{
	int	i;

	i = -1;
	char *temp;
	sem_unlink("time_stone_sem");
	sem_unlink("sim_status_sem");
	sem_unlink("forks");
	table->time_stone_sem = sem_open("time_stone_sem", O_CREAT, 0600, 1);
	table->sim_status_sem = sem_open("sim_status_sem", O_CREAT, 0600, 0);
	table->forks = sem_open("forks", O_CREAT, 0600, dna->gene_pool);
	while (++i < dna->gene_pool)
	{
		temp = get_sem_name("philo_", i);
		sem_unlink(temp);
		table->philos[i]->soul = sem_open(temp, O_CREAT, 0600, 1);
		free(temp);
		table->philos[i]->time_stone_sem = table->time_stone_sem;
		table->philos[i]->sim_status_sem = table->sim_status_sem;
		table->philos[i]->forks = table->forks;

	}
}

/**
 * @brief					Function to call all subroutines that initialize the sim data
 *
 * @param	dna				Common simulation data for all philosphers
 * @return	t_table*		Pointer to struct containing all required simulation data
 */
t_table	*initialize_simulation(t_dna *dna)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	table->philos = prep_philos(dna);
	initialize_semaphores(table, dna);
	return (table);
}
