/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 13:09:51 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/21 17:48:05 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_all(char *msg, t_data data, int i)
{
	if (msg)
		printf("%s\n", msg);
	pthread_mutex_destroy(&data.write_lock);
	pthread_mutex_destroy(&data.thread_lock);
	pthread_mutex_destroy(&data.death_lock);
	while (++i < data.num_of_philos)
	{
		pthread_mutex_destroy(&data.meal_locks[i]);
		pthread_mutex_destroy(&data.forks[i]);
	}
}

static int	is_dead(t_data data, t_philo *p, int i)
{
	if (get_current_time() - p[i].last_meal > data.time_to_die)
	{
		pthread_mutex_unlock(p[i].meal_lock);
		pthread_mutex_lock(&p->data->death_lock);
		data.dead_flag = 1;
		pthread_mutex_unlock(&p->data->death_lock);
		pthread_mutex_lock(&p->data->write_lock);
		printf("%d\t", get_current_time() - p->start_time);
		printf("%d %s\n", p->id, "died");
		pthread_mutex_unlock(&p->data->write_lock);
		return (1);
	}
	return (0);
}

static int	ate_all(t_data data, t_philo *p, int i)
{
	if (!data.num_of_times_to_eat)
		return (0);
	if (data.meal_count == data.num_of_philos)
	{
		pthread_mutex_unlock(p[i].meal_lock);
		pthread_mutex_lock(&p->data->death_lock);
		data.dead_flag = 1;
		pthread_mutex_unlock(&p->data->death_lock);
		return (1);
	}
	return (0);
}

int	monitor(t_data data, t_philo *p, int i)
{
	ft_usleep(1);
	while (1)
	{
		i = -1;
		while (++i < data.num_of_philos)
		{
			pthread_mutex_lock(p[i].meal_lock);
			if (is_dead(data, p, i) == 1)
				return (1);
			if (ate_all(data, p, i) == 1)
				return (1);
			pthread_mutex_unlock(p[i].meal_lock);
		}
	}
	return (0);
}

void wait_threads(t_data data)
{
	pthread_mutex_lock(&data.thread_lock);
	while (data.threads_running)
	{
		pthread_mutex_unlock(&data.thread_lock);
		ft_usleep(100);
		pthread_mutex_lock(&data.thread_lock);
	}
	pthread_mutex_unlock(&data.thread_lock);
}
