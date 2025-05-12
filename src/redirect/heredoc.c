#include "minishell.h"

static char *tmp_filename(int heredoc_number)
{
    char    filename[30];
    char    *number_str;

    ft_bzero(filename, 30);
    number_str = ft_itoa(heredoc_number);
    ft_strlcat(filename, "/tmp/heredoc", 30);
    ft_strlcat(filename, number_str, 30);
    free(number_str);
    return (ft_strdup(filename));
}

void read_heredoc(int *exit_status, t_env *env, char *delimiter, int heredoc_number)
{
    char    *line_read;
    char    *filename;
    int     tmp_file_fd;

    filename = tmp_filename(heredoc_number);
    tmp_file_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    free(filename);
    
    line_read = readline("> ");
    while (line_read && !ft_strcmp(line_read, delimiter))
    {
        line_read = expand_variables(line_read, env, *exit_status);
        ft_putstr_fd(line_read, tmp_file_fd);
        ft_putchar_fd('\n', tmp_file_fd);
        free(line_read);
        line_read = readline("> ");
    }
    
    if (!line_read)
        print_error("warning", delimiter, "heredoc delimited by EOF");
    free(line_read);
    close(tmp_file_fd);
    exit(EXIT_SUCCESS);
}

int exec_heredoc(char *delimiter, int heredoc_number, int *exit_status, t_env *env)
{
    pid_t   pid;

    pid = fork();
    if (pid == -1)
    {
        print_error("fork", NULL, strerror(errno));
        return (FAILURE);
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        read_heredoc(exit_status, env, delimiter, heredoc_number);
    }
    else
    {
        waitpid(pid, exit_status, 0);
        if (WIFEXITED(*exit_status))
            *exit_status = WEXITSTATUS(*exit_status);
        if (*exit_status != EXIT_SUCCESS)
            return (FAILURE);
    }
    return (SUCCESS);
}

char *handle_heredoc(char *delimiter)
{
    char *filename;
    int fd;
    char *line;

    filename = ft_strjoin("/tmp/heredoc_", delimiter);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    while (1) {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
            break;
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    
    close(fd);
    return filename;
}