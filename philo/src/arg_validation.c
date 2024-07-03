/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:23:50 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/03 01:40:48 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (TRUE);
	return (FALSE);
}

static int	arg_err_msg(int type, const char *arg)
{
	char *str_error;

	str_error = "";
	if (type == ('a' + 'c'))
		ft_putstr_fd("Error\n philosophers: invalid number of args\n", STDERR_FILENO);
	else if (type == ('a' + 'v'))
	{
		str_error = format_string("Error\n philosophers: '", arg,
			"': invalid argument, the program receives only digits\n", "");
		ft_putstr_fd(str_error, STDERR_FILENO);
		free(str_error);
	}
	return (EXIT_FAILURE);
}

int	arg_validation(int ac, char **av)
{
	int	matrix_index;
	int	string_index;

	if (ac < 5 || ac > 6)
		return (arg_err_msg(('a' + 'c'), NULL));
	matrix_index = 1;
	while (av[matrix_index])
	{
		string_index = 0;
		if (av[matrix_index][string_index] == '\0')
			return (arg_err_msg(('a' + 'v'), av[matrix_index]));
		while (av[matrix_index][string_index])
		{
			if (!ft_isdigit(av[matrix_index][string_index]))
				return (arg_err_msg(('a' + 'v'), av[matrix_index]));
			string_index++;
		}
		matrix_index++;
	}
	return (EXIT_SUCCESS);
}
