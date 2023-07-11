/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:03:13 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/11 16:30:58 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_take(t_philo *philo)
{
	pthread_mutex_lock(fork_pick(philo, 1));
	if (die_return(philo) == 1)
		return (1);
	printf("%lu %d has taken a fork\n", get_time(philo->p), philo->id + 1);
	if (philo->p->num_philo <= 1)
	{
		ft_usleep(philo->p, philo->p->time_die + 10);
		if (die_return(philo) == 1)
			return (1);
	}
	pthread_mutex_lock(fork_pick(philo, 2));
	if (die_return(philo) == 1)
		return (1);
	printf("%lu %d has taken a fork\n", get_time(philo->p), philo->id + 1);
	return (0);
}

void	ft_eating(t_philo *philo)
{
	printf("%lu %d is eating\n", get_time(philo->p), philo->id + 1);
	pthread_mutex_lock(&philo->last_eat_mtx);
	philo->last_eat = get_time(philo->p);
	pthread_mutex_unlock(&philo->last_eat_mtx);
	ft_usleep(philo->p, philo->p->time_eat);
	pthread_mutex_unlock(fork_pick(philo, 2));
	pthread_mutex_unlock(fork_pick(philo, 1));
	pthread_mutex_lock(&philo->ate_mtx);
	philo->ate++;
	pthread_mutex_lock(&philo->p->eat_mtx);
	if (philo->p->eat_max != 0 && philo->ate == philo->p->eat_max)
		philo->p->eat_count += 1;
	pthread_mutex_unlock(&philo->p->eat_mtx);
	pthread_mutex_unlock(&philo->ate_mtx);
}

void	*p_thread(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (die_return(philo) == 1)
		{
			pthread_mutex_unlock(fork_pick(philo, 2));
			pthread_mutex_unlock(fork_pick(philo, 1));
			return (0);
		}
		if (fork_take(philo) == 1)
			return (0);
		ft_eating(philo);
		if (die_return(philo) == 1)
			return (0);
		printf("%lu %d is sleeping\n", get_time(philo->p), philo->id + 1);
		ft_usleep(philo->p, philo->p->time_sleep);
		if (die_return(philo) == 1)
			return (0);
		printf("%lu %d is thinking\n", get_time(philo->p), philo->id + 1);
		if (die_return(philo) == 1)
			return (0);
	}
	return (0);
}

void	*die_check(void *args)
{
	t_params	*p;
	int			i;

	i = 0;
	p = (t_params *)args;
	while (i < p->num_philo)
	{
		pthread_mutex_lock(&p->philo[i].last_eat_mtx);
		pthread_mutex_lock(&p->die_mtx);
		if (get_time(p) - p->philo[i].last_eat > (unsigned long)p->time_die)
		{
			printf("%lu %d died\n", get_time(p), i + 1);
			p->die = 1;
			pthread_mutex_unlock(&p->die_mtx);
			pthread_mutex_unlock(&p->philo[i].last_eat_mtx);
			return (0);
		}
		pthread_mutex_unlock(&p->die_mtx);
		pthread_mutex_unlock(&p->philo[i].last_eat_mtx);
		if (count_check(p) == 0)
			return (0);
		i = 0;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	p;
	int			i;
	int			status_addr;

	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	params_init(&p, argv, argc);
	p.philo = philo_init(&p, -1);
	i = 0;
	get_time(&p);
	while (i < p.num_philo)
	{
		pthread_create(&(p.philo[i].thread), NULL, p_thread, &p.philo[i]);
		i++;
	}
	pthread_create(&(p.die_thr), NULL, die_check, &p);
	pthread_join(p.die_thr, (void **)&status_addr);
	i = 0;
	while (i < p.num_philo)
	{
		pthread_join(p.philo[i].thread, (void **)&status_addr);
		i++;
	}
	return (0);
}
