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

// Instantiation structs, verification and free structs.
#include <stdio.h>

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
