/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:00:09 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 15:49:59 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int			sign;
	long int	result;

	sign = 1;
	result = 0;
	if (!*str)
		return (0);
	while (*str == 9 || *str == 10 || *str == 11
		|| *str == 12 || *str == 13 || *str == 0 || *str == ' ')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while ((*str >= '0' && *str <= '9') && *str)
	{
		result *= 10;
		result += *str - '0';
		str++;
		if ((result * sign) > INT_MAX || (result * sign) < INT_MIN)
			return (err("Error. Write valid arguments.\n"));
	}
	return (result * sign);
}

void	mutex_init(t_params *p)
{
	int	i;

	i = 0;
	while (i < p->num_philo)
	{
		pthread_mutex_init(&(p->forks[i]), NULL);
		i++;
	}
}

int	params_init(t_params *p, char **args, int argc)
{
	p->num_philo = ft_atoi(args[1]);
	p->time_die = ft_atoi(args[2]);
	p->time_eat = ft_atoi(args[3]);
	p->time_sleep = ft_atoi(args[4]);
	if (p->num_philo <= 0 || p->time_die <= 0 || p->time_eat <= 0
		|| p->time_sleep <= 0)
		return (err("Error. Write valid arguments\n"));
	p->eat_max = -1;
	if (argc == 6)
		p->eat_max = ft_atoi(args[5]);
	if (p->eat_max == 0 || p->eat_max < -1)
		return (1);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->num_philo);
	if (!p->forks)
	{
		printf("Memory allocating error.\n");
		return (1);
	}
	p->die = 0;
	p->eat_count = 0;
	mutex_init(p);
	pthread_mutex_init(&(p->die_mtx), NULL);
	pthread_mutex_init(&(p->eat_mtx), NULL);
	pthread_mutex_init(&(p->time_mtx), NULL);
	return (0);
}

t_philo	*philo_init(t_params *p, int i)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * p->num_philo);
	if (!philo)
		return (NULL);
	while (++i < p->num_philo)
	{
		philo[i].id = i;
		philo[i].left = &(p->forks[i]);
		philo[i].right = &(p->forks[0]);
		if (i != p->num_philo - 1)
			philo[i].right = &(p->forks[i + 1]);
		philo[i].ate = 0;
		philo[i].last_eat = 0;
		philo[i].p = p;
		pthread_mutex_init(&(philo[i].last_eat_mtx), NULL);
		pthread_mutex_init(&(philo[i].ate_mtx), NULL);
	}
	return (philo);
}

int	params_valid(char **args, int count)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (count < 4 || count > 5)
		return (err("Error. Write valid number of arguments.\n"));
	while (++i < count)
	{
		if (args[i] && ft_strcmp(args[i], "1") && ft_atoi(args[i]) == 1)
			return (1);
		while (args[i][++j] || args[i][0] == '\0')
		{
			if ((args[i][j] > '9' || args[i][j] < '0') || args[i][0] == '\0')
				return (err("Error. Write valid arguments.\n"));
		}
		j = -1;
	}
	return (0);
}
