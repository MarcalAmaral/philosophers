/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 03:53:49 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:47:49 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	consult_state_of_simulation(t_philo *philo)
{
	int	state;

	state = TRUE;
	pthread_mutex_lock(&philo->data[0]->state_of_simulation_mutex);
	if (!philo->data[0]->state_of_simulation)
		state = FALSE;
	pthread_mutex_unlock(&philo->data[0]->state_of_simulation_mutex);
	return (state);
}

static int	eat(t_philo *philo, int *number_of_meals)
{
	pick_up_first_fork(philo);
	if (first_validation_before_dinner(philo))
		return (EXIT_FAILURE);
	pick_up_second_fork(philo);
	if (second_validation_before_dinner(philo))
		return (EXIT_FAILURE);
	if (validation_before_eating(philo, number_of_meals))
		return (EXIT_FAILURE);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->philo_fork);
	return (EXIT_SUCCESS);
}

static int	dinner_meals_validation(t_philo *philo, int *number_of_meals)
{
	if (philo->data[0]->nb_of_times_each_philo_must_eat != -1)
	{
		if (*number_of_meals == philo->data[0]->nb_of_times_each_philo_must_eat)
		{
			philo->satisfied = TRUE;
			return (TRUE);
		}
	}
	return (FALSE);
}

void	*dinner_routine(void *arg)
{
	t_philo	*philo;
	int		number_of_meals;

	philo = (t_philo *) arg;
	if (philo->data[0]->nb_of_times_each_philo_must_eat == 0)
	{
		philo->satisfied = TRUE;
		return (NULL);
	}
	number_of_meals = 0;
	while (consult_state_of_simulation(philo))
	{
		if (eat(philo, &number_of_meals))
			return (NULL);
		if (dinner_meals_validation(philo, &number_of_meals))
			return (NULL);
		if (write_act_msg(SLEEPING, philo))
			return (NULL);
		if (consult_state_of_simulation(philo))
			usleep(philo->data[0]->time_to_sleep_in_us);
		if (write_act_msg(THINKING, philo))
			return (NULL);
		usleep(200);
	}
	return (NULL);
}
