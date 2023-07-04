/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:03:00 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/02 21:03:51 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int params_valid(char **args, int count)
{
	int i;
	int j;

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

int		die_return(t_philo *philo)
{
	pthread_mutex_lock(&philo->p->die_mtx);
	if (philo->p->die == 1)
	{
		pthread_mutex_unlock(&philo->p->die_mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->p->die_mtx);
	return (0);
}

void	ft_usleep(unsigned long ms)
{
	unsigned long	start;

	start = get_time();
	usleep(950 * ms);
	while ((get_time() - start) < ms)
		usleep(10);
}

void	*hThread(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (die_return(philo) == 1)
			return (0);
		pthread_mutex_unlock(&philo->p->die_mtx);
		pthread_mutex_lock(fork_pick(philo, 1));
		if (die_return(philo) == 1)
		{
			pthread_mutex_unlock(fork_pick(philo, 1));
			return (0);
		}
		printf("%lums %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_lock(fork_pick(philo, 2));
		if (die_return(philo) == 1)
		{
			pthread_mutex_unlock(fork_pick(philo, 2));
			pthread_mutex_unlock(fork_pick(philo, 1));
			return (0);
		}
		printf("%lums %d has taken a fork\n", get_time(), philo->id);
		printf("%lums %d is eating\n", get_time(), philo->id);
		pthread_mutex_lock(&philo->last_eat_mtx);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->last_eat_mtx);
		ft_usleep(philo->p->time_eat);
        pthread_mutex_unlock(fork_pick(philo, 2));
        pthread_mutex_unlock(fork_pick(philo, 1));
		pthread_mutex_lock(&philo->ate_mtx);
		philo->ate++;
        pthread_mutex_lock(&philo->p->eat_mtx);
        if (philo->p->eat_max != 0 && philo->ate == philo->p->eat_max)
        {
            philo->p->eat_count += 1;
            printf("%d\n", philo->p->eat_max);
        }
        pthread_mutex_unlock(&philo->p->eat_mtx);
		pthread_mutex_unlock(&philo->ate_mtx);
		if (die_return(philo) == 1)
			return (0);
		printf("%lums %d is sleeping\n", get_time(), philo->id);
		ft_usleep(philo->p->time_sleep);
		if (die_return(philo) == 1)
			return (0);
		printf("%lums %d is thinking\n", get_time(), philo->id);
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
        if (get_time() - p->philo[i].last_eat > (unsigned long)p->time_die)
        {
            pthread_mutex_lock(&p->die_mtx);
            printf("%lums %d died; LAST EAT - %d\n ", get_time(), i, p->philo[i].last_eat);
            p->die = 1;
            pthread_mutex_unlock(&p->die_mtx);
            pthread_mutex_unlock(&p->philo[i].last_eat_mtx);
            return (0);
        }
		pthread_mutex_unlock(&p->philo[i].last_eat_mtx);
        pthread_mutex_lock(&p->eat_mtx);
        pthread_mutex_lock(&p->die_mtx);
        if (p->eat_count == p->num_philo)
        {
            p->die = 1;
            pthread_mutex_unlock(&p->die_mtx);
            pthread_mutex_unlock(&p->eat_mtx);
            return (0);
        }
        pthread_mutex_unlock(&p->die_mtx);
        pthread_mutex_unlock(&p->eat_mtx);
        i++;
        if (i == p->num_philo)
            i = 0;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	p;
	int			i;
	int 		status_addr;

	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	params_init(&p, argv, argc);
	p.philo = philo_init(&p, -1);
	i = 0;
	while (i < p.num_philo)
	{
		pthread_create(&(p.philo[i].thread), NULL, hThread, &p.philo[i]);
		i++;
    }
	get_time();
	pthread_create(&(p.die_thr), NULL, die_check, &p);
	pthread_join(p.die_thr, (void**) &status_addr);
	i = 0;
	while (i < p.num_philo)
	{
		pthread_join(p.philo[i].thread, (void**) &status_addr);
		i++;
    }
	return (0);
}