#include "minishell.h"

int	env(t_env *minienv)
{
    while (minienv)
    {
        if (ft_strchr(minienv->key_pair, '='))
            ft_printf("%s\n", minienv->key_pair);
        minienv = minienv->next;
    }
    return (EXIT_SUCCESS);
}