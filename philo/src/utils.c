/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:21:53 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/04 06:49:21 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	write_act_msg_output_and_free(char *s1, char *s2, char *s3)
{
	char	*str_formatted;

	str_formatted = format_string(s1, " ", s2, s3);
	ft_putstr_fd(str_formatted, STDOUT_FILENO);
	free(str_formatted);
	free(s1);
	free(s2);
	return ;
}

int	write_act_msg(enum e_actions action, t_philo *philo)
{
	char	*timestamp_start_of_simulation;
	char	*str_philo_id;
	char	*str_philo_act;

	if (!consult_state_of_simulation(philo))
		return (EXIT_FAILURE);
	str_philo_act = "";
	timestamp_start_of_simulation = ft_litoa(get_current_timestamp(MILLISECONDS)
			- philo->data[0]->timestamp_of_simulation);
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
	write_act_msg_output_and_free(timestamp_start_of_simulation,
		str_philo_id, str_philo_act);
	return (EXIT_SUCCESS);
}

int	is_satisfied(t_manager *manager)
{
	int	index;

	index = 0;
	if (manager->data[0].nb_of_times_each_philo_must_eat != -1)
	{
		while (manager->philos[index].satisfied == TRUE)
			index++;
		if (index == manager->data[0].nb_of_philos)
		{
			pthread_mutex_lock(&manager->data->state_of_simulation_mutex);
			manager->data[0].state_of_simulation = FALSE;
			pthread_mutex_unlock(&manager->data->state_of_simulation_mutex);
			return (TRUE);
		}
		return (FALSE);
	}
	return (FALSE);
}

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
	ft_putstr_fd("An error occurred when get current timestamp\n",
		STDERR_FILENO);
	return (EXIT_FAILURE);
}

char	*format_string(const char *s, const char *s1,
		const char *s2, const char *s3)
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
