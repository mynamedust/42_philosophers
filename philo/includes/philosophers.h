/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:27:27 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 15:43:22 by almeliky         ###   ########.fr       */
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
# include <limits.h>

//Types
typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_max;
	int				eat_count;
	int				die;
	struct s_philo	*philo;
	pthread_t		die_thr;
	pthread_mutex_t	die_mtx;
	pthread_mutex_t	eat_mtx;
	pthread_mutex_t	time_mtx;
	pthread_mutex_t	*forks;
}	t_params;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				ate;
	int				last_eat;
	pthread_t		thread;
	t_params		*p;
	pthread_mutex_t	last_eat_mtx;
	pthread_mutex_t	ate_mtx;
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

//Philo
void			*die_check(void *args);
void			*p_thread(void *args);
int				fork_take(t_philo *philo);
void			ft_eating(t_philo *philo);

//Main
int				err(char *str);
int				ft_strcmp(char *s1, char *s2);

#endif