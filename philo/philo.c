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
