/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:55:42 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/04 15:06:57 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (big[i] != 0 && i < len)
	{
		if (big[i] == little[0])
		{
			j = 0;
			while (little[j] != '\0' && big[i + j] == little[j]
				&& (i + j) < len)
				j++;
			if (little[j] == '\0')
				return ((char *)(big + i));
		}
		i++;
	}
	return (NULL);
}
