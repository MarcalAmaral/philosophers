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
# include <string.h>

// printf
# include <stdio.h>

// malloc, free
# include <stdlib.h>

// write, usleep
# include <unistd.h>

// gettimeofday
# include <sys/time.h>

// pthread_create, pthread_detach, pthread_join, pthread_mutex_init, 
// pthread_mutex_destroy,  pthread_mutex_lock, pthread_mutex_unlock
# include <pthread.h>

# define TRUE 1
# define FALSE 0

# define MAX_PHILOS 200

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

enum e_procedures {
	CREATE,
	JOIN
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

typedef struct s_data {
	long int			nb_of_philos;
	long int			state_of_simulation;
	pthread_mutex_t		state_of_simulation_mutex;
	_Atomic long int	time_to_die_in_ms;
	_Atomic long int	time_to_eat_in_us;
	_Atomic long int	time_to_sleep_in_us;
	_Atomic long int	nb_of_times_each_philo_must_eat;
	_Atomic long int	timestamp_of_simulation;
}	t_data;

typedef struct s_philo {
	t_data				**data;
	long int			id;
	pthread_t			philo;
	_Atomic int			satisfied;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		philo_fork;
	_Atomic long int	last_meal;
}	t_philo;

typedef struct s_manager {
	pthread_t	monitor;
	t_philo		*philos;
	t_data		*data;
}	t_manager;

// Argument validation
int			arg_validation(int ac, char **av);

// Instatiation and deallocating structures
int			instantiation_manager_struct(char **av, t_manager *manager);
void		deallocating_structures(t_manager *manager);

// Error messages
int			err_msg_mutex_destroy(enum e_error error, long int id);
int			err_msg_join_thread(enum e_error error, long int id,
				t_manager *manager);
int			err_msg_create_thread(enum e_error error, long int id,
				t_manager *manager);
void		err_msg_mutex_init(enum e_error error, long int id);

// Libft functions
int			ft_strlen(const char *string);
char		*ft_litoa(long int num);
void		ft_putstr_fd(const char *string, int fd);
long int	ft_atoli(const char *nptr);

// Utilities
int			is_satisfied(t_manager *manager);
int			write_act_msg(enum e_actions action, t_philo *philo);
int			consult_state_of_simulation(t_philo *philo);
char		*format_string(const char *s, const char *s1,
				const char *s2, const char *s3);
long int	get_current_timestamp(enum e_unit_time unit);

// Dinner simulation
void		*dinner_routine(void *arg);
void		*monitor_routine(void *arg);
int			start_simulation(t_manager *manager);

// Eat actions and validations
void		pick_up_first_fork(t_philo *philo);
void		pick_up_second_fork(t_philo *philo);
int			first_validation_before_dinner(t_philo *philo);
int			second_validation_before_dinner(t_philo *philo);
int			validation_before_eating(t_philo *philo, int *number_of_meals);

#endif
