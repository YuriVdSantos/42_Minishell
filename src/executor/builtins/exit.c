#include "minishell.h"

int ft_exit(t_cmd *cmd) 
{
    if (!cmd->args[1])
        exit(get_exit_status());
    
    if (cmd->args[2]) {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return 1;
    }
    if (!is_valid_number(cmd->args[1])) {
        ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
        exit(255);
    }

    int status = ft_atoi(cmd->args[1]);
    exit(status % 256); // Garante valor entre 0-255
}