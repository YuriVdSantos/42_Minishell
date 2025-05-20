#include "minishell.h"

int redirect_input(char *command) 
{
    char *redirect_pos;
    char *filename;
    int input_fd;

    redirect_pos = get_redirect_position(command, INPUT_REDIRECT_CHAR);
    if (!redirect_pos) {
        return SUCCESS;
    }

    filename = get_label_name(redirect_pos);
    if (!filename) {
        return FAILED;
    }

    input_fd = open(filename, FILE_OPEN_MODE, FILE_FLAGS);
    if (input_fd == -1) {
        print_error_msg("open", filename);
        free(filename);
        return FAILED;
    }

    redirect_fd(input_fd, STDIN_FILENO);
    free(filename);
    return SUCCESS;
}