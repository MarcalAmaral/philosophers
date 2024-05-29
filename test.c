#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

void	*routine(int *var)
{
	int i;

	i = 0;
	while (i < 10)
	{
		*var += 1;
		i++;
	}
}

void	create_threads_for_philos(int n_philos, int *var)
{
	pthread_t			th[n_philos];
	int	i = 0;

	while (i < n_philos)
	{
		if (pthread_create(&th[i], NULL, &routine, var))
		{
			perror("pthread create error");
			return ;
		}
		i++;
	}
	i = 0;
	while (i < n_philos)
	{
		if (pthread_join(th[i], NULL))
		{
			perror("pthread join error");
			return ;
		}
		i++;
	}
	return ;
}

int main(void)
{
	int	number_of_philos = 220;
	int	var = 0;

	create_threads_for_philos(number_of_philos, &var);
	printf("Valur of var: %d\n", var);
	// pthread_t t1;
	// int var = 0;

	// pthread_create(&t1, NULL, &routine, &var);
	// pthread_join(t1, NULL);
	// printf("Value of var %d\n", var);
	return (0);
}