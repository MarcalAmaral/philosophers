/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:28:46 by myokogaw          #+#    #+#             */
/*   Updated: 2024/07/03 01:33:32 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	err_msg_mutex_init(enum e_error error, long int id)
{
	char	*str_id;
	char	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EAGAIN)
		str_error_type = "): Resource temporarily unavailable\n";
	else if (error == ENOMEM)
		str_error_type = "): Cannot allocate memory\n";
	else if (error == EPERM)
		str_error_type = "): Operation not permitted\n";
	else if (error == EBUSY)
		str_error_type = "): Device or resource busy\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid argument\n";
	str_error = format_string("Occur an error when destroy thread of id: (", str_id, str_error_type, "");
	ft_putstr_fd(str_error, STDERR_FILENO);
	free(str_id);
	free(str_error);
	return ;
}

int	err_msg_mutex_destroy(enum e_error error, long int id)
{
	char	*str_id;
	char	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EBUSY)
		str_error_type = "): Device or resource busy\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid argument\n";
	str_error = format_string("Occur an error when destroy thread of id: (", str_id, str_error_type, "");
	ft_putstr_fd(str_error, STDERR_FILENO);
	free(str_id);
	free(str_error);
	return (NOERROR);
}

int	err_msg_detach_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;
	char 	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EINVAL)
		str_error_type = "): Is not a joinable thread\n";
	else if (error == ESRCH)
		str_error_type = "): No thread with the ID thread could be found\n";
	str_error = format_string("An error occurred when detach thread of id: (", str_id, str_error_type, "");
	ft_putstr_fd(str_error, STDERR_FILENO);
	free(str_id);
	free(str_error);
	deallocating_structures(manager);
	return (EXIT_FAILURE);
}

int	err_msg_join_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;
	char	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == ESRCH)
		str_error_type = "): No thread with the ID thread could be found\n";
	else if (error == EINVAL)
		str_error_type = "): thread is not a joinable thread or another thread is already waiting to join with this thread\n";
	else if (error == EDEADLK)
		str_error_type = "): A deadlock was detected (e.g., two threads tried to join with each other); or thread specifies the calling thread.\n";
	str_error = format_string("An error occurred when join thread of id: (", str_id, str_error_type, "");
	ft_putstr_fd(str_error, STDERR_FILENO);
	free(str_id);
	free(str_error);
	deallocating_structures(manager);
	return (EXIT_FAILURE);
}

int	err_msg_create_thread(enum e_error error, long int id, t_manager *manager)
{
	char	*str_id;
	char 	*str_error;
	char	*str_error_type;

	str_error_type = "";
	str_id = ft_litoa(id);
	if (error == EAGAIN)
		str_error_type = "): Resource temporarily unavailable\n";
	else if (error == EINVAL)
		str_error_type = "): Invalid settings in attr\n";
	else if (error == EPERM)
		str_error_type = "): No permission to set the scheduling \
policy and parameters specified in attr\n";
	str_error = format_string("An error occurred when create thread of id: (", str_id, str_error_type, "");
	ft_putstr_fd(str_error, STDERR_FILENO);
	free(str_id);
	free(str_error);
	deallocating_structures(manager);
	return (EXIT_FAILURE);
}
