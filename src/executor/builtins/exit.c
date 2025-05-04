#include "minishell.h"

int ft_exit(t_cmd *cmd, t_env **env)
{
    int exit_code;
    
    ft_putendl_fd("exit", STDERR_FILENO);
    
    if (!cmd->args[1])
        exit_code = 0;
    else if (cmd->args[2])
    {
        print_error("exit", NULL, "too many arguments");
        return (1); // Não sai do programa, apenas retorna erro
    }
    else if (!ft_isdigit_str(cmd->args[1]))
    {
        print_error("exit", cmd->args[1], "numeric argument required");
        exit_code = 255;
    }
    else
        exit_code = get_exit_status();
    
    free_env(*env);
    free_commands(cmd);
    
    exit(exit_code);
}