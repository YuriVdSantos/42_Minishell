#include "minishell.h"
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
    char *processed;
    char *expanded;

    if (!cmd || !cmd->args)
    {
        write(STDERR_FILENO, "minishell: echo: missing arguments\n", 34);
        return (1);  // Exit code 1 para erro de sintaxe
    }

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
        // Verificação adicional para argumentos NULL
        if (!args[i])
        {
            write(STDERR_FILENO, "minishell: echo: null argument\n", 31);
            return (1);
        }
        
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }

    // Adiciona nova linha se necessário
    if (newline)
    {
        if (write(STDOUT_FILENO, "\n", 1) == -1)
        {
            write(STDERR_FILENO, "minishell: echo: write error\n", 29);
            return (1);  // Erro ao escrever na saída padrão
        }
    }
    
    return (0);  // Sucesso
}