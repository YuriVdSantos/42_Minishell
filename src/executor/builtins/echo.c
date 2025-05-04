#include "minishell.h"
#include <unistd.h>
#include <stdbool.h>

void ft_putstr_fd(char *str, int fd)
{
    if (!str)
        return;
    while (*str)
    {
        write(fd, str, 1);
        str++;
    }
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int ft_echo(t_cmd *cmd)
{
    bool newline;
    int i;
    char **args;

    if (!cmd || !cmd->args)
        return (1);

    args = cmd->args;
    newline = true;
    i = 1;
    
    // Verifica a opção -n
    if (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        newline = false;
        i++;
    }

    // Imprime os argumentos
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }

    // Adiciona nova linha se necessário
    if (newline)
        write(STDOUT_FILENO, "\n", 1);
    
    return (0);
}