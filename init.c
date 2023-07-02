/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:00:09 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/02 20:26:14 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mutex_init(t_params *p)
{
	int i;

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
	p->eat_count = 0;
	if (argc == 6)
		p->eat_count = ft_atoi(args[5]);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->num_philo);
	if (!p->forks)
		return (1);
	p->die = 0;
	p->print = 0;
	mutex_init(p);
	pthread_mutex_init(&(p->print_mtx), NULL);
	pthread_mutex_init(&(p->die_mtx), NULL);
	return (0);
}

t_philo	*philo_init(t_params *p, int i)
{
	t_philo *philo;
	
	philo = malloc(sizeof(t_philo) * p->num_philo);
	if (!philo)
		return (NULL);
	while (++i < p->num_philo)
	{
		philo[i].status = 1;
		philo[i].id = i;
		philo[i].left = &(p->forks[i]);
		philo[i].right = &(p->forks[0]);
		if (i != p->num_philo - 1)
			philo[i].right = &(p->forks[i + 1]);
		philo[i].ate = 0;
		philo[i].last_eat = 0;
		philo[i].p = p;
		pthread_mutex_init(&(philo[i].last_eat_mtx), NULL);
		pthread_mutex_init(&(philo[i].status_mtx), NULL);
		pthread_mutex_init(&(philo[i].ate_mtx), NULL);
	}
	return (philo);
}