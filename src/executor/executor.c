#include "../redirect/redirect.h"
#include "minishell.h"

static int execute_builtin(t_cmd *cmd, t_env **env)
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

static void close_fds(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO)
        close(cmd->in_fd);
    if (cmd->out_fd != STDOUT_FILENO)
        close(cmd->out_fd);
}

static char *get_cmd_path(char *cmd, t_env *env)
{
    char *path;
    char *path_env;
    char **paths;
    int i;
    struct stat st;

    if (!cmd || !*cmd)
        return (NULL);
    
    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    
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

int execute(t_cmd *cmd, t_env **env)
{
    pid_t pid;
    int status;
    char *path;
    
    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);
    
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, env));
    
    path = get_cmd_path(cmd->args[0], *env);
    if (!path)
    {
        print_error(cmd->args[0], NULL, "command not found");
        return (127);
    }
    
    pid = fork();
    if (pid == -1)
    {
        free(path);
        perror("fork");
        return (1);
    }
    else if (pid == 0)
    {
        if (cmd->in_fd != STDIN_FILENO)
            dup2(cmd->in_fd, STDIN_FILENO);
        if (cmd->out_fd != STDOUT_FILENO)
            dup2(cmd->out_fd, STDOUT_FILENO);
        
        close_fds(cmd);
        
        char **env_array = env_to_array(*env);
        execve(path, cmd->args, env_array);
        ft_free_array(env_array);
        free(path);
        print_error(cmd->args[0], NULL, "execve failed");
        exit(126);
    }
    else
    {
        free(path);
        close_fds(cmd);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
}
