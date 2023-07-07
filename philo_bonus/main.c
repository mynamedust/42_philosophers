#include "philosophers.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void	ft_eating(t_philo *philo)
{
	printf("%lu %d is eating\n", get_time(philo->p), philo->id + 1);
	sem_wait(philo->last_eat_sem);
	philo->last_eat = get_time(philo->p);
	sem_post(philo->last_eat_sem);
	ft_usleep(philo->p, philo->p->time_eat);
	sem_post(philo->p->forks);
	sem_post(philo->p->forks);
	sem_wait(philo->ate_sem);
	philo->ate++;
	if (philo->p->eat_max != 0 && philo->ate == philo->p->eat_max)
	{
		sem_wait(philo->p->die_sem);
		philo->p->die = 1;
		sem_post(philo->p->die_sem);
	}
	sem_post(philo->ate_sem);
}

void	*p_thread(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (die_return(philo) == 1)
		{
			sem_post(philo->p->forks);
			sem_post(philo->p->forks);
			return (0);
		}
		sem_wait(philo->p->forks);
		sem_wait(philo->p->forks);
		ft_eating(philo);
		if (die_return(philo) == 1)
			return (0);
		printf("%lu %d is sleeping\n", get_time(philo->p), philo->id + 1);
		ft_usleep(philo->p, philo->p->time_sleep);
		if (die_return(philo) == 1)
			return (0);
		printf("%lu %d is thinking\n", get_time(philo->p), philo->id + 1);
		if (die_return(philo) == 1)
			return (0);
	}
	return (0);
}
void	*die_check(void *args)
{
	t_params	*p;

	p = (t_params *)args;
	while (1)
	{
		sem_wait(p->philo->last_eat_sem);
		if (get_time(p) - p->philo->last_eat > (unsigned long)p->time_die)
		{
			printf("%lu %d died\n", get_time(p), p->philo->id + 1);
			sem_wait(p->die_sem);
			p->die = 1;
			sem_post(p->die_sem);
			sem_post(p->philo->last_eat_sem);
			return (0);
		}
		sem_post(p->philo->last_eat_sem);
	}
	return (0);
}

void	p_process(t_params *p, t_philo *philo)
{
	pthread_t	checker;
	pthread_t	philosoph;
	int			status_addr;

	pthread_create(&checker, NULL, p_thread, philo);
	pthread_create(&philosoph, NULL, die_check, p);
	pthread_join(checker, (void **)&status_addr);
	pthread_join(philosoph, (void **)&status_addr);
}

int main(int argc, char **argv)
{
	pid_t		pid;
	t_params	p;
	int			i;

	printf("My ID #%d\n", 1);
	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	printf("My ID #%d\n", 1);
	if (params_init(&p, argv, argc) == 1)
		return (1);
	printf("My ID #%d\n", 1);
	i = -1;
	while (++i < p.num_philo)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
			break;
	}
	if (pid == 0)
	{
		p.philo = philo_init(&p, i);
		p_process(&p, p.philo);
		printf("My ID #%d\n", p.philo->id);
	}
	return (0);
}