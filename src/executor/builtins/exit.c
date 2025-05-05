#include "minishell.h"

static int is_valid_exit_number(char *str)
{
    int i = 0;

    if (!str)
        return (0);

    // Permite sinal + ou - no início
    if (str[i] == '+' || str[i] == '-')
        i++;

    // Verifica se todos os caracteres restantes são dígitos
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }

    // Caso especial: apenas "+" ou "-" sem dígitos
    if (i == 1 && (str[0] == '+' || str[0] == '-'))
        return (0);

    return (1);
}

static int contains_quotes(char *str)
{
    return (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
}

int ft_exit(t_cmd *cmd)
{
    int status;
    char *arg;

    if (!cmd->args[1])
        exit(get_exit_status());

    // Verifica argumentos extras
    if (cmd->args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (1);
    }

    arg = cmd->args[1];
    
    // Remove aspas antes de validar
    if (contains_quotes(arg))
    {
        char *unquoted = remove_quotes(arg);
        if (!is_valid_exit_number(unquoted))
        {
            free(unquoted);
            ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
            exit(255);
        }
        status = ft_atoi(unquoted);
        free(unquoted);
    }
    else if (!is_valid_exit_number(arg))
    {
        ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
        exit(255);
    }
    else
    {
        status = ft_atoi(arg);
    }

    exit(status % 256);
}