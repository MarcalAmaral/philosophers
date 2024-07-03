/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:21:53 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/03 01:41:22 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	ft_putstr_fd("An error occurred when get current timestamp\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

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
