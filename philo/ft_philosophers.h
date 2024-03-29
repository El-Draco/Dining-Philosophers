/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 19:03:07 by rriyas            #+#    #+#             */
/*   Updated: 2022/10/01 19:02:43 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_dna
{
	int	gene_pool;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meals;
}	t_dna;

typedef struct s_fork
{
	int				fork;
	pthread_mutex_t	fork_mutex;
}	t_fork;

typedef struct s_philo
{
	int				*hungry;
	int				alive;
	struct s_philo	**others;
	t_fork			**forks;
	int				id;
	pthread_t		life;
	pthread_mutex_t	soul;
	pthread_mutex_t	*time_stone;
	int				*sim_status;
	t_dna			dna;
	long			last_meal;
	long			birth;
	long			dead;
	int				plates;
}	t_philo;

typedef struct s_table
{
	t_fork			**forks;
	t_philo			**philos;
	pthread_mutex_t	time_stone_mutex;
	int				*time_stone_status;
	int				*hungry;
}	t_table;

// parse.c
int		skip_beg(char *str);
int		ft_atoi(char *str);
int		parse_args(int argc, char **argv, t_dna *d);

// initialize.c
t_philo	**prep_philos(t_dna *dna, t_fork **forks, int hungry[],
			int *sim_status);
t_table	*prepare_table(t_dna *dna);
void	initialize_mutexes(t_table *table, t_dna *dna);
t_table	*initialize_simulation(t_dna *dna);

// philo_utils.c
long	time_stamp(void);
int		check_death(t_philo *p);
int		check_sim_status(t_philo *p);
void	console_log(t_philo *p, char *action);
void	update_fork_stat(t_philo **p, int id, int n);

// eat.c
int		check_left_and_right_forks(t_fork **f, int id, int n);
void	lock_forks(t_fork **f, int id, int n);
void	unlock_forks(t_fork **f, int id, int n);
int		pick_up_forks(t_philo **p, t_fork **f, int id, t_dna dna);
int		try_to_eat(t_philo **p, t_fork **f, int id, t_dna dna);

// cleanup.c
void	kill_simulation(t_philo *p);
void	destroy_mutexes(t_table *table, int n);
void	clean_table(t_table *table, int n);
void	log_philo_death(t_philo **philos, int n);

// main.c
void	call_waiter(t_table *table, t_dna dna);
void	eat(t_philo **p, int id);
void	p_sleep(t_philo **p, int id, t_dna dna);
void	*life_cycle(void *arg);

#endif