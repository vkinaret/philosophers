/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:22:48 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/23 19:03:20 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct timeval	t_timeval;
typedef pthread_mutex_t	t_mtx;

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	int			last_meal;
	int			meals_eaten;
	int			*num_of_philos;
	int			*time_to_eat;
	int			*time_to_sleep;
	t_mtx		*r_fork;
	t_mtx		*l_fork;
	t_mtx		meal_lock;
	t_mtx		*death_lock;
	t_mtx		*write_lock;
	int			*start_time;
	int			*status;
}				t_philo;

typedef struct s_monitor
{
	int			status;
	int			start_time;
	int			num_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_times_to_eat;
	t_mtx		write_lock;
	t_mtx		death_lock;
	t_mtx		*forks;
	t_philo		*philos;
}				t_monitor;

//		ROUTINE.C
void	*routine(void *ptr);
void	print_action(t_philo *p, char *action);

//		MONITOR.C
void	monitor(t_monitor *m);

//		UTILS.C
int		destroy_all(char *msg, t_monitor *m, int exit_code);
int		get_current_time(void);
void	ft_usleep(int milliseconds);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *str);

#endif
