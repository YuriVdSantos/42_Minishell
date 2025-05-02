#include "minishell.h"

int ft_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            ft_printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return (0);
}