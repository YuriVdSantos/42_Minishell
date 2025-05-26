/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:55:17 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/04 14:45:46 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if ((dest == NULL && src == NULL) || n == 0)
		return (dest);
	if (src > dest)
	{
		i = 0;
		while (i < n)
		{
			((unsigned char *) dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else if (n > 0)
	{
		i = n;
		while (i > 0)
		{
			i--;
			((unsigned char *) dest)[i] = ((unsigned char *)src)[i];
		}
	}
	return (dest);
}
