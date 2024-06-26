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

int	ft_strlen(const char *string)
{
	int counter;

	counter = 0;
	if (!string)
		return (counter);
	while (string[counter])
		counter++;
	return (counter);	
}

// format string
char	*format_string(const char *s, const char *s1, const char *s2, const char *s3)
{
	int		lenght;
	int		index;
	char	*formatted_string;

	lenght = (ft_strlen(s) + ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3));
	formatted_string = malloc((lenght + 1) * sizeof(char));
	formatted_string[lenght] = '\0';
	lenght = 0;
	index = 0;
	while (s[index])
		formatted_string[lenght++] = s[index++];
	index = 0;
	while (s1[index])
		formatted_string[lenght++] = s1[index++];
	index = 0;
	while (s2[index])
		formatted_string[lenght++] = s2[index++];
	index = 0;
	while (s3[index])
		formatted_string[lenght++] = s3[index++];
	return (formatted_string);
}

// Argument validation
int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (TRUE);
	return (FALSE);
}

void	write_stderr(const char *string)
{
	if (!string)
		return ;
	write(STDERR_FILENO, string, ft_strlen(string));
}

int	arg_err_msg(int type, const char *arg)
{
	char *str_error;

	str_error = "";
	if (type == ('a' + 'c'))
		write_stderr("Error\n philosophers: invalid number of args\n");
	else if (type == ('a' + 'v'))
	{
		str_error = format_string("Error\n philosophers: (", arg,
			"): invalid argument, the program receives only digits\n", "");
		write_stderr(str_error);
		free(str_error);
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
	char	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EAGAIN)
		str_error_type = "): Resource temporarily unavailable\n";
	else if (error == ENOMEM)
		str_error_type = "): Cannot allocate memory\n";
	else if (error == EPERM)
		str_error_type = "): Operation not permitted\n";
	else if (error == EBUSY)
		str_error_type = "): Device or resource busy\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid argument\n";
	str_error = format_string("Occur an error when destroy thread of id: (", str_id, str_error_type, "");
	write_stderr(str_error);
	free(str_id);
	free(str_error);
	return ;
}

int	err_msg_mutex_destroy(enum e_error error, long int id)
{
	char	*str_id;
	char	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EBUSY)
		str_error_type = "): Device or resource busy\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid argument\n";
	str_error = format_string("Occur an error when destroy thread of id: (", str_id, str_error_type, "");
	write_stderr(str_error);
	free(str_id);
	free(str_error);
	return (NOERROR);
}

void	destroying_structs(t_manager *manager)
{
	int				array_index;
	enum e_error	error;

	array_index = 0;
	while (array_index < manager->data->nb_of_philos)
	{
		free(manager->philos[array_index].data);
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
	printf("time to eat in microseconds: %ld\n", data->time_to_eat_in_us);
	printf("time to sleep in microseconds: %ld\n", data->time_to_sleep_in_us);
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
	write_stderr("Cannot allocate memory\n");
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
	data->time_to_eat_in_us = (ft_atoli(av[3]) * 1e3);
	data->time_to_sleep_in_us = (ft_atoli(av[4]) * 1e3);
	data->nb_of_times_each_philo_must_eat = -1;
	if (av[5] != NULL)
		data->nb_of_times_each_philo_must_eat = ft_atoli(av[5]);
	data->timestamp_of_simulation = 0;
	return (data);
}

void	instantiation_philo_struct(t_philo *philo, pthread_mutex_t *right_fork, t_data *data)
{
	static int		id;
	enum e_error	error;

	philo->id = id;
	philo->last_meal = 0;
	philo->data = malloc(1 * sizeof(t_data **));
	*philo->data = data;
	error = pthread_mutex_init(&philo->philo_fork, NULL);
	if (error != NOERROR)
		err_msg_mutex_init(error, philo->id);
	philo->right_fork = right_fork;
	id++;
}

t_philo	*instantiation_array_philos(long int nb_of_philos, t_data *data)
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
			&philos[(array_index + 1) % nb_of_philos].philo_fork, data);
		array_index++;
	}
	return (philos);
}

int	instantiation_manager_struct(char **av, t_manager *manager)
{
	manager->data = instantiation_data_struct(av);
	if (!manager->data)
		return (instantiation_struct_err_msg());
	manager->philos = instantiation_array_philos(manager->data->nb_of_philos, manager->data);
	if (!manager->philos)
		return (instantiation_struct_err_msg());
	// print_data(manager->data);
	// print_array_philo(manager->philos, manager->data->nb_of_philos);
	// destroying_structs(manager);
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
	write_stderr("An error occurred when get current timestamp\n");
	return (EXIT_FAILURE);
}

int	err_msg_detach_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;
	char 	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EINVAL)
		str_error_type = "): Is not a joinable thread\n";
	else if (error == ESRCH)
		str_error_type = "): No thread with the ID thread could be found\n";
	str_error = format_string("An error occurred when detach thread of id: (", str_id, str_error_type, "");
	write_stderr(str_error);
	free(str_id);
	free(str_error);
	destroying_structs(manager);
	return (EXIT_FAILURE);
}

int	err_msg_create_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;
	char 	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EAGAIN)
		str_error_type = "): Resource temporarily unavailable\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid settings in attr\n";
	else if (error == EPERM)
		str_error_type = "): No permission to set the scheduling \
policy and parameters specified in attr\n";
	str_error = format_string("An error occurred when create thread of id: (", str_id, str_error_type, "");
	write_stderr(str_error);
	free(str_id);
	free(str_error);
	destroying_structs(manager);
	return (EXIT_FAILURE);
}

void	write_stdout(const char *string)
{
	if (!string)
		return ;
	write(STDOUT_FILENO, string, ft_strlen(string));
}

void	write_act_msg(enum e_actions action, long int philo_id, long int current_timestamp_ms)
{
	char	*str_current_timestamp_ms;
	char	*str_philo_id;
	char	*str_philo_act;
	char	*str_formatted;

	str_philo_act = "";
	str_current_timestamp_ms = ft_litoa(current_timestamp_ms);
	str_philo_id = ft_litoa(philo_id);
	if (action == TAKEN_FORK)
		str_philo_act = " has taken a fork\n";
	else if (action == EATING)
		str_philo_act = " is eating\n";
	else if (action == SLEEPING)
		str_philo_act = " is sleeping\n";
	else if (action == THINKING)
		str_philo_act = " is thinking\n";
	else if (action == DIED)
		str_philo_act = " died\n";
	str_formatted = format_string(str_current_timestamp_ms, " ", str_philo_id, str_philo_act);
	write_stdout(str_formatted);
	free(str_formatted);
	free(str_current_timestamp_ms);
	free(str_philo_id);
	return ;
}

void	eat(t_philo *info_philo)
{
	long int current_timestamp_in_ms;

	pthread_mutex_lock(info_philo->right_fork);
	current_timestamp_in_ms = get_current_timestamp(MILLISECONDS);
	write_act_msg(TAKEN_FORK, info_philo->id, current_timestamp_in_ms);
	pthread_mutex_lock(&info_philo->philo_fork);
	current_timestamp_in_ms = get_current_timestamp(MILLISECONDS);
	write_act_msg(TAKEN_FORK, info_philo->id, current_timestamp_in_ms);
	write_act_msg(EATING, info_philo->id, current_timestamp_in_ms);
	info_philo->last_meal = current_timestamp_in_ms;
	usleep(info_philo->data[0]->time_to_eat_in_us);
	pthread_mutex_unlock(info_philo->right_fork);
	pthread_mutex_unlock(&info_philo->philo_fork);
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
		usleep(500);
	while (TRUE)
	{
		eat(info);
		write_act_msg(SLEEPING, info->id, get_current_timestamp(MILLISECONDS));
		usleep(info->data[0]->time_to_sleep_in_us);
		write_act_msg(THINKING, info->id, get_current_timestamp(MILLISECONDS));
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_manager	*manager_info;
	long int	current_timestamp;
	long int	iteration;
	long int	time_for_another_check;
	int			simulation_state;

	manager_info = (t_manager *) arg;
	time_for_another_check = ((manager_info->data->time_to_die_in_ms - 10) * 1e3);
	simulation_state = TRUE;
	while (TRUE)
	{
		current_timestamp = get_current_timestamp(MILLISECONDS);
		iteration = 0;
		while (iteration < manager_info->data->nb_of_philos)
		{
			if ((current_timestamp - manager_info->philos[iteration].last_meal) > manager_info->data->time_to_die_in_ms)
			{
				write_act_msg(DIED, manager_info->philos[iteration].id, current_timestamp);
				simulation_state = FALSE;
				break ;
			}
			iteration++;
		}
		if (simulation_state == FALSE)
			break ;
		usleep(time_for_another_check);
	}
	return (NULL);
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
	error = pthread_create(&manager->monitor, NULL, &monitor_routine, manager);
	if (error != NOERROR)
		return (err_msg_create_thread(error, -1, manager));
	pthread_join(manager->monitor, NULL);
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