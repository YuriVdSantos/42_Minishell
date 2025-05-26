/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:54:26 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/05 17:37:23 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	mtz_free(char **mtz, int i)
{
	while (i >= 0)
	{
		free(mtz[i]);
		i--;
	}
	free(mtz);
}

static int	ft_count_split(const char *str, char c)
{
	int	count;

	count = 0;
	while (*str != '\0')
	{
		while (*str == c)
			str++;
		if (*str != c && *str != '\0')
			count++;
		while (*str != c && *str != '\0')
			str++;
	}
	return (count);
}

static char	**ft_fill(char **mtz, char *str, char c)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (str[count])
	{
		while (*str == c && *str != '\0')
			str++;
		while (str[count] != c && str[count] != '\0')
			count++;
		if (count > 0)
		{
			mtz[i] = ft_substr(str, 0, count);
			if (mtz[i] == NULL)
			{
				mtz_free(mtz, i);
				return (NULL);
			}
			i++;
		}
		str = str + count;
		count = 0;
	}
	return (mtz);
}

char	**ft_split(char const *s, char c)
{
	char	**mtz;
	int		count;

	if (!s)
		return (NULL);
	count = ft_count_split(s, c);
	mtz = malloc((count + 1) * sizeof(char *));
	if (mtz == NULL)
		return (NULL);
	mtz = ft_fill(mtz, (char *)s, c);
	mtz[count] = NULL;
	return (mtz);
}
