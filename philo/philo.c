/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:43:34 by vkinaret          #+#    #+#             */
/*   Updated: 2024/03/21 14:43:36 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	destroy_all(char *msg, t_data data, t_philo *p, int i)
{
	while (i < data.num_of_philos)
	{
		pthread_mutex_destroy(&p[i].meal_lock);
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data.write_lock);
	pthread_mutex_destroy(&data.death_lock);
	if (msg)
		return (printf("%s\n", msg));
	return (0);
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
		p[i].thread = NULL;
		p[i].r_fork = &data.forks[i];
		p[i].l_fork = NULL;
		if (i < data.num_of_philos - 1)
			p[i].l_fork = &data.forks[i + 1];
		else if (i && i == data.num_of_philos - 1)
			p[i].l_fork = &data.forks[0];
		pthread_mutex_init(&p[i].meal_lock, NULL);
	}
}

static void	init_data(int i, char *argv, t_data *data)
{
	if (i == 0)
	{
		data->meal_count = 0;
		data->dead_flag = 0;
		data->threads_running = 0;
		pthread_mutex_init(&data->write_lock, NULL);
		pthread_mutex_init(&data->death_lock, NULL);
		pthread_mutex_init(&data->thread_lock, NULL);
		while (i < data->num_of_philos)
		{
			pthread_mutex_init(&data->forks[i], NULL);
			i++;
		}
		return ;
	}
	if (i == 1)
		data->num_of_philos = ft_atoi(argv);
	if (i == 2)
		data->time_to_die = ft_atoi(argv);
	if (i == 3)
		data->time_to_eat = ft_atoi(argv);
	if (i == 4)
		data->time_to_sleep = ft_atoi(argv);
	if (i == 5)
		data->num_of_times_to_eat = ft_atoi(argv);
}

static int	parsing(char **argv, int i, int j, t_data *data)
{
	while (argv[++i])
	{
		j = 0;
		if (argv[i][j] == '+')
			argv[i]++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (printf("Invalid character.\n"));
			j++;
		}
		if ((i == 1 && j > 3) || (i == 1 && j == 3
				&& ft_atoi(argv[i]) > MAX_PHILO))
			return (printf("Too many philosophers.\n"));
		if ((j == 10 && ft_strncmp(argv[i], "2147483647", 11) > 0)
			|| j > 10 || ft_atoi(argv[i]) <= 0)
			return (printf("Value must be between 1 and INT_MAX.\n"));
		init_data(i, argv[i], data);
	}
	init_data(0, NULL, data);
	return (0);
}

static void	wait_threads(t_data *data)
{
	pthread_mutex_lock(&data->thread_lock);
	printf("number of threads: %d\n", data->threads_running);
	while (data->threads_running)
	{
		pthread_mutex_unlock(&data->thread_lock);
		ft_usleep(500);
		printf("waiting for %d threads\n", data->threads_running);
		pthread_mutex_lock(&data->thread_lock);
	}
	pthread_mutex_unlock(&data->thread_lock);
}

int	main(int argc, char **argv)
{
	int		i;
	t_data	data;
	t_philo	p[MAX_PHILO];

	i = -1;
	if (argc != 5 && argc != 6)
		return (printf("Wrong number of arguments.\n"));
	if (parsing(argv, 0, 0, &data) != 0)
		return (1);
	if (argc == 5)
		data.num_of_times_to_eat = 0;
	init_philos(p, data, i);
	pthread_mutex_lock(&data.thread_lock);
	while (++i < data.num_of_philos)
	{
		if (pthread_create(&p[i].thread, NULL, routine, &p[i]) != 0)
		{
			printf("error encountered on philo %d\n", i);
			while (--i >= 0)
				pthread_join(p[i].thread, NULL);
			return (destroy_all("Error creating thread.", data, p, 0));
		}
		data.threads_running++;
	}
	pthread_mutex_unlock(&data.thread_lock);
	monitor(data, p, -1, data.num_of_times_to_eat);
	printf("simulation finished! i is %d\n", i);
	wait_threads(&data);
	while (--i >= 0)
	{
		printf("joining %d\n", i);
		pthread_join(p[i].thread, NULL);
		printf("joined %d\n", i);
	}
	printf("threads destroyed! i is %d\n", i);
	return (destroy_all(NULL, data, p, 0));
}
