/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 21:32:04 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/12 21:32:04 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_b.h"

void	ft_eating(t_philo *philo)
{
	sem_wait(philo->p->print_sem);
	printf("%lu %d is eating\n", get_time(), philo->id + 1);
	sem_post(philo->p->print_sem);
	sem_wait(philo->p->last_eat_sem);
	philo->last_eat = get_time();
	sem_post(philo->p->last_eat_sem);
	ft_usleep(philo->p->time_eat);
	sem_post(philo->p->forks);
	sem_post(philo->p->forks);
	sem_wait(philo->p->ate_sem);
	philo->ate++;
	if (philo->p->eat_max != 0 && philo->ate == philo->p->eat_max)
	{
		sem_post(philo->p->print_sem);
		sem_post(philo->p->die_sem);
		sem_post(philo->p->last_eat_sem);
		sem_post(philo->p->ate_sem);
		exit(1);
	}
	sem_post(philo->p->ate_sem);
}

void	fork_pick(t_philo *philo)
{
	sem_wait(philo->p->forks);
	sem_wait(philo->p->print_sem);
	printf("%lu %d has taken a fork\n", get_time(), philo->id + 1);
	sem_post(philo->p->print_sem);
	sem_wait(philo->p->forks);
	sem_wait(philo->p->print_sem);
	printf("%lu %d has taken a fork\n", get_time(), philo->id + 1);
	sem_post(philo->p->print_sem);
}

void	*p_thread(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (die_return(philo) == 1)
			exit (0);
		fork_pick(philo);
		ft_eating(philo);
		if (die_return(philo) == 1)
			exit (0);
		sem_wait(philo->p->print_sem);
		printf("%lu %d is sleeping\n", get_time(), philo->id + 1);
		sem_post(philo->p->print_sem);
		ft_usleep(philo->p->time_sleep);
		if (die_return(philo) == 1)
			exit (0);
		sem_wait(philo->p->print_sem);
		printf("%lu %d is thinking\n", get_time(), philo->id + 1);
		sem_post(philo->p->print_sem);
		if (die_return(philo) == 1)
			exit (0);
	}
	return (0);
}

void	*die_check(void *args)
{
	t_params	*p;

	p = (t_params *)args;
	while (1)
	{
		sem_wait(p->last_eat_sem);
		sem_wait(p->die_sem);
		if (get_time() - p->philo->last_eat > (unsigned long)p->time_die)
		{
			sem_wait(p->print_sem);
			printf("%lu %d died\n", get_time(), p->philo->id + 1);
			p->die = 1;
			exit (0);
		}
		sem_post(p->die_sem);
		sem_post(p->last_eat_sem);
	}
	return (0);
}

void	p_process(t_params *p, t_philo *philo)
{
	pthread_t	checker;
	int			status_addr;

	pthread_create(&checker, NULL, die_check, p);
	p_thread(philo);
	pthread_join(checker, (void **)&status_addr);
}
