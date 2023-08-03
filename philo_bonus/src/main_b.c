/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:27:04 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 16:01:23 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_b.h"

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

	if (params_valid(argv + 1, argc - 1))
		return (1);
	if (params_init(&p, argv, argc) == 1)
		return (1);
	get_time();
	i = 0;
	while (i < p.num_philo)
	{
		p.pids[i] = fork();
		if (p.pids[i] == -1)
			exit(1);
		if (p.pids[i] == 0)
			break ;
		i++;
	}
	ft_parent_or_child(&p, i, 256);
	clean_exit(&p);
	exit (0);
}
