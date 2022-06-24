/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 18:21:56 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/24 19:02:52 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum status
{
	DEAD = 0,
	HUNGRY = 1,
	ATE = 2,
	THINKING = 3,
	SLEEPING = 4,
}	mood;

typedef struct s_dna
{
	int gene_pool;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals;
}		t_dna;

typedef struct	s_fork
{
	int				fork;
	pthread_mutex_t	fork_mutex;
}		t_fork;

typedef struct s_philo
{
	int 			alive;
	int				id;
	pthread_t		life;
	pthread_mutex_t	soul;
	t_dna			dna;
	long			last_meal;
	enum	status	mood;
	struct s_philo	*prev;
	struct s_philo	*next;
} t_philo;