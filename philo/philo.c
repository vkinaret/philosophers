/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:43:34 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/21 18:02:36 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	create_threads(t_philo *p, t_data data, int i)
{
	pthread_mutex_lock(&data.thread_lock);
	while (++i < data.num_of_philos)
	{
		if (pthread_create(&p[i].thread, NULL, routine, &p[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(p[i].thread, NULL);
			destroy_all("Error creating thread.", data, 0);
			return ;
		}
		data.threads_running++;
	}
	pthread_mutex_unlock(&data.thread_lock);
}

static void	init_philos(t_philo *p, t_data data, int i)
{
	while (++i < data.num_of_philos)
	{
		p[i].id = i + 1;
		p[i].data = &data;
		p[i].meals_eaten = 0;
		p[i].last_meal = get_current_time();
		p[i].start_time = get_current_time();
		p[i].thread = 0;
		p[i].r_fork = &data.forks[i];
		p[i].l_fork = NULL;
		if (i < data.num_of_philos - 1)
			p[i].l_fork = &data.forks[i + 1];
		else if (i == data.num_of_philos - 1)
			p[i].l_fork = &data.forks[0];
		p[i].meal_lock = &data.meal_locks[i];
	}
}

static void	init_data(t_data *data, int argc, char **argv, int i)
{
	data->dead_flag = 0;
	data->meal_count = 0;
	data->threads_running = 0;
	data->num_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_of_times_to_eat = ft_atoi(argv[5]);
	else
		data->num_of_times_to_eat = 0;
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->thread_lock, NULL);
	while (++i < data->num_of_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->meal_locks[i], NULL);
	}
}

static int	parsing(int argc, char **argv, int i, int j)
{
	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments.\n"));
	while (argv[++i])
	{
		j = 0;
		if (argv[i][j] == '+')
			argv[i]++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (printf("Invalid character.\n"));
			if (i == 1 && j > 3)
				return (printf("Too many philosophers.\n"));
			j++;
		}
		if (i == 1 && j == 3 && ft_atoi(argv[i]) > MAX_PHILO)
			return (printf("Too many philosophers.\n"));
		if (j == 10 && ft_strncmp(argv[i], "2147483647", 11) > 0)
			return (printf("Value must be between 1 and INT_MAX.\n"));
		if (j > 10 || ft_atoi(argv[i]) <= 0)
			return (printf("Value must be between 1 and INT_MAX.\n"));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int		i;
	t_data	data;
	t_philo	p[MAX_PHILO];

	if (parsing(argc, argv, 0, 0))
		return (1);
	init_data(&data, argc, argv, -1);
	init_philos(p, data, -1);
	create_threads(p, data, -1);
	monitor(data, p, -1);
	wait_threads(data);
	i = data.num_of_philos;
	while (--i >= 0)
		pthread_join(p[i].thread, NULL);
	destroy_all(NULL, data, -1);
	return (0);
}
