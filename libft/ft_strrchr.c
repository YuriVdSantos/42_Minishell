/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:55:44 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/04 20:33:57 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*temp;
	unsigned char	uc;

	uc = (unsigned char)c;
	temp = NULL;
	while (*s)
	{
		if ((unsigned char)*s == uc)
			temp = s;
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return ((char *)temp);
}
