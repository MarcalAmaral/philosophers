/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deallocating_structures.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:45:37 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:09:46 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	deallocating_structures(t_manager *manager)
{
	int				array_index;
	enum e_error	error;

	array_index = 0;
	while (array_index < manager->data->nb_of_philos)
	{
		free(manager->philos[array_index].data);
		error = pthread_mutex_destroy(&manager->philos[array_index].philo_fork);
		if (error != NOERROR)
			error = err_msg_mutex_destroy(error,
					manager->philos[array_index].id);
		array_index++;
	}
	pthread_mutex_destroy(&manager->data->state_of_simulation_mutex);
	free(manager->data);
	free(manager->philos);
	return ;
}
