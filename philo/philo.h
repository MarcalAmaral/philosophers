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

#define TRUE 1
#define FALSE 0

enum e_actions {
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

enum e_unit_time {
	SECONDS,
	MILLISECONDS,
	MICROSECONDS
};

enum e_error {
	NOERROR,
	EPERM,
	ESRCH = 3,
	EAGAIN = 11,
	ENOMEM = 12,
	EBUSY = 16,
	EINVAL = 22,
	EDEADLK = 36
};

// Coloco o tempo para microsegundos pois a função usleep utiliza-se microsegundos.
typedef struct s_data {
	long int			nb_of_philos;
	_Atomic long int	time_to_die_in_ms;
	_Atomic long int	time_to_eat_in_us;
	_Atomic long int	time_to_sleep_in_us;
	_Atomic long int	nb_of_times_each_philo_must_eat;
	_Atomic long int	timestamp_of_simulation;
	_Atomic long int	state_of_simulation;
}   t_data;

typedef struct s_philo {
	t_data				**data;
	long int			id;
	pthread_t			philo;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		philo_fork;
	_Atomic long int	last_meal;
}	t_philo;

typedef struct s_manager {
	pthread_t	monitor;
	t_philo		*philos;
	t_data		*data;
} t_manager;

#endif