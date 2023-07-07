/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:00:09 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/07 21:17:25 by almeliky         ###   ########.fr       */
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
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while ((*str >= '0' && *str <= '9') && *str)
	{
		result += *str - '0';
		result *= 10;
		str++;
	}
	result /= 10;
	return (result * sign);
}

int	params_init(t_params *p, char **args, int argc)
{
	p->num_philo = ft_atoi(args[1]);
	p->time_die = ft_atoi(args[2]);
	p->time_eat = ft_atoi(args[3]);
	p->time_sleep = ft_atoi(args[4]);
	p->eat_max = -1;
	if (argc == 6)
		p->eat_max = ft_atoi(args[5]);
	p->die = 0;
	p->forks = sem_open("forks1", O_CREAT, p->num_philo, p->num_philo);
	if (p->forks == SEM_FAILED)
	{
		printf("Error: semaphore initialization failed\n");
		return (1);
	}
	p->die_sem = sem_open("die_sem1", O_CREAT, 1, 1);
	if (p->die_sem == SEM_FAILED)
	{
		printf("Error: semaphore initialization failed\n");
		return (1);
	}
	p->time_sem = sem_open("time_sem3", O_CREAT, 1, 1);
	if (p->time_sem == SEM_FAILED)
	{
		printf("Error: semaphore initialization failed\n");
		return (1);
	}
	return (0);
}

t_philo	*philo_init(t_params *p, int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->ate = 0;
	philo->last_eat = 0;
	philo->p = p;
	philo->last_eat_sem = sem_open("last_eat", O_CREAT, 1, 1);
	if (philo->last_eat_sem == SEM_FAILED)
		return (NULL);
	philo->ate_sem = sem_open("ate_sem", O_CREAT, 1, 1);
	if (philo->ate_sem == SEM_FAILED)
		return (NULL);
	return (philo);
}

int	params_valid(char **args, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (args[i][j])
		{
			if (args[i][j] > '9' || args[i][j] < '0')
				return (1);
			j++;
		}
		i++;
		j = 0;
	}
	return (0);
}
