/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instantiation_of_structures.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:36:07 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/03 04:33:31 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	instantiation_struct_err_msg(void)
{
	ft_putstr_fd("Cannot allocate memory\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

static t_data	*instantiation_data_struct(char **av)
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

static void	instantiation_philo_struct(t_philo *philo, pthread_mutex_t *right_fork, t_data *data)
{
	static int		id = 1;
	enum e_error	error;

	philo->id = id;
	philo->last_meal = 0;
	philo->satisfied = FALSE;
	philo->data = malloc(1 * sizeof(t_data **));
	*philo->data = data;
	error = pthread_mutex_init(&philo->philo_fork, NULL);
	if (error != NOERROR)
		err_msg_mutex_init(error, philo->id);
	philo->right_fork = right_fork;
	id++;
}

static t_philo	*instantiation_array_philos(long int nb_of_philos, t_data *data)
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
	return (EXIT_SUCCESS);
}
