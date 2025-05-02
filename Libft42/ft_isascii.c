/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:54:55 by jhualves          #+#    #+#             */
/*   Updated: 2024/11/05 16:18:45 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}

/*int	main(void)
{
	unsigned char	c;
    c = 'Q';
    printf("\alphabet is passed: %d", 
    ft_isascii(c));

    c = 128;
    printf("\nlowercase alphabet is passed: %d", 
    ft_isascii(c));

    c='+';
    printf("\nnon-alphabetic character is passed: 
    %d", ft_isascii(c));

    c = 128;
    printf("\nuppercase alphabet is passed: %d", 
    isascii(c));

    c = 5;
    printf("\lowercase alphabet is passed: %d", 
    isascii(c));

  	c = '+';
    printf("\n non-alphabetic character is passed: %d", 
    isascii(c));

    return 0;

}*/