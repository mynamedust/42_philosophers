/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:24:55 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/04 20:11:27 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	*fork_pick(t_philo *philo, int order)
{
	if (order == 1)
	{
		if (philo->id % 2 == 0)
			return (philo->right);
		return (philo->left);
	}
	if (philo->id % 2 == 0)
		return (philo->left);
	return (philo->right);
}

unsigned long	get_time(t_params	*p)
{
	struct timeval			tv;
	static unsigned long	start;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&p->time_mtx);
	if (!start)
		start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	pthread_mutex_unlock(&p->time_mtx);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
}

int	count_check(t_params *p)
{
	pthread_mutex_lock(&p->eat_mtx);
	if (p->eat_count == p->num_philo)
	{
		pthread_mutex_lock(&p->die_mtx);
		p->die = 1;
		pthread_mutex_unlock(&p->die_mtx);
		pthread_mutex_unlock(&p->eat_mtx);
		return (0);
	}
	pthread_mutex_unlock(&p->eat_mtx);
	return (1);
}

void	ft_usleep(t_params *p, unsigned long ms)
{
	unsigned long	start;

	start = get_time(p);
	usleep(950 * ms);
	while ((get_time(p) - start) < ms)
		usleep(10);
}

int	die_return(t_philo *philo)
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
