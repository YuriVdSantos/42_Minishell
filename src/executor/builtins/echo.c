#include "echo.h"
#include <unistd.h>
#include <stdbool.h>

void	ft_putstr_fd(char *str, int fd)
{
	while(*str)
	{
		write(fd, str, 1);
		str++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while(*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_echo(char **args)
{
	bool	newline;
	int		i;

	newline = true;
	i = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = false;
		i++;
	}

	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}

	if(newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);

}