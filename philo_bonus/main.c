#include "philosophers.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void	clean_exit(t_params *p)
{
	sem_close(p->die_sem);
	sem_close(p->forks);
	sem_close(p->time_sem);
	sem_close(p->last_eat_sem);
	sem_close(p->ate_sem);
	sem_close(p->print_sem);
	sem_unlink("forks");
	sem_unlink("die_sem");
	sem_unlink("time_sem");
	sem_unlink("last_eat");
	sem_unlink("ate_sem");
	sem_unlink("print_sem");
	exit(0);
}

void	ft_eating(t_philo *philo)
{
	sem_wait(philo->p->print_sem);
	printf("%lu %d is eating\n", get_time(philo->p), philo->id + 1);
	sem_post(philo->p->print_sem);
	sem_wait(philo->p->last_eat_sem);
	philo->last_eat = get_time(philo->p);
	sem_post(philo->p->last_eat_sem);
	ft_usleep(philo->p, philo->p->time_eat);
	sem_post(philo->p->forks);
	sem_post(philo->p->forks);
	sem_wait(philo->p->ate_sem);
	philo->ate++;
	if (philo->p->eat_max != 0 && philo->ate == philo->p->eat_max)
	{
		sem_post(philo->p->ate_sem);
		exit(1);
	}
	sem_post(philo->p->ate_sem);
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
			exit (0);
		}
		sem_wait(philo->p->forks);
		sem_wait(philo->p->print_sem);
		printf("%lu %d has taken a fork\n", get_time(philo->p), philo->id + 1);
		sem_post(philo->p->print_sem);
		sem_wait(philo->p->forks);
		sem_wait(philo->p->print_sem);
		printf("%lu %d has taken a fork\n", get_time(philo->p), philo->id + 1);
		sem_post(philo->p->print_sem);
		ft_eating(philo);
		if (die_return(philo) == 1)
		{
			sem_post(philo->p->forks);
			sem_post(philo->p->forks);
			exit (0);
		}
		sem_wait(philo->p->print_sem);
		printf("%lu %d is sleeping\n", get_time(philo->p), philo->id + 1);
		sem_post(philo->p->print_sem);
		ft_usleep(philo->p, philo->p->time_sleep);
		if (die_return(philo) == 1)
		{
			sem_post(philo->p->forks);
			sem_post(philo->p->forks);
			exit (0);
		}
		sem_wait(philo->p->print_sem);
		printf("%lu %d is thinking\n", get_time(philo->p), philo->id + 1);
		sem_post(philo->p->print_sem);
		if (die_return(philo) == 1)
		{
			sem_post(philo->p->forks);
			sem_post(philo->p->forks);
			exit (0);
		}
	}
	return (0);
}
void	*die_check(void *args)
{
	t_params	*p;

	p = (t_params *)args;
	while (1)
	{
		sem_wait(p->last_eat_sem);
		sem_wait(p->die_sem);
		if (get_time(p) - p->philo->last_eat > (unsigned long)p->time_die)
		{
			sem_wait(p->print_sem);
			printf("%lu %d died, lasteat - %d\n", get_time(p), p->philo->id + 1, p->philo->last_eat);
			p->die = 1;
			exit (0);
		}
		sem_post(p->die_sem);
		sem_post(p->last_eat_sem);
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
	t_params	p;
	int			i;
	int			status;

	status = 256;
	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	if (params_init(&p, argv, argc) == 1)
		return (1);
	i = 0;
	while (i < p.num_philo)
	{
		p.pids[i] = fork();
		if (p.pids[i] == -1)
			exit(1);
		if (p.pids[i] == 0)
			break;
		i++;
	}
	if (i < p.num_philo)
	{
		printf("%d - ID\n", i);
		p.philo = philo_init(&p, i);
		p_process(&p, p.philo);
	}
	if (i >= p.num_philo)
	{
		while (status == 256 && --i >= 0)
			waitpid(-1, &status, 0);
		printf("status - %d\n", status);
		while (++i < p.num_philo)
			kill(p.pids[i], SIGKILL);
	}
	clean_exit(&p);
	exit (0);
}