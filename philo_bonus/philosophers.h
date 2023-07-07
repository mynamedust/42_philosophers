/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:27:27 by almeliky          #+#    #+#             */
/*   Updated: 2023/07/07 20:52:04 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

//includes
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>

//Types
typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_max;
	int				die;
	sem_t			*die_sem;
	sem_t			*forks;
	sem_t			*time_sem;
	struct s_philo	*philo;
}	t_params;

typedef struct s_philo
{
	int				id;
	int				ate;
	int				last_eat;
	sem_t			*last_eat_sem;
	sem_t			*ate_sem;
	t_params		*p;
}	t_philo;

//Init
int				ft_atoi(const char *str);
int				params_valid(char **args, int count);
int				params_init(t_params *p, char **args, int argc);
void			mutex_init(t_params *p);
t_philo			*philo_init(t_params *p, int i);

//Utils
pthread_mutex_t	*fork_pick(t_philo *philo, int order);
unsigned long	get_time(t_params	*p);
int				count_check(t_params *p);
void			ft_usleep(t_params *p, unsigned long ms);
int				die_return(t_philo *philo);

//Main
void			*die_check(void *args);
void			*p_thread(void *args);
int				fork_take(t_philo *philo);
void			ft_eating(t_philo *philo);

#endif