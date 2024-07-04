/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:32:13 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:31:06 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initializing_philo_threads(t_manager *manager)
{
	long int		index;
	enum e_error	error;

	index = 0;
	while (index < manager->data->nb_of_philos)
	{
		manager->philos[index].last_meal = \
		manager->data->timestamp_of_simulation;
		error = pthread_create(&manager->philos[index].philo, NULL, \
		&dinner_routine, &manager->philos[index]);
		if (error != NOERROR)
			return (err_msg_create_thread(error, manager->philos[index].id, \
			manager));
		index++;
	}
	return (EXIT_SUCCESS);
}

int	joining_philo_threads(t_manager *manager)
{
	long int		index;
	enum e_error	error;

	index = 0;
	while (index < manager->data->nb_of_philos)
	{
		error = pthread_join(manager->philos[index].philo, NULL);
		if (error != NOERROR)
			return (err_msg_join_thread(error, manager->philos[index].id, \
			manager));
		index++;
	}
	return (EXIT_SUCCESS);
}

void	*exceptional_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	if (philo->data[0]->nb_of_times_each_philo_must_eat == 0)
	{
		philo->satisfied = TRUE;
		return (NULL);
	}
	write_act_msg(TAKEN_FORK, philo);
	while (consult_state_of_simulation(philo))
		continue ;
	return (NULL);
}

int	exceptional_simulation(t_manager *manager)
{
	enum e_error	error;

	if (manager->data->nb_of_philos == 1)
	{
		manager->philos[0].last_meal = manager->data->timestamp_of_simulation;
		error = pthread_create(&manager->philos[0].philo, NULL,
				&exceptional_routine, &manager->philos[0]);
		if (error != NOERROR)
			return (err_msg_create_thread(error,
					manager->philos[0].id, manager));
		error = pthread_create(&manager->monitor, NULL,
				&monitor_routine, manager);
		if (error != NOERROR)
			return (err_msg_create_thread(error, -1, manager));
		error = pthread_join(manager->philos[0].philo, NULL);
		if (error != NOERROR)
			return (err_msg_join_thread(error, manager->philos[0].id, manager));
		error = pthread_join(manager->monitor, NULL);
		if (error != NOERROR)
			return (err_msg_join_thread(error, -1, manager));
		return (TRUE);
	}
	return (FALSE);
}

int	start_simulation(t_manager *manager)
{
	enum e_error	error;

	if (!manager)
		return (EXIT_FAILURE);
	manager->data->state_of_simulation = TRUE;
	manager->data->timestamp_of_simulation = \
get_current_timestamp(MILLISECONDS);
	if (!exceptional_simulation(manager))
	{
		if (initializing_philo_threads(manager))
			return (EXIT_FAILURE);
		error = pthread_create(&manager->monitor, NULL,
				&monitor_routine, manager);
		if (error != NOERROR)
			return (err_msg_create_thread(error, -1, manager));
		if (joining_philo_threads(manager))
			return (EXIT_FAILURE);
		error = pthread_join(manager->monitor, NULL);
		if (error != NOERROR)
			return (err_msg_join_thread(error, -1, manager));
	}
	deallocating_structures(manager);
	return (EXIT_SUCCESS);
}
