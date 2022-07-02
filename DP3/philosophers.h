/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 18:21:56 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/01 16:04:57 by rriyas           ###   ########.fr       */
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
	struct s_philo	**others;
	t_fork			**forks;
	int				id;
	pthread_t		life;
	pthread_mutex_t	soul;
	t_dna			dna;
	long			last_meal;
	long			birth;
	long			dead;
	enum	status	mood;
	struct s_philo	*prev;
	struct s_philo	*next;
} t_philo;

typedef struct s_table
{
	t_fork			**forks;
	t_philo			**philos;
	pthread_mutex_t	status_mutex;
	int				status;
}					t_table;

int parse_args(int argc, char **argv, t_dna *d);