/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 13:09:51 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/14 13:09:54 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_unlock_and_lock(t_mtx *unlock, t_mtx *lock)
{
	if (unlock)
		pthread_mutex_unlock(unlock);
	if (lock)
		pthread_mutex_lock(lock);
	return (0);
}

void	print_death(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->write_lock);
	printf("%d\t", get_current_time() - philo->start_time);
	printf("%d %s\n", philo->id, action);
	pthread_mutex_unlock(&philo->data->write_lock);
}

int	monitor(t_data data, t_philo *p, int i, int flag)
{
	ft_usleep(1);
	while (1)
	{
		i = -1;
		while (++i < data.num_of_philos)
		{
			ft_unlock_and_lock(NULL, &p[i].meal_lock);
			if (get_current_time() - p[i].last_meal >= data.time_to_die)
			{
				ft_unlock_and_lock(&p[i].meal_lock, &data.death_lock);
				data.dead_flag = 1;
				ft_unlock_and_lock(&data.death_lock, NULL);
				print_death(&p[i], "died");
				return (1);
			}
			if (flag && data.meal_count == data.num_of_philos)
			{
				ft_unlock_and_lock(&p[i].meal_lock, &data.death_lock);
				data.dead_flag = 1;
				ft_unlock_and_lock(&data.death_lock, NULL);
				print_death(&p[i], "all philos ate");
				return (1);
			}
			ft_unlock_and_lock(&p[i].meal_lock, NULL);
		}
	}
	return (0);
}
