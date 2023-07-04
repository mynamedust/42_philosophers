#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

//includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

//Types
typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_max;
    int             eat_count;
	int				die;
	struct s_philo	*philo;
	pthread_t		die_thr;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	die_mtx;
    pthread_mutex_t eat_mtx;
	pthread_mutex_t	*forks;
}	t_params;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				ate;
	int				last_eat;
	int 			status;
	pthread_t		thread;
	t_params		*p;
	pthread_mutex_t last_eat_mtx;
	pthread_mutex_t status_mtx;
	pthread_mutex_t ate_mtx;
}	t_philo;

//Init
int		params_init(t_params *p, char **args, int argc);
t_philo	*philo_init(t_params *p, int i);

//Utils
int				ft_atoi(const char *str);
pthread_mutex_t	*fork_pick(t_philo *philo, int order);
unsigned long	get_time();

int params_valid(char **args, int count);
void	*hThread(void *args);
void	*die_check(void *args);

#endif