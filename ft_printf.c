/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:56:38 by rriyas            #+#    #+#             */
/*   Updated: 2022/07/29 19:28:11 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	call_slave_fn(va_list *ap, int *len, char spec)
{
	if (spec == 'c')
		*len += (ft_putchar(va_arg(*ap, int)));
	else if (spec == 's')
		*len += (ft_putstr(va_arg(*ap, char *)));
	else if (spec == 'd' || spec == 'i')
		*len += (ft_putnbr_base(va_arg(*ap, int), "0123456789"));
	else if (spec == 'u')
		*len += (ft_putnbr_base(va_arg(*ap, unsigned int), "0123456789"));
	else if (spec == 'X')
		*len += (ft_putnbr_base(va_arg(*ap, unsigned int), "0123456789ABCDEF"));
	else if (spec == 'x')
		*len += (ft_putnbr_base(va_arg(*ap, unsigned int), "0123456789abcdef"));
	else if (spec == 'p')
	{
		*len += write(1, "0x", 2);
		*len += (ft_print_mem(va_arg(*ap, unsigned long), "0123456789abcdef"));
	}
	else
		return (0);
	return (1);
}

int	ft_printf(const char *format, ...)
{
	int			i;
	va_list		ap;
	int			len;

	len = 0;
	i = -1;
	va_start(ap, format);
	while (format[++i])
	{
		if (format[i] == '%')
		{
			if (!call_slave_fn(&ap, &len, format[i + 1]))
				len += write(1, &format[i], 1);
			i++;
		}
		else
			len += write(1, &format[i], 1);
	}
	va_end(ap);
	return (len);
}
