#include "minishell.h"
#include <stdbool.h>
#include <unistd.h>

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int ft_echo(t_cmd *cmd, t_env **env)
{
    bool newline;
    int i;
    char *processed;
    char *expanded;

    if (!cmd || !cmd->args)
    {
        write(STDERR_FILENO, "minishell: echo: missing arguments\n", 34);
        return (1);
    }

    newline = true;
    i = 1;
    
    // Verifica a opção -n
    if (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
    {
        newline = false;
        i++;
    }

    // Imprime os argumentos
    while (cmd->args[i])
    {
        processed = remove_quotes(cmd->args[i]);
        if (!processed)
        {
            write(STDERR_FILENO, "minishell: echo: memory error\n", 29);
            return (1);
        }
        
        expanded = expand_variables(processed, *env, get_exit_status());
        free(processed);
        
        if (!expanded)
        {
            write(STDERR_FILENO, "minishell: echo: memory error\n", 29);
            return (1);
        }
        
        ft_putstr_fd(expanded, STDOUT_FILENO);
        free(expanded);
        
        if (cmd->args[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }

    // Adiciona nova linha se necessário
    if (newline)
    {
        if (write(STDOUT_FILENO, "\n", 1) == -1)
        {
            write(STDERR_FILENO, "minishell: echo: write error\n", 29);
            return (1);
        }
    }
    
    return (0);
}