/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:38:47 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/23 19:26:53 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *p, char *action)
{
	pthread_mutex_lock(p->write_lock);
	printf("%d\t%d %s\n", get_current_time() - *p->start_time, p->id, action);
	pthread_mutex_unlock(p->write_lock);
}

static int	ft_die(t_philo *p, t_mtx *mtx_one, t_mtx *mtx_two)
{
	pthread_mutex_lock(p->death_lock);
	if (*p->status)
	{
		pthread_mutex_unlock(p->death_lock);
		if (mtx_one)
			pthread_mutex_unlock(mtx_one);
		if (mtx_two)
			pthread_mutex_unlock(mtx_two);
		return (1);
	}
	pthread_mutex_unlock(p->death_lock);
	return (0);
}

static int	ft_think_and_sleep(t_philo *p)
{
	if (ft_die(p, NULL, NULL))
		return (1);
	print_action(p, "is thinking");
	if (ft_die(p, NULL, NULL))
		return (1);
	print_action(p, "is sleeping");
	ft_usleep(*p->time_to_sleep);
	return (0);
}

static int	ft_eat(t_philo *p)
{
	if (ft_die(p, NULL, NULL))
		return (1);
	pthread_mutex_lock(p->r_fork);
	if (ft_die(p, p->r_fork, NULL))
		return (1);
	print_action(p, "picked up a fork");
	pthread_mutex_lock(p->l_fork);
	if (ft_die(p, p->r_fork, p->l_fork))
		return (1);
	print_action(p, "picked up a fork");
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal = get_current_time();
	pthread_mutex_unlock(&p->meal_lock);
	if (ft_die(p, p->r_fork, p->l_fork))
		return (1);
	print_action(p, "is eating");
	pthread_mutex_lock(&p->meal_lock);
	p->meals_eaten++;
	pthread_mutex_unlock(&p->meal_lock);
	ft_usleep(*p->time_to_eat);
	pthread_mutex_unlock(p->r_fork);
	pthread_mutex_unlock(p->l_fork);
	return (0);
}

void	*routine(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	if (*p->num_of_philos == 1)
	{
		print_action(p, "has taken a fork");
		return (ptr);
	}
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal = get_current_time();
	pthread_mutex_unlock(&p->meal_lock);
	if (p->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(p->death_lock);
	while (!*p->status)
	{
		pthread_mutex_unlock(p->death_lock);
		if (ft_eat(p) || ft_think_and_sleep(p))
			return (ptr);
		pthread_mutex_lock(p->death_lock);
	}
	pthread_mutex_unlock(p->death_lock);
	return (ptr);
}
