/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 18:09:38 by myokogaw          #+#    #+#             */
/*   Updated: 2024/05/23 18:09:38 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Visão Geral (Overview)

// Quando um filósofo come ele deve dormir logo a seguir, após devolver o garfo a mesa
// e imediatamente ao acordar ele deve pensar;

// Devo dar free em todo o programa e encerrá-lo imediatamente após um filósofo morrer de fome;

// Todo filósofo deve comer e nao pode morrer de fome.

// Devo receber no máximo 5 váriaveis como argumentos, sendo elas números relativos a 
// tempo que serão convertidos a milisegundos, sendo elas, (número de filósofos, tempo para morrer,
// tempo para comer, tempo para dormir e opcionalmente número de vezes que cada filósofo deve comer);
// Milisegundos para Segundos é igual a (segundos * 10^-3) 1ms == 0.001s;

// Caso receba menos que 4 argumentos aponto erro e encerro o programa, caso eu receba algum valor
// como entrada que não sejam numeros encerro o programa e aponto o erro.

// Cada filósofo tem seu ID que varia de 1 à número de filósofos.

// O filósofo de ID 1 sentará-se adjacente ao último filósofo (Filósofo com o ID mais alto).
// Outros filósofos N, sentaram-se entre os N - 1 e N + 1

// No meu programa devo apresentar um logo para o usuário informando o seguinte:
// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

// No qual timestamp_in_ms é referente ao tempo desde o início do programa e X
// é referente ao ID do filósofo.
// Quando estiver sendo printada essas mensagens de log, o programa não deve apresentar nenhum outro print.

// A morte de um filósofo deve ser apresentada no máximo 10ms após a factual morte do mesmo.
// Não se deve forçar a morte de filósofos.

// É proibido corrida de dados no seu program (data races).

// Parte mandatória

// Deve-se resolver o problema, com threads e mutexes.

// Cada filósofo deve ser uma thread.

// Deve-se haver um garfo entre cada par de filósofos. Por tanto, se temos uma vários filósofos, teremos
// Um garfo à direita e um garfo à esquerda;

// Para prevenir a duplicação de garfos, deve-se proteger cada garfo com um mutex.

void	write_err(const char *str)
{
	if (!str)
		return ;
	while (*str)
		str += write(STDERR_FILENO, str, 1);
}

bool	err_msg_params(int type, char *arg)
{
	if (type == ('a' + 'c'))
		write_err("Error\n philosophers: invalid number of args\n");
	if (type == ('a' + 'v'))
	{
		write_err("Error\n philosophers: (");
		write_err(arg);
		write_err("): invalid argument, the program receives only digits\n");
	}
	return (true);
}

bool	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

bool	validation_of_paramters(int ac, char **av)
{
	int	matrix_index;
	int	char_index;

	if (ac < 5 || ac > 6)
		return (err_msg_params(('a' + 'c'), NULL));
	matrix_index = 1;
	while (av[matrix_index])
	{
		char_index = 0;
		while (av[matrix_index][char_index])
		{
			if (!ft_isdigit(av[matrix_index][char_index]))
				return (err_msg_params(('a' + 'v'), av[matrix_index]));
			char_index++;
		}
		matrix_index++;
	}
	return (false);
}

unsigned int	ft_atoui(const char *nptr)
{
	unsigned int	result;

	result = 0;
	while ((*nptr >= '0') && (*nptr <= '9'))
	{
		result *= 10;
		result += *nptr - '0';
		nptr++;
	}
	return (result);
}


// Todos nascem ao mesmo tempo (tempo inicial).
// Cada filósofo deve possuir o seu tempo, tempo da última refeição, tempo da soneca e tempo do pensar.
// Cada filósofo tem um garfo, os quais são compartilhados entre seus outros companheiros de mesa.
// Partiremos do príncipio que o garfo da esquerda pertence a um filósofo sendo assim o garfo da direita pego emprestado.

// O tempo do filósofo é encadeado com a refeição do mesmo, então o fluxo seria, comer->dormir->pensar,
// Desde a última refeição ou início da simulação o filósofo, pode morrer caso o tempo_para_morrer seja ultrapassado neste fluxo;
// O tempo para morrer do filósofo é iniciado no início da simulação e resetado após uma refeição.
t_philo	*instantiation_philo(unsigned int nb_of_philo)
{
	t_philo			*philos;
	unsigned int	index;

	philos = (t_philo *) malloc((nb_of_philo + 1) * sizeof(t_philo));
	if (!philos)
	{
		printf("An error occurred when alloc philos");
		return (NULL);
	}
	philos[nb_of_philo].id = -1;
	while (index < nb_of_philo)
	{
		memset(&philos[index], 0, sizeof(t_philo));
		if (philos[index + 1].id == -1)
		{
			philos[index].id = index + 1;
			philos[index].right_fork = &philos[0].philo_fork;
			break ;
		}
		philos[index].id = index + 1;
		philos[index].right_fork = &philos[index + 1].philo_fork;
		index++;
	}
	return (philos);
}

void	print_matrix_of_philos(t_philo *arr_philos)
{
	int 	i;

	i = 0;
	while (arr_philos[i].id != -1)
	{
		printf("id dos philos %d\n", arr_philos[i].id);
		printf("addr philo fork %p\n", (void *) &arr_philos[i].philo_fork);
		printf("addr right fork %p\n", (void *) arr_philos[i].right_fork);
		i++;
	}
	return ;
}

// number_of_philosophers: A quantidade de filósofos é a mesma quantidade de garfos
// time_to_die (in milliseconds): É o tempo no qual ele ficar sem comer ele morrerá, sendo o tempo contado
// a partir da última refeição ou do início da simulação.
// time_to_eat (in milliseconds): É o tempo no qual os filósofos levam para comer, durante este tempo
// eles devem segurar dois garfos.
// time_to_sleep (in milliseconds): É o tempo que será gasto dormindo.
// number_of_times_each_philosopher_must_eat: Número de vezes que cada filósofo deve comer, completando-a ou algum filósofo 
// morrendo a simulação se encerra.
void	instantiation_struct(int ac, char **av)
{
	struct timeval	tv;
	t_data			data;

	memset(&data, 0, sizeof(t_data));
	data.philos = instantiation_philo(ft_atoui(av[1]));
	print_matrix_of_philos(data.philos);
	data.time_to_die_in_us = (ft_atoui(av[2]) * 1e3);
	data.time_to_eat_in_us = (ft_atoui(av[3]) * 1e3);
	data.time_to_sleep_in_us = (ft_atoui(av[4]) * 1e3);
	if (gettimeofday(&tv, NULL))
		return ;
	data.start_time = (tv.tv_sec * 1e6) + tv.tv_usec;
	if (ac == 6)
		data.nb_of_times_each_philo_must_eat = ft_atoui(av[5]);
	else
		data.nb_of_times_each_philo_must_eat = -1;
	return ;
}

// Como calcular o tempo de execução dos filósofos?
// Pela diferença no tempo perante o valor inical.

int	main(int ac, char **av)
{
	if (validation_of_paramters(ac, av))
		return (1);
	instantiation_struct(ac, av);
	return (0);
}
