/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:54:48 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/05 16:18:01 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

/*int	main(void)
{
	char	c;
    c = 'Q';
    printf("\nppercase alphabet is passed: %d", ft_isalnum(c));

    c = '7';
    printf("\nowercase alphabet is passed: %d", ft_isalnum(c));

    c='+';
    printf("\nnon-alphabetic character is passed: %d", ft_isalnum(c));

    c = ' ';
    printf("\nUppercase alphabet is passed: %d", isalnum(c));

    c = 'T';
    printf("\nLowercase alphabet is passed: %d", isalnum(c));

    c='5';
    printf("\nnon-alphabetic character is passed: %d", isalnum(c));

    return 0;
}*/