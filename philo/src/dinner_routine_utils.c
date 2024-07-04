/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_routine_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 00:32:49 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:18:33 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_first_fork(t_philo *philo)
{
	if ((philo->id % 2) == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(&philo->philo_fork);
	return ;
}

void	pick_up_second_fork(t_philo *philo)
{
	if ((philo->id % 2) == 0)
		pthread_mutex_lock(&philo->philo_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	return ;
}

int	first_validation_before_dinner(t_philo *philo)
{
	if (write_act_msg(TAKEN_FORK, philo))
	{
		if ((philo->id % 2) == 0)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(&philo->philo_fork);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	second_validation_before_dinner(t_philo *philo)
{
	if (write_act_msg(TAKEN_FORK, philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->philo_fork);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	validation_before_eating(t_philo *philo, int *number_of_meals)
{
	if (write_act_msg(EATING, philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->philo_fork);
		return (EXIT_FAILURE);
	}
	*number_of_meals += 1;
	philo->last_meal = get_current_timestamp(MILLISECONDS);
	if (consult_state_of_simulation(philo))
		usleep(philo->data[0]->time_to_eat_in_us);
	return (EXIT_SUCCESS);
}
