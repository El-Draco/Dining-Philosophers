/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:58:22 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 18:19:41 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	gave_up_on_life(t_philo *philo)
{
	sem_wait(philo->soul);
	if (philo->done == 1 || philo->dead != 0)
	{
		sem_post(philo->soul);
		return (1);
	}
	sem_post(philo->soul);
	return (0);
}

/**
 * @brief					The thread routine for each philosopher (eat - sleep - think)
 *
 * @param	arg				Data peratining to a particular philospher (thread)
 * @return	void*			Return value (always NULL)
 */
void	*life_cycle(t_philo *philo)
{
	pthread_t	death;

	pthread_create(&death, NULL, ft_check_death, philo);
	pthread_detach(death);
	while (2)
	{
		if (gave_up_on_life(philo))
			return (NULL);
		eat(philo);
		if (gave_up_on_life(philo))
			return (NULL);
		p_sleep(philo);
		console_log(philo, "is thinking\n");
	}
	return (NULL);
}

void	create_philos(t_table *table, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		table->philos[i]->pid = fork();
		if (table->philos[i]->pid == 0)
			life_cycle(table->philos[i]);
		usleep(90);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_dna	dna;
	t_table	*table;

	if (parse_args(argc, argv, &dna) == -1)
		return (0);
	table = initialize_simulation(&dna);
	create_philos(table, dna.gene_pool);
	call_waiter(table, dna);
	clean_table(table, dna.gene_pool);
	return (0);
}
