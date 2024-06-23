/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:38:47 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/21 13:37:12 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_unlock(t_mtx *mtx_one, t_mtx *mtx_two, int sleep, int exit)
{
	if (sleep)
		ft_usleep(sleep);
	if (mtx_one)
		pthread_mutex_unlock(mtx_one);
	if (mtx_two)
		pthread_mutex_unlock(mtx_two);
	return (exit);
}

void	print_action(t_philo *p, char *action)
{
	pthread_mutex_lock(&p->data->write_lock);
	if (ft_die(p))
	{
		ft_unlock(&p->data->write_lock, NULL, 0, 1);
		return ;
	}
	printf("%d\t", get_current_time() - p->start_time);
	printf("%d %s\n", p->id, action);
	pthread_mutex_unlock(&p->data->write_lock);
}

void	*routine(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	pthread_mutex_lock(&p->data->thread_lock);
	pthread_mutex_unlock(&p->data->thread_lock);
	if (p->id % 2 == 0)
		ft_usleep(1);
	while (!ft_die(p))
	{
		if (ft_eat(p) || ft_think(p) || ft_sleep(p))
			break ;
	}
	pthread_mutex_lock(&p->data->thread_lock);
	p->data->threads_running -= 1;
	pthread_mutex_unlock(&p->data->thread_lock);
	return (p);
}
