#include "minishell.h"

int execute(t_cmd *cmd, t_env **env)
{
    pid_t pid;
    int status;
    
    if (!cmd)
        return (1);
    
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, env));
    
    pid = fork();
    if (pid == -1)
    {
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
        
        execve(get_cmd_path(cmd->args[0], *env), cmd->args, env_to_array(*env));
        print_error(cmd->args[0], NULL, "command not found");
        exit(127);
    }
    else
    {
        close_fds(cmd);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
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