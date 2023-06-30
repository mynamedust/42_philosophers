#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				ate;
	int				last_eat;
	int 			status;
	pthread_t		thread;
	pthread_mutex_t last_eat_mtx;
	pthread_mutex_t status_mtx;
	pthread_mutex_t ate_mtx;
}	t_philo;


typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_count;
	int				die;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}	t_params;

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

int params_valid(char **args, int count)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (args[i][j])
		{
			if (args[i][j] > '9' || args[i][j] < '0')
				return (1);
			j++;
		}
		i++;
		j = 0;
	}
	return (0);
}

void	mutex_init(t_params *p)
{
	int i;

	i = 0;
	while (i < p->num_philo)
	{
		pthread_mutex_init(&(p->forks[i]), NULL);
		i++;
	}
}

int	params_init(t_params *p, char **args, int argc)
{
	p->num_philo = ft_atoi(args[1]);
	p->time_die = ft_atoi(args[2]);
	p->time_eat = ft_atoi(args[3]);
	p->time_sleep = ft_atoi(args[4]);
	p->eat_count = 0;
	if (argc == 6)
		p->eat_count = ft_atoi(args[5]);
	p->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * p->num_philo);
	if (!p->forks)
		return (1);
	p->die = 0;
	mutex_init(p);
	return (0);
}

pthread_mutex_t	*fork_pick(t_philo *philo, int order)
{
	if (order == 1)
	{
		if (philo->id % 2 == 0)
			return philo->right;
		return philo->right;
	}
	if (philo->id % 2 == 0)
		return philo->left;
	return philo->right;
}

void	*hThread(void *args) {
	t_philo	*philo;
	int		i = 0;

	philo = (t_philo *)args;
	pthread_mutex_lock(fork_pick(philo, 1));
	pthread_mutex_lock(fork_pick(philo, 2));
	printf("%d %d is eating", get_time(), philo->id);
	usleep(200000);
	pthread_mutex_unlock(fork_pick(philo, 2));
	pthread_mutex_unlock(fork_pick(philo, 1));
	printf("%d %d is sleeping", get_time(), philo->id);
	usleep(200000);
    return 0;
}

int	get_time()
{
	struct timeval			tv;
	static unsigned long	start;
	static int				flag;

	gettimeofday(&tv, NULL);
	if (!flag)
	{
		start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		flag = 1;
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
}

t_philo	*philo_init(t_params *p, int i)
{
	t_philo *philo;
	
	philo = malloc(sizeof(t_philo) * p->num_philo);
	if (!philo)
		return (NULL);
	while (++i < p->num_philo)
	{
		philo[i].status = 1;
		philo[i].id = i + 1;
		philo[i].left = &(p->forks[p->num_philo]);
		if (i != 1)
			philo[i].left = &(p->forks[i - 1]);
		philo[i].right = &(p->forks[0]);
		if (i != p->num_philo)
			philo[i].right = &(p->forks[i + 1]);
		philo[i].ate = 0;
		philo[i].last_eat = 0;
		pthread_mutex_init(&(philo[i].last_eat_mtx), NULL);
		pthread_mutex_init(&(philo[i].status_mtx), NULL);
		pthread_mutex_init(&(philo[i].ate_mtx), NULL);
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	pthread_t	thread;
	t_params	p;
	int			status;
	int			i;
	int status_addr;

	get_time();
	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	params_init(&p, argv, argc);
	p.philo = philo_init(&p, -1);
	i = 0;
	while (i < p.num_philo)
	{
		status = pthread_create(&(p.philo[i].thread), NULL, hThread, &p.philo[i]);
    	if (status != 0) 
		{	
        	printf("main error: can't create thread, status = %d\n", status);
        	exit(1);
		}
		i++;
    }
	i = 0;
	while (i < p.num_philo)
	{
		status = pthread_join(p.philo[i].thread, (void**)&status_addr);
		i++;
    }
	printf("here\n");
	return (0);
}