/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:56:32 by rriyas            #+#    #+#             */
/*   Updated: 2021/11/13 15:19:37 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	num_size(unsigned long long n, unsigned long long base_len)
{
	unsigned long long	i;

	i = 1;
	while (n >= base_len)
	{
		n /= base_len;
		i++;
	}
	return (i);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnbr_base(long long nbr, char *base)
{
	long	n;
	int		base_len;
	int		neg;

	neg = 0;
	if (nbr < 0)
		neg = 1;
	n = nbr;
	base_len = ft_strlen(base);
	if (nbr < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n < base_len)
		write(1, &base[n], 1);
	if (n >= base_len)
	{
		ft_putnbr_base(n / base_len, base);
		write(1, &base[n % base_len], 1);
	}
	return (num_size(n, base_len) + neg);
}

int	ft_print_mem(unsigned long long nbr, char *base)
{
	unsigned long long	n;

	n = nbr;
	if (nbr == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	if (n < 16)
		write(1, &base[n], 1);
	if (n >= 16)
	{
		ft_putnbr_base(n / 16, base);
		write(1, &base[n % 16], 1);
	}
	return (num_size(nbr, 16));
}
