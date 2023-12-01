/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 12:44:31 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/01 14:57:36 by rriyas           ###   ########.fr       */
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
t_philo	**prep_philos(t_dna *dna, t_fork **forks, int hungry[], int *sim_status)
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
		philos[i]->others = philos;
		philos[i]->forks = forks;
		philos[i]->last_meal = time_now;
		philos[i]->birth = time_now;
		philos[i]->alive = 1;
		philos[i]->plates = 0;
		philos[i]->hungry = hungry;
		philos[i]->sim_status = sim_status;
		philos[i]->birth = time_now;
	}
	return (philos);
}

/**
 * @brief					Function to assign the forks and philosphers in a t_table struct
 *
 * @param	dna				Common simulation data for all philosphers
 * @return	t_table*		Pointer to struct containing all required simulation data
 */
t_table	*prepare_table(t_dna *dna)
{
	int		i;
	t_table	*table;
	t_fork	**forks;

	i = -1;
	table = malloc(sizeof(t_table));
	forks = malloc(dna->gene_pool * sizeof(t_fork *));
	while (++i < dna->gene_pool)
	{
		forks[i] = malloc(dna->gene_pool * sizeof(t_fork));
		forks[i]->fork = 1;
	}
	table->forks = forks;
	return (table);
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
	// pthread_mutex_init(&(table->time_stone_mutex), NULL);
	// sem_init(&(table->time_stone_sem), 1, dna->gene_pool);
	table->time_stone_sem = sem_open("/time_stone", O_CREAT, 0644, 1);
	while (++i < dna->gene_pool)
	{
		table->forks[i]->fork_sem = sem_open(get_sem_name("/fork_", i), O_CREAT, 0644, 1);
		table->philos[i]->soul = sem_open(get_sem_name("/philo_", i), O_CREAT, 0644, 1);
		// sem_init(&(table->forks[i]->fork_sem), 1, dna->gene_pool);
		// sem_init(&(table->philos[i]->soul), 1, dna->gene_pool);
		// pthread_mutex_init(&(table->forks[i]->fork_mutex), NULL);
		// pthread_mutex_init(&(table->philos[i]->soul), NULL);
		// table->philos[i]->time_stone = &table->time_stone_mutex;
		table->philos[i]->time_stone_sem = table->time_stone_sem;
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
	int		*hungry;
	int		*sim_status;
	int		i;

	i = -1;
	sim_status = malloc(sizeof(int));
	table = prepare_table(dna);
	hungry = malloc(dna->gene_pool * (sizeof(int)));
	while (++i < dna->gene_pool)
		hungry[i] = -1;
	table->hungry = hungry;
	table->time_stone_status = sim_status;
	*(table->time_stone_status) = 1;
	table->philos = prep_philos(dna, table->forks, hungry, sim_status);
	initialize_semaphores(table, dna);
	return (table);
}
