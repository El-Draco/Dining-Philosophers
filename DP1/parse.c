/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:44:00 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/20 15:54:16 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	skip_beg(char *str, int *neg)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		*neg *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	return (i);
}

long	ft_atoi(char *str, int *flag)
{
	long	num;
	int		i;
	int		neg;

	num = 0;
	neg = 1;
	i = skip_beg(str, &neg);
	if (str[i] && str[i] != ' ')
	{
		while (str[i] && str[i] != ' ')
		{
			if (!(str[i] >= '0' && str[i] <= '9'))
				*flag = -1;
			num = num * 10 + (str[i++] - '0');
		}
	}
	else
		*flag = -1;
	return (neg * num);
}

int parse_args(int argc, char **argv, t_dna *d)
{
	int	flag;

	flag = 1;
	if (argc < 5 || argc > 6)
		return (-1);
	else if (argc <= 6)
	{
		d->time_to_die = ft_atoi(argv[2], &flag);
		d->time_to_eat = ft_atoi(argv[3], &flag);
		d->time_to_sleep = ft_atoi(argv[4], &flag);
		if (argc == 6)
			d->meals = ft_atoi(argv[5], &flag);
	}
	return flag;
}
