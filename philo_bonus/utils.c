/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:24:55 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/07 20:51:31 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(t_params	*p)
{
	struct timeval			tv;
	static unsigned long	start;

	gettimeofday(&tv, NULL);
	sem_wait(p->time_sem);
	if (!start)
		start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	sem_post(p->time_sem);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
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
	sem_wait(philo->p->die_sem);
	if (philo->p->die == 1)
	{
		sem_post(philo->p->die_sem);
		return (1);
	}
	sem_post(philo->p->die_sem);
	return (0);
}
