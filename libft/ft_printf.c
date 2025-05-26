/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:08:20 by jhualves          #+#    #+#             */
/*   Updated: 2025/02/02 22:17:52 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	handle_format(const char *format, va_list args, int *count)
{
	if (*format == 'd' || *format == 'i')
		(*count) += ft_putnbr (va_arg(args, int));
	else if (*format == 'u')
		(*count) += ft_putunbr_fd (va_arg(args, unsigned int));
	else if (*format == 'x' || *format == 'X')
		(*count) += ft_puthex_fd (va_arg(args, unsigned int), *format);
	else if (*format == 's')
		(*count) += ft_putstr (va_arg(args, char *));
	else if (*format == 'c')
		(*count) += ft_putchar (va_arg(args, int));
	else if (*format == 'p')
		(*count) += ft_putptr_fd (va_arg(args, void *));
	else if (*format == '%')
		(*count) += ft_putchar('%');
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	args;

	count = 0;
	if (!*format)
		return (-1);
	va_start(args, format);
	while (*format != '\0')
	{
		if (*format == '%' && *(format + 1) != '\0')
		{
			format++;
			handle_format(format, args, &count);
		}
		else
		{
			count += ft_putchar(*format);
		}
		format++;
	}
	va_end(args);
	return (count);
}

int	ft_putunbr_fd(unsigned int n)
{
	char	c;
	int		count;

	count = 0;
	if (n == 0)
		return (write(1, "0", 1));
	if (n >= 10)
		count += ft_putunbr_fd(n / 10);
	c = (n % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}

int	ft_putstr(char *s)
{
	int	count;

	count = 0;
	if (!s)
		return (write (1, "(null)", 6));
	while (*s)
	{
		count += write(1, s, 1);
		s++;
	}
	return (count);
}

int	ft_putnbr(int n)
{
	char	c;
	int		count;

	count = 0;
	if (n == -2147483648)
	{
		return (write(1, "-2147483648", 11));
	}
	if (n == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	if (n < 0)
	{
		count += write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
		count += ft_putnbr(n / 10);
	c = (n % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}
