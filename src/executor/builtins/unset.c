#include "minishell.h"

int ft_unset(t_cmd *cmd, t_env **env)
{
    int status = 0;

    if (!cmd->args[1])
        return (0);
    
    for (int i = 1; cmd->args[i]; i++)
    {
        if (!unset_env_value(env, cmd->args[i]))
            status = 1;
    }
    return (status);
}