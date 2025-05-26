#include "minishell.h"

void	free_minienv(t_env **minienv)
{
    t_env	*current;
    t_env	*next_node;

    current = *minienv;
    while (current != NULL)
    {
        next_node = current->next;
        free(current->key_pair);
        free(current);
        current = next_node;
    }
    *minienv = NULL;
}