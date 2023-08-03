/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeliky <almeliky@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:27:27 by almeliky          #+#    #+#             */
/*   Updated: 2023/08/02 16:01:00 by almeliky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_B_H
# define PHILOSOPHERS_B_H

//Includes
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <limits.h>

//Types
typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_max;
	int				die;
	pid_t			*pids;
	sem_t			*die_sem;
	sem_t			*forks;
	sem_t			*time_sem;
	sem_t			*last_eat_sem;
	sem_t			*ate_sem;
	sem_t			*print_sem;
	struct s_philo	*philo;
}	t_params;

typedef struct s_philo
{
	int				id;
	int				ate;
	int				last_eat;
	t_params		*p;
}	t_philo;

//Init
int				params_valid(char **args, int count);
t_philo			*philo_init(t_params *p, int id);
int				params_init(t_params *p, char **args, int argc);
void			ft_sem_init(t_params *p);
int				ft_atoi(const char *str);

//Utils
unsigned long	get_time(void);
void			ft_usleep(unsigned long ms);
int				die_return(t_philo *philo);
void			clean_exit(t_params *p);
void			ft_parent_or_child(t_params *p, int i, int status);

//Philo
void			p_process(t_params *p, t_philo *philo);
void			*die_check(void *args);
void			*p_thread(void *args);
void			fork_pick(t_philo *philo);
void			ft_eating(t_philo *philo);

//Main
int				err(char *str);
int				ft_strcmp(char *s1, char *s2);

#endif