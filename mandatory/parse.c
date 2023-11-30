/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:44:00 by rriyas            #+#    #+#             */
/*   Updated: 2022/09/23 19:06:40 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	skip_beg(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	return (i);
}

int	ft_atoi(char *str)
{
	long	num;
	int		i;

	num = 0;
	i = skip_beg(str);
	if (i == -1)
		return (-1);
	if (str[i] && str[i] != ' ')
	{
		while (str[i] && str[i] != ' ')
		{
			if (!(str[i] >= '0' && str[i] <= '9'))
				return (-1);
			num = num * 10 + (str[i++] - '0');
		}
	}
	else
		return (-1);
	if (num > 2147483647)
		return (-1);
	return (num);
}

int	valid_args(t_dna *d, int argc)
{
	int	error;

	error = 1;
	if (d->gene_pool <= 0)
		error = -1;
	if (d->time_to_die == -1)
		error = -1;
	if (d->time_to_eat == -1)
		error = -1;
	if (d->time_to_sleep == -1)
		error = -1;
	if (argc == 6 && d->meals == -1)
		error = -1;
	if (error == 1)
		return (1);
	if (d->gene_pool != 0 && error == -1)
		printf("Invalid arguments passed!\n");
	return (-1);
}

int	parse_args(int argc, char **argv, t_dna *d)
{
	if (argc < 5 || argc > 6)
		return (-1);
	else if (argc <= 6)
	{
		d->gene_pool = ft_atoi(argv[1]);
		d->time_to_die = ft_atoi(argv[2]);
		d->time_to_eat = ft_atoi(argv[3]);
		d->time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			d->meals = ft_atoi(argv[5]);
		else
			d->meals = -1;
	}
	return (valid_args(d, argc));
}
