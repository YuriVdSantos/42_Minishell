#include "minishell.h"

void cleanup_redirections(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO)
        close(cmd->in_fd);
    if (cmd->out_fd != STDOUT_FILENO)
        close(cmd->out_fd);
}

int execute_builtin(t_cmd *cmd, t_env **env)
{
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        return (ft_exit(cmd));
    else if (ft_strcmp(cmd->args[0], "echo") == 0)
        return (ft_echo(cmd, env));
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        return (ft_cd(cmd, env));
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return (ft_pwd());
    else if (ft_strcmp(cmd->args[0], "export") == 0)
        return (ft_export(cmd, env));
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
        return (ft_unset(cmd, env));
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        return (ft_env(*env));
    return (0);
}

void close_fds(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO)
        close(cmd->in_fd);
    if (cmd->out_fd != STDOUT_FILENO)
        close(cmd->out_fd);
}

char *get_cmd_path(char *cmd, t_env *env)
{
    char *path;
    char *path_env;
    char **paths;
    int i;
    struct stat st;

    if (!cmd || !*cmd)
        return (NULL);
    
    // Se o comando já tem um path absoluto ou relativo
    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return (NULL);
    
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    i = -1;
    while (paths[++i])
    {
        path = ft_strjoin(paths[i], "/");
        path = ft_strjoin_free(path, cmd);
        if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            ft_free_array(paths);
            return (path);
        }
        free(path);
    }
    
    ft_free_array(paths);
    return (NULL);
}

int handle_redirections(t_cmd *cmd)
{
    // Handle input redirection
    if (cmd->in_redirect)
    {
        if (cmd->heredoc_number > 0)
            redirect_heredoc(cmd->in_redirect, cmd->heredoc_number);
        else if (redirect_input(cmd->in_redirect) == FAILED)
            return (1);
    }

    // Handle output redirection
    if (cmd->out_redirect)
    {
        if (redirect_output(cmd->out_redirect) == FAILED)
            return (1);
    }

    return (0);
}

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (ft_strcmp(cmd, "exit") == 0 ||
            ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0);
}

int execute_command(t_cmd *cmd, t_env **env)
{
    int status;
    pid_t pid;
    char *path;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);

    // Handle redirections first
    if (setup_redirections(cmd) != 0)
        return (1);

    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, env));

    pid = fork();
    if (pid == 0) {
        // Child process
        path = get_cmd_path(cmd->args[0], *env);
        if (!path) {
            print_error(cmd->args[0], NULL, "command not found");
            exit(127);
        }

        char **env_array = env_to_array(*env);
        execve(path, cmd->args, env_array);
        print_error("execve", cmd->args[0], strerror(errno));
        exit(126);
    } else {
        // Parent process
        waitpid(pid, &status, 0);
        cleanup_redirections(cmd);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
}