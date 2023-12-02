/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 19:03:07 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/02 17:36:26 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <pthread.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>

typedef struct s_dna
{
	int	gene_pool;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meals;
}	t_dna;

typedef struct s_philo
{
	int				alive;
	int				id;
	pid_t			pid;
	pthread_t		life;
	sem_t			*soul;
	sem_t			*forks;
	sem_t			*time_stone_sem;
	sem_t			*sim_status_sem;
	sem_t			*test_sem;
	t_dna			dna;
	long			last_meal;
	long			birth;
	long			dead;
	int				plates;
	int				done;
}	t_philo;

typedef struct s_table
{
	t_philo			**philos;
	sem_t			*time_stone_sem;
	sem_t			*sim_status_sem;
	sem_t			*test_sem;
	sem_t			*forks;

}	t_table;

// parse.c
char	*ft_strjoin(char const *s1, char const *s2);
int		skip_beg(char *str);
int		ft_atoi(char *str);
int		parse_args(int argc, char **argv, t_dna *d);

//parser_utils.c:
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);

// initialize.c
t_philo	**prep_philos(t_dna *dna);
t_table	*prepare_table(t_dna *dna);
void	initialize_semaphores(t_table *table, t_dna *dna);
t_table	*initialize_simulation(t_dna *dna);
char	*get_sem_name(char *sem_name, int id);

// philo_utils.c
long	time_stamp(void);
int		check_death(t_philo *p);
int		check_sim_status(t_philo *p);
void	console_log(t_philo *p, char *action);

// cleanup.c
int		ft_strlen_(const char *str);
void	kill_simulation(t_philo *p);
void	destroy_semaphores(t_table *table, int n);
void	clean_table(t_table *table, int n);
void	log_philo_death(t_philo **philos, int n);

// main.c
void	call_waiter(t_table *table, t_dna dna);
void	*ft_check_death(void *arg);
void	eat(t_philo *p);
void	p_sleep(t_philo *p);
void	*life_cycle(t_philo *arg);

#endif