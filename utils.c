/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:24:55 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/02 21:07:01 by almeliky         ###   ########.fr       */
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

pthread_mutex_t	*fork_pick(t_philo *philo, int order)
{
	if (order == 1)
	{
		if (philo->id % 2 == 0)
			return philo->right;
		return philo->left;
	}
	if (philo->id % 2 == 0)
		return philo->left;
	return philo->right;
}

unsigned long	get_time()
{
	struct timeval			tv;
	static unsigned long	start;

	gettimeofday(&tv, NULL);
	if (!start)
	{
		start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
}