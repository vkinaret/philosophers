/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 17:25:06 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/23 17:36:29 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_all(char *msg, t_monitor *m, int exit_code)
{
	int	i;

	i = -1;
	if (msg)
		printf("%s\n", msg);
	pthread_mutex_destroy(&m->write_lock);
	pthread_mutex_destroy(&m->death_lock);
	if (m->forks)
	{
		i = -1;
		while (++i < m->num_of_philos)
			pthread_mutex_destroy(&m->forks[i]);
		free(m->forks);
	}
	if (m->philos)
	{
		i = -1;
		while (++i < m->num_of_philos)
			pthread_mutex_destroy(&m->philos[i].meal_lock);
		free(m->philos);
	}
	return (exit_code);
}

int	get_current_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(1, "Failed to get time.\n", 21);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int milliseconds)
{
	int	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	if (n == 0)
		return (0);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[i] == str2[i] && str1[i] != '\0' && i < (n - 1))
		i++;
	return (str1[i] - str2[i]);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	nbr;
	int	sign;

	i = 0;
	nbr = 0;
	sign = 1;
	if (str[i] == 45)
		sign = -1;
	if (str[i] == 43 || str[i] == 45)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10;
		nbr = nbr + (str[i] - '0');
		i++;
	}
	return (nbr * sign);
}
