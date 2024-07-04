/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 03:52:16 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:46:53 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_die_msg(t_philo *philo)
{
	char	*timestamp_start_of_simulation;
	char	*str_id;
	char	*str_formatted;

	timestamp_start_of_simulation = ft_litoa(get_current_timestamp(MILLISECONDS)
			- philo->data[0]->timestamp_of_simulation);
	str_id = ft_litoa(philo->id);
	str_formatted = format_string(timestamp_start_of_simulation, " ",
			str_id, " died\n");
	ft_putstr_fd(str_formatted, STDOUT_FILENO);
	free(timestamp_start_of_simulation);
	free(str_id);
	free(str_formatted);
	return (TRUE);
}

static int	is_it_time_to_die(t_manager *manager, long int current_timestamp,
		int iteration)
{
	if ((current_timestamp - manager->philos[iteration].last_meal)
		> manager->data->time_to_die_in_ms
		&& manager->philos[iteration].satisfied == FALSE)
	{
		pthread_mutex_lock(&manager->data[0].state_of_simulation_mutex);
		manager->data[0].state_of_simulation = FALSE;
		pthread_mutex_unlock(&manager->data[0].state_of_simulation_mutex);
		return (philo_die_msg(&manager->philos[iteration]));
	}
	return (FALSE);
}

static int	consult_state_of_simulation_monitor(t_manager *manager)
{
	int	state;

	state = TRUE;
	pthread_mutex_lock(&manager->data->state_of_simulation_mutex);
	if (!manager->data->state_of_simulation)
		state = FALSE;
	pthread_mutex_unlock(&manager->data->state_of_simulation_mutex);
	return (state);
}

void	*monitor_routine(void *arg)
{
	t_manager	*manager;
	long int	current_timestamp;
	long int	iteration;

	manager = (t_manager *) arg;
	if (is_satisfied(manager))
		return (NULL);
	while (consult_state_of_simulation_monitor(manager))
	{
		iteration = 0;
		current_timestamp = get_current_timestamp(MILLISECONDS);
		if (is_satisfied(manager))
			break ;
		while (iteration < manager->data->nb_of_philos)
		{
			if (is_it_time_to_die(manager, current_timestamp, iteration))
				return (NULL);
			iteration++;
		}
	}
	return (NULL);
}
