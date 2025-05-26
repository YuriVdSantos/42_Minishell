#include "minishell.h"

static int	is_signal(int status, int signal)
{
    return (WTERMSIG(status) == signal);
}

static void	handle_signal_output(int status, int is_last_child)
{
    if (is_signal(status, SIGINT))
        ft_putstr_fd("\n", STDOUT_FILENO);
    else if (is_signal(status, SIGQUIT) && is_last_child)
        ft_putstr_fd("Quit\n", STDOUT_FILENO);
}

static int	handle_signal_interrupt(int status, int is_last_child)
{
    handle_signal_output(status, is_last_child);
    return (INTERRUPT + WTERMSIG(status));
}

int	wait_for_child(int child_pid, int is_last_child)
{
    int	status;

    if (child_pid == FORK_ERROR)
        return (EXIT_FAILURE);
    if (waitpid(child_pid, &status, 0) == -1)
    {
        char *pid_str = ft_itoa(child_pid);
        print_perror_msg("waitpid", pid_str);
        free(pid_str);
        return (EXIT_FAILURE);
    }
    if (WIFSIGNALED(status))
        return (handle_signal_interrupt(status, is_last_child));
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (EXIT_FAILURE);
}

int	wait_for_children(int children_pid[1024])
{
    int	i;
    int	exit_status;

    i = 0;
    exit_status = 0;
    while (children_pid[i] != 0)
    {
        int is_last_child = (children_pid[i + 1] == 0);
        exit_status = wait_for_child(children_pid[i], is_last_child);
        i++;
    }
    clean_after_execute(children_pid);
    return (exit_status);
}