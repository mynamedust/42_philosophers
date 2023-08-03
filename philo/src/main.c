/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:03:13 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 17:18:43 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	err(char *str)
{
	while (*str)
		write(2, str++, 1);
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	p;
	int			i;
	int			status_addr;

	if (params_valid(argv + 1, argc - 1))
		return (1);
	if (params_init(&p, argv, argc) == 1)
		return (0);
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
