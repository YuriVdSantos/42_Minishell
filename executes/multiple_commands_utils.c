#include "minishell.h"

int	*init_children_pid(char **commands)
{
    int		*children_pid;
    size_t	num_commands;

    num_commands = arr_len(commands);
    children_pid = ft_calloc(num_commands + 1, sizeof(int));
    if (!children_pid)
        return (NULL);
    return (children_pid);
}

void	clean_after_execute(int *children_pid)
{
    if (children_pid)
    {
        close_extra_fds();
        free(children_pid);
    }
}

void	quit_child(char **commands, t_env **minienv)
{
    if (commands)
        free_array(commands);
    if (minienv)
        free_minienv(minienv);
    close_all_fds();
    rl_clear_history();
    exit(EXIT_FAILURE);
}