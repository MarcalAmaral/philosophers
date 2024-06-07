/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 18:02:40 by myokogaw          #+#    #+#             */
/*   Updated: 2024/05/23 18:02:40 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// memset
#include <string.h>

// printf
#include <stdio.h>

// malloc, free
#include <stdlib.h>

// write, usleep
#include <unistd.h>

// gettimeofday
#include <sys/time.h>

// pthread_create, pthread_detach, pthread_join, pthread_mutex_init, 
// pthread_mutex_destroy,  pthread_mutex_lock, pthread_mutex_unlock
#include <pthread.h>

// Types
# include <stdbool.h>

typedef struct s_philo {
	unsigned int		id;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		philo_fork;
	int					is_sleeping;
	unsigned int		last_meal;
	unsigned long int	your_time;
}	t_philo;


// Coloco o tempo para microsegundos pois a função usleep utiliza-se microsegundos.
typedef struct s_data {
	t_philo					*philos;
	unsigned int			time_to_die_in_us;
	unsigned int			time_to_eat_in_us;
	unsigned int			time_to_sleep_in_us;
	unsigned int			nb_of_times_each_philo_must_eat;
	unsigned long long int	start_time;
}   t_data;


#endif