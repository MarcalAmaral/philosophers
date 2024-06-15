/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 23:30:47 by myokogaw          #+#    #+#             */
/*   Updated: 2024/06/13 23:30:47 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// args for program and sequence respectively -> number of philos, time to die,
// time to eat, time to sleep and [optionally -> number of each times each phi-
// sophers must eat].
// exclude argument of filename bin, we have 4 (ac == 5) arguments or 5 (ac == 6). 


// Argument validation
int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (TRUE);
	return (FALSE);
}

void	write_err(const char *string)
{
	if (!string)
		return ;
	while (*string)
		string += write(STDERR_FILENO, string, 1);
}

int	arg_err_msg(int type, const char *arg)
{
	if (type == ('a' + 'c'))
		write_err("Error\n philosophers: invalid number of args\n");
	else if (type == ('a' + 'v'))
	{
		write_err("Error\n philosophers: (");
		write_err(arg);
		write_err("): invalid argument, the program receives only digits\n");
	}
	return (EXIT_FAILURE);
}

int	arg_validation(int ac, char **av)
{
	int	matrix_index;
	int	string_index;

	if (ac < 5 || ac > 6)
		return (arg_err_msg(('a' + 'c'), NULL));
	matrix_index = 1;
	while (av[matrix_index])
	{
		string_index = 0;
		if (av[matrix_index][string_index] == '\0')
			return (arg_err_msg(('a' + 'v'), av[matrix_index]));
		while (av[matrix_index][string_index])
		{
			if (!ft_isdigit(av[matrix_index][string_index]))
				return (arg_err_msg(('a' + 'v'), av[matrix_index]));
			string_index++;
		}
		matrix_index++;
	}
	return (EXIT_SUCCESS);
}

// Instantiation structs, verification and free structs.
#include <stdio.h>

static long int	long_int_lenght(long int num)
{
	long int	lenght;

	lenght = 0;
	if (num <= 0)
		lenght += 1;
	while (num != 0)
	{
		lenght++;
		num = num / 10;
	}
	lenght++;
	return (lenght);
}

char	*ft_litoa(long int num)
{
	long int	iterator;
	long int	alloc;
	char		*string;

	alloc = long_int_lenght(num);
	iterator = alloc - 1;
	string = malloc(alloc * sizeof(char));
	if (!string)
		return (malloc(0));
	if (num == 0)
		string[0] = '0';
	if (num < 0)
	{
		num *= -1;
		string[0] = '-';
	}
	string[iterator] = '\0';
	while (iterator--, num != 0)
	{
		string[iterator] = num % 10 + '0';
		num = num / 10;
	}
	return (string);
}

void	err_msg_mutex_init(enum e_error error, long int id)
{
	char	*str_id;

	str_id = ft_itoa(id);
	write_err("Occur an error when destroy thread of id: (");
	write_err(str_id);
	if (error == EAGAIN)
		write_err("): Resource temporarily unavailable\n");
	else if (error == ENOMEM)
		write_err("): Cannot allocate memory\n");
	else if (error == EPERM)
		write_err("): Operation not permitted\n");
	else if (error == EBUSY)
		write_err("): Device or resource busy\n");
	else if (error == EINVAL)
		write_err("): Invalid argument\n");
	free(str_id);
	return ;
}

int	err_msg_mutex_destroy(enum e_error error, long int id)
{
	char	*str_id;

	str_id = ft_itoa(id);
	write_err("Occur an error when destroy thread of id: (");
	write_err(str_id);
	if (error == EBUSY)
		write_err("): Device or resource busy\n");
	else if (error == EINVAL)
		write_err("): Invalid argument\n");
	free(str_id);
	return (NOERROR);
}

void	destroying_structs(t_manager *manager)
{
	int				array_index;
	enum e_error	error;

	array_index = 0;
	while (array_index < manager->data->nb_of_philos)
	{
		error = pthread_mutex_destroy(&manager->philos[array_index].philo_fork);
		if (error != NOERROR)
			error = err_msg_mutex_destroy(error, manager->philos[array_index].id);
		array_index++;
	}
	free(manager->data);
	free(manager->philos);
	return ;
}

void	print_array_philo(t_philo *philos, long int nb_of_philos)
{
	int	array_index;

	array_index = 0;
	while (array_index < nb_of_philos)
	{
		printf("id of philosopher: %ld\n", philos[array_index].id);
		printf("timestamp of last meal of philosopher: %ld\n", philos[array_index].last_meal);
		array_index++;
	}
	return ;
}

void	print_data(t_data *data)
{
	printf("quantity of philosophers: %ld\n", data->nb_of_philos);
	printf("time to die in milliseconds: %ld\n", data->time_to_die_in_ms);
	printf("time to eat in milliseconds: %ld\n", data->time_to_eat_in_ms);
	printf("time to sleep in milliseconds: %ld\n", data->time_to_sleep_in_ms);
	printf("quantity of times each philosopher must eat: %ld\n", data->nb_of_times_each_philo_must_eat);
	printf("timestamp of simulation: %ld\n", data->timestamp_of_simulation);
	return ;
}

long int	ft_atoli(const char *nptr)
{
	long int	result;

	result = 0;
	while ((*nptr >= '0') && (*nptr <= '9'))
	{
		result *= 10;
		result += *nptr - '0';
		nptr++;
	}
	return (result);
}

int	instantiation_struct_err_msg(void)
{
	write_err("Cannot allocate memory\n");
	return (EXIT_FAILURE);
}

t_data	*instantiation_data_struct(char **av)
{
	t_data	*data;

	data = (t_data *) malloc(1 * sizeof(t_data));
	if (!data)
		return (NULL);
	data->nb_of_philos = ft_atoli(av[1]);
	data->time_to_die_in_ms = ft_atoli(av[2]);
	data->time_to_eat_in_ms = ft_atoli(av[3]);
	data->time_to_sleep_in_ms = ft_atoli(av[4]);
	data->nb_of_times_each_philo_must_eat = -1;
	if (av[5] != NULL)
		data->nb_of_times_each_philo_must_eat = ft_atoli(av[5]);
	data->timestamp_of_simulation = 0;
	return (data);
}

void	instantiation_philo_struct(t_philo *philo, pthread_mutex_t *right_fork)
{
	static int		id;
	enum e_error	error;

	philo->id = id;
	philo->last_meal = 0;
	error = pthread_mutex_init(&philo->philo_fork, NULL);
	if (error != NOERROR)
		err_msg_mutex_init(error, philo->id);
	philo->right_fork = right_fork;
	id++;
}

t_philo	*instantiation_array_philos(long int nb_of_philos)
{
	t_philo	*philos;
	int		array_index;

	philos = (t_philo *) malloc(nb_of_philos * sizeof(t_philo));
	if (!philos)
		return (NULL);
	array_index = 0;
	while (array_index < nb_of_philos)
	{
		instantiation_philo_struct(&philos[array_index],
			&philos[(array_index + 1) % nb_of_philos].philo_fork);
		array_index++;
	}
	return (philos);
}

int	instantiation_manager_struct(char **av, t_manager *manager)
{
	manager->data = instantiation_data_struct(av);
	if (!manager->data)
		return (instantiation_struct_err_msg());
	manager->philos = instantiation_array_philos(manager->data->nb_of_philos);
	if (!manager->philos)
		return (instantiation_struct_err_msg());
	print_data(manager->data);
	print_array_philo(manager->philos, manager->data->nb_of_philos);
	return (EXIT_SUCCESS);
}

// Start dinner simulation
long int	get_current_timestamp(enum e_unit_time unit)
{
	struct timeval	tv;

	if (!gettimeofday(&tv, NULL))
	{
		if (unit == SECONDS)
			return (tv.tv_sec);
		else if (unit == MILLISECONDS)
			return (((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3)));
		else if (unit == MICROSECONDS)
			return (((tv.tv_sec * 1e6) + tv.tv_usec));
	}
	write_err("An error occurred when get current timestamp\n");
	return (EXIT_FAILURE);
}

int	err_msg_detach_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;

	str_id = ft_itoa(id);
	write_err("An error occurred when detach thread of id: (");
	write_err(str_id);
	if (error == EINVAL)
		write_err("): Is not a joinable thread\n");
	else if (error == ESRCH)
		write_err("): No thread with the ID thread could be found\n");
	free(str_id);
	destroying_structs(manager);
	return (EXIT_FAILURE);
}

int	err_msg_create_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;

	str_id = ft_itoa(id);
	write_err("An error occurred when create thread of id: (");
	write_err(str_id);
	if (error == EAGAIN)
		write_err("): Resource temporarily unavailable\n");
	else if (error == EINVAL)
		write_err("): Invalid settings in attr\n");
	else if (error == EPERM)
		write_err("): No permission to set the scheduling \
policy and parameters specified in attr\n");
	free(str_id);
	destroying_structs(manager);
	return (EXIT_FAILURE);
}

void	write_stdout(const char *string)
{
	if (!string)
		return ;
	while (*string)
		string += write(STDOUT_FILENO, string, 1);
	return ;
}

void	write_act_msg(enum e_actions action)
{
	write_stdout()
}

void	eat(void)
{
	return ;
}

void	thinking(void)
{
	return ;
}

void	*dinner_routine(void *arg)
{
	t_philo	*info;

	info = (t_philo *) arg;
	if ((info->id % 2) == 0)
		usleep(50);
	while (TRUE)
	{
		eat();
		usleep();
		thinking();
	}

}

int	start_simulation(t_manager *manager)
{
	int				array_index;
	long int		current_timestamp;
	enum e_error	error;

	if (!manager)
		return (EXIT_FAILURE);
	array_index = 0;
	current_timestamp = get_current_timestamp(MILLISECONDS);
	manager->data->timestamp_of_simulation = current_timestamp;
	while (array_index < manager->data->nb_of_philos)
	{
		manager->philos[array_index].last_meal = current_timestamp;
		error = pthread_create(&manager->philos[array_index].philo, NULL, &dinner_routine, &manager->philos[array_index]);
		if (error != NOERROR)
			return (err_msg_create_thread(error, manager->philos[array_index].id, manager));
		error = pthread_detach(manager->philos[array_index].philo);
		if (error != NOERROR)
			return (err_msg_detach_thread(error, manager->philos[array_index].id, manager));
		array_index++;
	}
	destroying_structs(manager);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	t_manager	manager;

	if (arg_validation(ac, av))
		return (EXIT_FAILURE);
	if (instantiation_manager_struct(av, &manager))
		return (EXIT_FAILURE);
	if (start_simulation(&manager))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}