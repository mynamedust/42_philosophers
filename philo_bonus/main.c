#include "philosophers.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
	pid_t		pid;
	t_params	p;
	int			i;
	// int			status_addr;

	if (argc < 5 || params_valid(argv + 1, argc - 1))
		return (1);
	params_init(&p, argv, argc);
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
		printf("My ID #%d\n", p.philo->id);
	}
	return (0);
}