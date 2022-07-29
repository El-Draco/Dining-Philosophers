/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 18:21:56 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/29 16:22:29 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
	int				*hungry;
	int 			alive;
	struct s_philo	**others;
	t_fork			**forks;
	int				id;
	pthread_t		life;
	pthread_mutex_t	soul;
	pthread_mutex_t *time_stone;
	t_dna			dna;
	long			last_meal;
	long			birth;
	long			dead;
	struct s_philo	*prev;
	struct s_philo	*next;
	int				plates;
} t_philo;

typedef struct s_table
{
	t_fork			**forks;
	t_philo			**philos;
	pthread_mutex_t	time_stone_mutex;
	int				time_stone;
	int				*hungry;
}					t_table;

int parse_args(int argc, char **argv, t_dna *d);