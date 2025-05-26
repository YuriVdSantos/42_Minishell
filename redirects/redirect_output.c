#include "minishell.h"

int	redirect_output(char *command)
{
    char	*output_redirect;
    char	*file_name;
    int		fd;
    int		open_flags;

    output_redirect = get_redirect_position(command, '>');
    if (!output_redirect)
        return (SUCCESS);
    open_flags = (output_redirect[1] == '>') ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC);
    file_name = get_label_name(output_redirect);
    fd = open(file_name, open_flags, 0644);
    if (fd == -1)
    {
        print_perror_msg("open", file_name);
        free(file_name);
        return (FAILED);
    }
    redirect_fd(fd, STDOUT_FILENO);
    free(file_name);
    return (SUCCESS);
}