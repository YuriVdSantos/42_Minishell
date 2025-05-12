#include "minishell.h"

int execute_pipeline(t_cmd *cmd, t_env **env)
{
    int prev_pipe[2] = {-1, -1};
    int next_pipe[2];
    pid_t pid;
    int status = 0;
    
    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(next_pipe) == -1)
            {
                perror("pipe");
                return (1);
            }
        }
        
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return (1);
        }
        else if (pid == 0)
        {
            // Child process
            if (prev_pipe[0] != -1)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }
            if (cmd->next)
            {
                dup2(next_pipe[1], STDOUT_FILENO);
                close(next_pipe[0]);
                close(next_pipe[1]);
            }
            
            // Handle redirections for this command
            if (handle_redirections(cmd) != 0)
                exit(1);
            
            if (is_builtin(cmd->args[0]))
                exit(execute_builtin(cmd, env));
            
            char *path = get_cmd_path(cmd->args[0], *env);
            if (!path)
            {
                print_error(cmd->args[0], NULL, "command not found");
                exit(127);
            }
            
            char **env_array = env_to_array(*env);
            execve(path, cmd->args, env_array);
            ft_free_array(env_array);
            free(path);
            print_error(cmd->args[0], NULL, "execve failed");
            exit(126);
        }
        
        // Parent process
        if (prev_pipe[0] != -1)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }
        if (cmd->next)
        {
            prev_pipe[0] = next_pipe[0];
            prev_pipe[1] = next_pipe[1];
        }
        cmd = cmd->next;
    }
    
    // Close any remaining pipes
    if (prev_pipe[0] != -1)
    {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
    
    // Wait for all children
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            set_exit_status(WEXITSTATUS(status));
    }
    
    return (get_exit_status());
}