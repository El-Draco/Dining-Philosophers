/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 13:09:24 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/20 17:07:37 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum status
{
	ALIVE = 1,
	DEAD = 0,
	EATING = 1,
	THINKING = 2,
	SLEEPING = 3,
} t_philo_status;

typedef struct s_dna
{
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals;
}		t_dna;

typedef struct s_philo
{
	int id;
	int fork;
	t_dna	dna;
	struct s_philo *prev;
	struct s_philo *next;
	pthread_mutex_t fork_mutex;
	pthread_t life;
	// void	*life_cycle();
} t_philo;

typedef struct s_queue
{
	struct s_philo	*philos;
	pthread_mutex_t	*m;
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
