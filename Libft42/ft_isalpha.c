/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:54:51 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/05 16:18:07 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (c);
	else
		return (0);
}

/*int	main(void)
{
	unsigned char	c;
    c = 'Q';
    printf("\nuppercase alphabet is passed: %d", ft_isalpha(c));

    c = 'q';
    printf("\nlowercase alphabet is passed: %d", ft_isalpha(c));

    c='+';
    printf("\nnon-alphabetic character is passed: %d", ft_isalpha(c));

    c = 'Q';
    printf("\nuppercase alphabet is passed: %d", isalpha(c));

    c = 'T';
    printf("\nlowercase alphabet is passed: %d", isalpha(c));

    c='+';
    printf("\nnon-alphabetic character is passed: %d", isalpha(c));

    return 0;
}*/
