/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:24:55 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 16:01:30 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_b.h"

unsigned long	get_time(void)
{
	struct timeval			tv;
	static unsigned long	start;

	gettimeofday(&tv, NULL);
	if (!start)
		start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
}

void	ft_usleep(unsigned long ms)
{
	unsigned long	start;

	start = get_time();
	usleep(950 * ms);
	while ((get_time() - start) < ms)
		usleep(10);
}

int	die_return(t_philo *philo)
{
	sem_wait(philo->p->die_sem);
	if (philo->p->die == 1)
	{
		sem_post(philo->p->forks);
		sem_post(philo->p->forks);
		sem_post(philo->p->die_sem);
		return (1);
	}
	sem_post(philo->p->die_sem);
	return (0);
}

void	clean_exit(t_params *p)
{
	sem_close(p->die_sem);
	sem_close(p->forks);
	sem_close(p->time_sem);
	sem_close(p->last_eat_sem);
	sem_close(p->ate_sem);
	sem_close(p->print_sem);
	sem_unlink("forks");
	sem_unlink("die_sem");
	sem_unlink("time_sem");
	sem_unlink("last_eat");
	sem_unlink("ate_sem");
	sem_unlink("print_sem");
	exit(0);
}

void	ft_parent_or_child(t_params *p, int i, int status)
{
	if (i < p->num_philo)
	{
		get_time();
		p->philo = philo_init(p, i);
		p_process(p, p->philo);
	}
	if (i >= p->num_philo)
	{
		while (status == 256 && --i >= 0)
			waitpid(-1, &status, 0);
		i = -1;
		while (++i < p->num_philo)
			kill(p->pids[i], SIGKILL);
	}
}
