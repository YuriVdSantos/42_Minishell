#include "minishell.h"

void	handle_pipe(int original_fd_out, char *current_command, char **commands)
{
    static int	pipe_fds[2];
    char		*last_command = commands[arr_len(commands) - 1];
    int			is_first_command = (current_command == commands[0]);
    int			has_next_command = (current_command != last_command);

    if (!is_first_command)
    {
        redirect_fd(pipe_fds[IN], STDIN_FILENO);
        close(pipe_fds[IN]);
    }
    if (has_next_command)
    {
        if (pipe(pipe_fds) == -1)
        {
            print_perror_msg("pipe", current_command);
            return;
        }
        redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
        close(pipe_fds[OUT]);
    }
    else
    {
        redirect_fd(original_fd_out, STDOUT_FILENO);
    }
}