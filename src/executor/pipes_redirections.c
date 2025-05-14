#include "minishell.h"

int setup_redirections(t_cmd *cmd)
{
    if (cmd->in_file) {
        cmd->in_fd = open(cmd->in_file, O_RDONLY);
        if (cmd->in_fd == -1) {
            print_error(cmd->args[0], cmd->in_file, strerror(errno));
            return (1);
        }
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            close(cmd->in_fd);
            return (1);
        }
        close(cmd->in_fd);
    }
    
    if (cmd->out_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (cmd->append_mode ? O_APPEND : O_TRUNC);
        
        cmd->out_fd = open(cmd->out_file, flags, 0644);
        if (cmd->out_fd == -1) {
            print_error(cmd->args[0], cmd->out_file, strerror(errno));
            return (1);
        }
        if (dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            close(cmd->out_fd);
            return (1);
        }
        close(cmd->out_fd);
    }
    
    return (0);
}