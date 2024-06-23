/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:22:48 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/21 17:48:22 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> //for printf
# include <string.h> 
# include <stdlib.h>
# include <unistd.h> //for 
# include <sys/time.h> //for gettimeofday
# include <pthread.h> //for pthread functions

# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

typedef struct timeval	t_timeval;
typedef pthread_mutex_t	t_mtx;

typedef struct s_data
{
	int			dead_flag;
	int			meal_count;
	int			threads_running;
	int			num_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_times_to_eat;
	t_mtx		write_lock;
	t_mtx		death_lock;
	t_mtx		thread_lock;
	t_mtx		forks[MAX_PHILO];
	t_mtx		meal_locks[MAX_PHILO];
}				t_data;

typedef struct s_philo
{
	int			id;
	t_data		*data;
	int			meals_eaten;
	int			last_meal;
	int			start_time;
	pthread_t	thread;
	t_mtx		*r_fork;
	t_mtx		*l_fork;
	t_mtx		*meal_lock;
}				t_philo;

//		ROUTINE.C
void	*routine(void *ptr);
void	print_action(t_philo *p, char *action);
int		ft_unlock(t_mtx *mtx_one, t_mtx *mtx_two, int sleep, int exit);

//		ACTION.C
int		ft_eat(t_philo *p);
int		ft_think(t_philo *p);
int		ft_sleep(t_philo *p);
int		ft_die(t_philo *p);

//		MONITOR.C
int		monitor(t_data data, t_philo *p, int i);
void	destroy_all(char *msg, t_data data, int i);
void 	wait_threads(t_data data);

//		UTILS.C
int		get_current_time(void);
void	ft_usleep(int milliseconds);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *str);

#endif
