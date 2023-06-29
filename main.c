#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//typedef struct s_philo
//{
//	int status;
//};

typedef struct s_params
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_count;
	pthread_mutex_t	*forks;
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
	mutex_init(p);
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;
	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	params_init(&params, argv, argc);
	return (0);
}