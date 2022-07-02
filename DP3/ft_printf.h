/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 19:36:47 by rriyas            #+#    #+#             */
/*   Updated: 2021/11/13 11:09:10 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(const char *format, ...);
int	ft_strlen(char *str);
int	num_size(unsigned long long n, unsigned long long base_len);
int	ft_putchar(char c);
int	ft_putstr(char *s);
int	ft_putnbr_base(long long nbr, char *base);
int	ft_print_mem(unsigned long long nbr, char *base);
int	call_slave_fn(va_list *ap, int *len, char spec);

#endif
