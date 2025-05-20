#include "minishell.h"

void define_execute_signals(int child_pid)
{
    struct sigaction sa;

    if (child_pid == 0)
    {
        init_sigaction(&sa, SIG_DFL, 0);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
    }
    else
    {
        init_sigaction(&sa, SIG_IGN, 0);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
    }
}
static void setup_pipe_redirections(int in_fd, int pipe_fds[2], int has_next)
{
    if (in_fd != -1)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (has_next)
    {
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
    }
}

void close_pipe_ends(int in_fd, int pipe_fds[2], int has_next)
{
    if (in_fd != -1)
        close(in_fd);
    if (has_next)
        close(pipe_fds[1]);
}

int handle_signal_status(int status, int is_last_child)
{
    if (WTERMSIG(status) == SIGINT)
    {
        if (is_last_child)
            ft_putstr_fd("\n", STDOUT_FILENO);
        return (128 + SIGINT);
    }
    if (WTERMSIG(status) == SIGQUIT && is_last_child)
    {
        ft_putstr_fd("Quit\n", STDOUT_FILENO);
        return (128 + SIGQUIT);
    }
    return (EXIT_FAILURE);
}

static int handle_input_redirect(t_cmd *cmd)
{
    if (cmd->in_file)
    {
        cmd->in_fd = open(cmd->in_file, O_RDONLY);
        if (cmd->in_fd == -1)
        {
            print_error_msg("open", cmd->in_file);
            return (FAILED);
        }
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
        {
            print_error_msg("dup2", NULL);
            close(cmd->in_fd);
            return (FAILED);
        }
        close(cmd->in_fd);
    }
    return (SUCCESS);
}

static int handle_output_redirect(t_cmd *cmd)
{
    if (cmd->out_file)
    {
        int flags = O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC);
        cmd->out_fd = open(cmd->out_file, flags, 0644);
        if (cmd->out_fd == -1)
        {
            print_error_msg("open", cmd->out_file);
            return (FAILED);
        }
        if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
        {
            print_error_msg("dup2", NULL);
            close(cmd->out_fd);
            return (FAILED);
        }
        close(cmd->out_fd);
    }
    return (SUCCESS);
}

// Função para executar builtins no processo pai
int execute_builtin_parent(t_cmd *cmd, t_env **env)
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

// Função para executar comandos externos
static int execute_external(t_cmd *cmd, t_env *env)
{
    char *path = get_cmd_path(cmd->args[0], env);
    char **env_array = env_to_array(env);
    //int status;

    if (!path)
    {
        print_error_msg(cmd->args[0], "command not found");
        ft_free_array(env_array);
        return (CMD_NOT_FOUND);
    }

    if (execve(path, cmd->args, env_array) == -1)
    {
        print_error_msg("execve", cmd->args[0]);
        free(path);
        ft_free_array(env_array);
        return (NOT_EXECUTABLE);
    }
    free(path);
    ft_free_array(env_array);
    return (EXIT_SUCCESS);
}

// Função para executar um único comando
int execute_command(t_cmd *cmd, t_env **env)
{
    pid_t pid;
    int status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (EXIT_FAILURE);

    // Handle redirections
    if (handle_input_redirect(cmd) != SUCCESS || handle_output_redirect(cmd) != SUCCESS)
        return (EXIT_FAILURE);

    if (is_builtin(cmd->args[0]))
        return (execute_builtin_parent(cmd, env));

    pid = fork();
    if (pid == 0)
    {
        // Child process
        define_execute_signals(0); // Set signals for child
        status = execute_external(cmd, *env);
        exit(status);
    }
    else if (pid > 0)
    {
        // Parent process
        define_execute_signals(pid); // Set signals for parent
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (handle_signal_status(status, TRUE));
    }
    else
    {
        print_error_msg("fork", NULL);
        return (EXIT_FAILURE);
    }
}

// Função para executar pipeline de comandos
int execute_pipeline(t_cmd *cmds, t_env **env)
{
    int pipe_fds[2] = {-1, -1};
    int prev_pipe_in = -1;
    pid_t pid;
    int status = 0;
    t_cmd *current = cmds;

    while (current)
    {
        if (current->next && pipe(pipe_fds) == -1)
        {
            print_error_msg("pipe", NULL);
            return (EXIT_FAILURE);
        }

        pid = fork();
        if (pid == 0)
        {
            // Child process
            define_execute_signals(0);
            setup_pipe_redirections(prev_pipe_in, pipe_fds, current->next != NULL);
            
            if (handle_input_redirect(current) != SUCCESS || 
                handle_output_redirect(current) != SUCCESS)
                exit(EXIT_FAILURE);

            if (is_builtin(current->args[0]))
                exit(execute_builtin_parent(current, env));
            else
                exit(execute_external(current, *env));
        }
        else if (pid < 0)
        {
            print_error_msg("fork", NULL);
            return (EXIT_FAILURE);
        }

        // Parent continues
        close_pipe_ends(prev_pipe_in, pipe_fds, current->next != NULL);
        prev_pipe_in = pipe_fds[0];
        current = current->next;
    }

    // Wait for all children
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            set_exit_status(WEXITSTATUS(status));
        else
            set_exit_status(handle_signal_status(status, TRUE));
    }

    return (get_exit_status());
}

