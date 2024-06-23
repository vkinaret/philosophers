/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:20:13 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/21 18:01:13 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_die(t_philo *p)
{
	pthread_mutex_lock(&p->data->death_lock);
	if (p->data->dead_flag)
	{
		pthread_mutex_unlock(&p->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&p->data->death_lock);
	return (0);
}

int	ft_sleep(t_philo *p)
{
	if (ft_die(p))
		return (1);
	print_action(p, "is sleeping");
	if (p->data->time_to_die < p->data->time_to_sleep)
	{
		ft_usleep(p->data->time_to_die);
		return (1);
	}
	ft_usleep(p->data->time_to_sleep);
	return (0);
}

int	ft_think(t_philo *p)
{
	if (ft_die(p))
		return (1);
	print_action(p, "is thinking");
	return (0);
}

int ft_pick_up_fork(t_philo *p)
{
	if (ft_die(p))
		return (1);
	pthread_mutex_lock(p->r_fork);
	print_action(p, "has taken a fork");
	if (p->l_fork == NULL)
	{
		pthread_mutex_unlock(p->r_fork);
		return (1);
	}
	pthread_mutex_lock(p->l_fork);
	print_action(p, "has taken a fork");
	if (ft_die(p))
	{
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		return (1);
	}
	return (0);
}

int	ft_eat(t_philo *p)
{
	if (ft_pick_up_fork(p))
		return (1);
	pthread_mutex_lock(p->meal_lock);
	p->last_meal = get_current_time();
	p->meals_eaten += 1;
	if (p->meals_eaten == p->data->num_of_times_to_eat)
		p->data->meal_count += 1;
	pthread_mutex_unlock(p->meal_lock);
	if (ft_die(p))
	{
		pthread_mutex_unlock(p->l_fork);
		pthread_mutex_unlock(p->r_fork);
		return (1);
	}
	print_action(p, "is eating");
	ft_usleep(p->data->time_to_eat);
	pthread_mutex_unlock(p->l_fork);
	pthread_mutex_unlock(p->r_fork);
	return (0);
}
