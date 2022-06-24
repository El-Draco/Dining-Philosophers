/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 13:09:24 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/22 16:01:49 by rriyas           ###   ########.fr       */
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
	int				id;
	pthread_t		life;
	pthread_mutex_t	soul;
	t_dna			dna;
	long			last_meal;
	enum	status	mood;
	struct s_philo	*prev;
	struct s_philo	*next;

	// void	*life_cycle();
} t_philo;

typedef struct s_queue
{
	struct s_philo	*philos;
	t_fork			*forks;
	int				size;
	struct s_philo	*first;
	struct s_philo	*last;
} t_queue;

struct s_queue *newQueue(int size);
int get_size(struct s_queue *q);
int is_empty(struct s_queue *q);
struct s_philo *first(struct s_queue *q);
struct s_philo *last(struct s_queue *q);
void enqueue(struct s_queue *q, struct s_philo *p);
void dequeue(struct s_queue *q);


int parse_args(int argc, char **argv, t_dna *d);
long	ft_atoi(char *str, int *flag);
