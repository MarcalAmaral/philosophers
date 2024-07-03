/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:32:13 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/03 04:51:33 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	write_act_msg(enum e_actions action, t_philo *philo)
{
	char	*timestamp_start_of_simulation;
	char	*str_philo_id;
	char	*str_philo_act;
	char	*str_formatted;

	if (!philo->data[0]->state_of_simulation)
		return (EXIT_FAILURE);
	str_philo_act = "";
	timestamp_start_of_simulation = ft_litoa(get_current_timestamp(MILLISECONDS) - philo->data[0]->timestamp_of_simulation);
	str_philo_id = ft_litoa(philo->id);
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
	str_formatted = format_string(timestamp_start_of_simulation, " ", str_philo_id, str_philo_act);
	ft_putstr_fd(str_formatted, STDOUT_FILENO);
	free(str_formatted);
	free(timestamp_start_of_simulation);
	free(str_philo_id);
	return (EXIT_SUCCESS);
}

static int	eat(t_philo *info_philo, int *number_of_meals)
{
	if ((info_philo->id % 2) == 0)
		pthread_mutex_lock(info_philo->right_fork);
	else
		pthread_mutex_lock(&info_philo->philo_fork);
	if (write_act_msg(TAKEN_FORK, info_philo))
	{
		if ((info_philo->id % 2) == 0)
			pthread_mutex_unlock(info_philo->right_fork);
		else
			pthread_mutex_unlock(&info_philo->philo_fork);
		return (EXIT_FAILURE);
	}
	if ((info_philo->id % 2) == 0)
		pthread_mutex_lock(&info_philo->philo_fork);
	else
		pthread_mutex_lock(info_philo->right_fork);
	if (write_act_msg(TAKEN_FORK, info_philo))
	{
		pthread_mutex_unlock(info_philo->right_fork);
		pthread_mutex_unlock(&info_philo->philo_fork);
		return (EXIT_FAILURE);
	}
	if (write_act_msg(EATING, info_philo))
	{
		pthread_mutex_unlock(info_philo->right_fork);
		pthread_mutex_unlock(&info_philo->philo_fork);
		return (EXIT_FAILURE);
	}
	*number_of_meals += 1;
	info_philo->last_meal = get_current_timestamp(MILLISECONDS);
	if (info_philo->data[0]->state_of_simulation)
		usleep(info_philo->data[0]->time_to_eat_in_us);
	pthread_mutex_unlock(info_philo->right_fork);
	pthread_mutex_unlock(&info_philo->philo_fork);
	return (EXIT_SUCCESS);
}

static void	*dinner_routine(void *arg)
{
	t_philo	*info;
	int		number_of_meals;

	info = (t_philo *) arg;
	number_of_meals = 0;
	while (info->data[0]->state_of_simulation)
	{
		if (info->data[0]->nb_of_times_each_philo_must_eat != -1)
		{
			if (number_of_meals == info->data[0]->nb_of_times_each_philo_must_eat)
			{
				info->satisfied = TRUE;
				return (NULL);
			}
		}
		if (eat(info, &number_of_meals))
			return (NULL);
		if (write_act_msg(SLEEPING, info))
			return (NULL);
		if (info->data[0]->state_of_simulation)
			usleep(info->data[0]->time_to_sleep_in_us);
		if (write_act_msg(THINKING, info))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}

static void	*monitor_routine(void *arg)
{
	t_manager	*manager_info;
	long int	current_timestamp;
	long int	iteration;
	int			signal_to_stop;

	manager_info = (t_manager *) arg;
	signal_to_stop = TRUE;
	while (manager_info->data[0].state_of_simulation)
	{
		iteration = 0;
		current_timestamp = get_current_timestamp(MILLISECONDS);
		while (iteration < manager_info->data->nb_of_philos)
		{
			if (manager_info->data[0].nb_of_times_each_philo_must_eat != -1)
			{
				if (manager_info->philos[iteration].satisfied == FALSE)
					signal_to_stop = FALSE;
				if ((iteration + 1) == manager_info->data->nb_of_philos)
				{
					if (signal_to_stop)
					{
						manager_info->data[0].state_of_simulation = FALSE;
						break ;	
					}
				}
			}
			if ((current_timestamp - manager_info->philos[iteration].last_meal) > manager_info->data->time_to_die_in_ms)
			{
				write_act_msg(DIED, &manager_info->philos[iteration]);
				manager_info->data[0].state_of_simulation = FALSE;
				break ;
			}
			iteration++;
		}
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
	manager->data->state_of_simulation = TRUE;
	manager->data->timestamp_of_simulation = current_timestamp;
	while (array_index < manager->data->nb_of_philos)
	{
		manager->philos[array_index].last_meal = current_timestamp;
		error = pthread_create(&manager->philos[array_index].philo, NULL, &dinner_routine, &manager->philos[array_index]);
		if (error != NOERROR)
			return (err_msg_create_thread(error, manager->philos[array_index].id, manager));
		array_index++;
	}
	error = pthread_create(&manager->monitor, NULL, &monitor_routine, manager);
	if (error != NOERROR)
		return (err_msg_create_thread(error, -1, manager));
	array_index = 0;
	while (array_index < manager->data->nb_of_philos)
	{
		error = pthread_join(manager->philos[array_index].philo, NULL);
		if (error != NOERROR)
			return (err_msg_join_thread(error, manager->philos[array_index].id, manager));
		array_index++;
	}
	pthread_join(manager->monitor, NULL);
	deallocating_structures(manager);
	return (EXIT_SUCCESS);
}
