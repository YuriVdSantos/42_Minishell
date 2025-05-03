#include "minishell.h"

int ft_export(t_cmd *cmd, t_env **env)
{
    char *equal_sign;
    char *key;
    char *value;
    int status = 0;

    if (!cmd->args[1])
        return (ft_env(*env));
    
    for (int i = 1; cmd->args[i]; i++)
    {
        equal_sign = ft_strchr(cmd->args[i], '=');
        if (equal_sign)
        {
            key = ft_strndup(cmd->args[i], equal_sign - cmd->args[i]);
            value = ft_strdup(equal_sign + 1);
            set_env_value(env, key, value);
            free(key);
            free(value);
        }
        else
        {
            if (!find_env_node(*env, cmd->args[i]))
                set_env_value(env, ft_strdup(cmd->args[i]), NULL);
        }
    }
    return (status);
}