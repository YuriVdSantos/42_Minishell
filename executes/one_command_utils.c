#include "minishell.h"

static void	ensure_original_fd_saved(int *original_fd, int std_fd)
{
    if (*original_fd == NO_REDIRECT)
        *original_fd = dup(std_fd);
}

void	save_original_fd_in(int original_fds[2])
{
    ensure_original_fd_saved(&original_fds[IN], STDIN_FILENO);
}

void	save_original_fd_out(int original_fds[2])
{
    ensure_original_fd_saved(&original_fds[OUT], STDOUT_FILENO);
}

int	handle_input_redirect(char *command, int original_fds[2])
{
    save_original_fd_in(original_fds);
    if (redirect_input(command) == FAILED)
    {
        redirect_fd(original_fds[IN], STDIN_FILENO);
        return (FAILED);
    }
    return (SUCCESS);
}

int	handle_output_redirect(char *command, int original_fds[2])
{
    save_original_fd_out(original_fds);
    if (redirect_output(command) == FAILED)
    {
        redirect_fd(original_fds[OUT], STDOUT_FILENO);
        return (FAILED);
    }
    return (SUCCESS);
}