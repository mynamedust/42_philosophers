/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:00:09 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/15 16:01:41 by almeliky         ###   ########.fr       */
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

void	ft_sem_init(t_params *p)
{
	sem_unlink("forks");
	sem_unlink("die_sem");
	sem_unlink("time_sem");
	sem_unlink("last_eat");
	sem_unlink("ate_sem");
	sem_unlink("print_sem");
	p->forks = sem_open("forks", O_CREAT, 0666, p->num_philo);
	p->die_sem = sem_open("die_sem", O_CREAT, 0666, 1);
	p->time_sem = sem_open("time_sem", O_CREAT, 0666, 1);
	p->last_eat_sem = sem_open("last_eat", O_CREAT, 0666, 1);
	p->ate_sem = sem_open("ate_sem", O_CREAT, 0666, 1);
	p->print_sem = sem_open("print_sem", O_CREAT, 0666, 1);
	if (p->print_sem == SEM_FAILED || p->forks == SEM_FAILED
		|| p->die_sem == SEM_FAILED || p->time_sem == SEM_FAILED
		|| p->last_eat_sem == SEM_FAILED || p->ate_sem == SEM_FAILED)
	{
		printf("Error: semaphore initialization failed, %d - errno\n", errno);
		exit (1);
	}
	return ;
}

int	params_init(t_params *p, char **args, int argc)
{
	p->num_philo = ft_atoi(args[1]);
	p->time_die = ft_atoi(args[2]);
	p->time_eat = ft_atoi(args[3]);
	p->time_sleep = ft_atoi(args[4]);
	p->pids = malloc(sizeof(pid_t) * p->num_philo);
	if (!p->pids)
		clean_exit(p);
	p->eat_max = -1;
	if (argc == 6)
	{
		p->eat_max = ft_atoi(args[5]);
		if (p->eat_max <= 0)
			exit(0);
	}
	p->die = 0;
	ft_sem_init(p);
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
