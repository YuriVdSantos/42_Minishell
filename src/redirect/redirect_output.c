#include "minishell.h"

int redirect_output(char *command) 
{
    char *redirect_pos;
    char *filename;
    int output_fd;
    int open_flags;

    redirect_pos = get_redirect_position(command, OUTPUT_REDIRECT_CHAR);
    if (!redirect_pos) {
        return SUCCESS;
    }

    open_flags = (redirect_pos[1] == OUTPUT_REDIRECT_CHAR) ? 
                 APPEND_FLAGS : TRUNCATE_FLAGS;

    filename = get_label_name(redirect_pos);
    if (!filename) {
        return FAILED;
    }

    output_fd = open(filename, open_flags, FILE_PERMISSIONS);
    if (output_fd == -1) {
        print_error_msg("open", filename);
        free(filename);
        return FAILED;
    }

    redirect_fd(output_fd, STDOUT_FILENO);
    free(filename);
    return SUCCESS;
}