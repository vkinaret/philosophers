/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 13:09:51 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/23 19:24:20 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	meal_counter(t_monitor *m, t_philo *p)
{
	int	i;

	i = -1;
	while (++i < m->num_of_philos)
	{
		pthread_mutex_lock(&p[i].meal_lock);
		if (p[i].meals_eaten != m->num_of_times_to_eat)
		{
			pthread_mutex_unlock(&p[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&p[i].meal_lock);
	}
	pthread_mutex_lock(p->death_lock);
	*p->status = 1;
	pthread_mutex_unlock(p->death_lock);
	return (1);
}

static int	death_checker(t_monitor *m, t_philo *p)
{
	int	i;

	i = -1;
	while (++i < m->num_of_philos)
	{
		pthread_mutex_lock(&p[i].meal_lock);
		if (get_current_time() - p[i].last_meal > m->time_to_die)
		{
			pthread_mutex_unlock(&p[i].meal_lock);
			pthread_mutex_lock(p->death_lock);
			*p->status = 1;
			print_action(&p[i], "died");
			pthread_mutex_unlock(p->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&p[i].meal_lock);
	}
	return (0);
}

void	monitor(t_monitor *m)
{
	while (1)
	{
		if (death_checker(m, m->philos))
			return ;
		if (m->num_of_times_to_eat && meal_counter(m, m->philos))
			return ;
	}
}
