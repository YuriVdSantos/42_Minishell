#include "minishell.h"

int	is_only_n(const char *str)
{
    int	j;

    j = 2;
    while (str[j])
    {
        if (str[j] != 'n')
            return (0);
        j++;
    }
    return (1);
}

int	echo(char **args)
{
    int	i;
    int	new_line;

    new_line = 1;
    i = 1;

    while (args[i] && ft_strncmp(args[i], "-n", 2) == 0 && is_only_n(args[i]))
    {
        new_line = 0;
        i++;
    }

    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }

    if (new_line)
        ft_putstr_fd("\n", STDOUT_FILENO);

    return (0);
}

