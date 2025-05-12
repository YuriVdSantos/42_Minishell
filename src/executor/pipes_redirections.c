#include "minishell.h"

int setup_redirections(t_cmd *cmd)
{
    if (cmd->in_file) {
        cmd->in_fd = open(cmd->in_file, O_RDONLY);
        if (cmd->in_fd == -1) {
            print_error("open", cmd->in_file, strerror(errno));
            return (1);
        }
        dup2(cmd->in_fd, STDIN_FILENO);
    }
    
    if (cmd->out_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (cmd->append_mode ? O_APPEND : O_TRUNC);
        
        cmd->out_fd = open(cmd->out_file, flags, 0644);
        if (cmd->out_fd == -1) {
            print_error("open", cmd->out_file, strerror(errno));
            return (1);
        }
        dup2(cmd->out_fd, STDOUT_FILENO);
    }
    
    return (0);
}