/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinaret <vkinaret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:43:34 by vkinaret          #+#    #+#             */
/*   Updated: 2024/06/23 19:26:37 by vkinaret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_simulation(t_monitor *m, t_philo *p)
{
	int	i;

	i = -1;
	while (++i < m->num_of_philos)
	{
		if (pthread_create(&p[i].thread, NULL, routine, &p[i]) != 0)
		{
			m->status = 1;
			while (--i >= 0)
				pthread_join(p[i].thread, NULL);
			return (destroy_all("Error creating thread.", m, 1));
		}
	}
	monitor(m);
	i = -1;
	while (++i < m->num_of_philos)
		pthread_join(p[i].thread, NULL);
	return (0);
}

static int	init_philos(t_philo *p, t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->num_of_philos)
	{
		p[i].id = i + 1;
		p[i].thread = 0;
		p[i].last_meal = get_current_time();
		p[i].meals_eaten = 0;
		p[i].num_of_philos = &m->num_of_philos;
		p[i].time_to_eat = &m->time_to_eat;
		p[i].time_to_sleep = &m->time_to_sleep;
		p[i].r_fork = &m->forks[i];
		p[i].l_fork = NULL;
		if (i < m->num_of_philos - 1)
			p[i].l_fork = &m->forks[i + 1];
		else if (i > 0 && i == m->num_of_philos - 1)
			p[i].l_fork = &m->forks[0];
		pthread_mutex_init(&p[i].meal_lock, NULL);
		p[i].write_lock = &m->write_lock;
		p[i].death_lock = &m->death_lock;
		p[i].start_time = &m->start_time;
		p[i].status = &m->status;
	}
	return (0);
}

static int	init_monitor(t_monitor *m, int argc, char **argv)
{
	int	i;

	i = -1;
	m->status = 0;
	m->start_time = get_current_time();
	m->num_of_philos = ft_atoi(argv[1]);
	m->time_to_die = ft_atoi(argv[2]);
	m->time_to_eat = ft_atoi(argv[3]);
	m->time_to_sleep = ft_atoi(argv[4]);
	m->num_of_times_to_eat = 0;
	if (argc == 6)
		m->num_of_times_to_eat = ft_atoi(argv[5]);
	pthread_mutex_init(&m->write_lock, NULL);
	pthread_mutex_init(&m->death_lock, NULL);
	m->forks = malloc(m->num_of_philos * sizeof(t_mtx));
	if (!m->forks)
		return (destroy_all("Failed to malloc forks.", m, 1));
	while (++i < m->num_of_philos)
		pthread_mutex_init(&m->forks[i], NULL);
	return (0);
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
		if (i == 1 && j == 3 && ft_atoi(argv[i]) > 200)
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
	t_monitor	m;
	t_philo		*p;

	if (parsing(argc, argv, 0, 0))
		return (1);
	p = malloc(ft_atoi(argv[1]) * sizeof(t_philo));
	if (!p)
		return (printf("Failed to malloc philos.\n"));
	m.philos = p;
	if (init_monitor(&m, argc, argv) || init_philos(p, &m))
		return (1);
	if (start_simulation(&m, p))
		return (1);
	return (destroy_all(NULL, &m, 0));
}
