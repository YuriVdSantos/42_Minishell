/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:59:09 by jhualves          #+#    #+#             */
/*   Updated: 2025/01/29 05:30:44 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	putptr(unsigned long long ptr)
{
	int		count;
	int		ptr_char;

	count = 0;
	if (ptr >= 16)
		count += putptr(ptr / 16);
	if (ptr % 16 < 10)
		ptr_char = (ptr % 16) + '0';
	else
		ptr_char = ptr % 16 - 10 + 'a';
	count += ft_putchar(ptr_char);
	return (count);
}

int	ft_putptr_fd(void *ptr)
{
	if (ptr == 0)
		return (write(1, "(nil)", 5));
	write(1, "0x", 2);
	return (putptr((unsigned long long)ptr) + 2);
}

static int	puthex(unsigned int hex, char format)
{
	int		count;
	char	hex_char;

	count = 0;
	if (hex >= 16)
		count += puthex(hex / 16, format);
	if (hex % 16 < 10)
		hex_char = (hex % 16) + '0';
	else
	{
		if (format == 'X')
			hex_char = hex % 16 - 10 + 'A';
		else
			hex_char = hex % 16 - 10 + 'a';
	}
	count += ft_putchar(hex_char);
	return (count);
}

int	ft_puthex_fd(unsigned int hex, char format)
{
	return (puthex(hex, format));
}
