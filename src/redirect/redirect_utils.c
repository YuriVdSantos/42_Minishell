#include "minishell.h"

static void skip_quoted_content(char **str, char quote_char) 
{
    (*str)++; // Skip opening quote
    while (**str && **str != quote_char) {
        (*str)++;
    }
    if (**str) {
        (*str)++; // Skip closing quote if found
    }
}

char *get_redirect_position(char *str, char redirect_char) 
{
    while (*str) {
        if (*str == SINGLE_QUOTE) {
            skip_quoted_content(&str, SINGLE_QUOTE);
            continue;
        }
        if (*str == DOUBLE_QUOTE) {
            skip_quoted_content(&str, DOUBLE_QUOTE);
            continue;
        }
        if (*str == redirect_char) {
            return str;
        }
        str++;
    }
    return NULL;
}

char get_next_redirect(char *str) 
{
    while (*str) {
        if (*str == SINGLE_QUOTE) {
            skip_quoted_content(&str, SINGLE_QUOTE);
            continue;
        }
        if (*str == DOUBLE_QUOTE) {
            skip_quoted_content(&str, DOUBLE_QUOTE);
            continue;
        }
        if (*str == '<' || *str == '>') {
            return *str;
        }
        str++;
    }
    return 0;
}

void redirect_fd(int fd_to_redirect, int fd_location) 
{
    if (fd_to_redirect < 0 || fd_location < 0) {
        return;
    }
    dup2(fd_to_redirect, fd_location);
    close(fd_to_redirect);
}

void redirect_fds(int fd_in, int fd_out) 
{
    if (fd_in != STDIN_FILENO) {
        redirect_fd(fd_in, STDIN_FILENO);
    }
    if (fd_out != STDOUT_FILENO) {
        redirect_fd(fd_out, STDOUT_FILENO);
    }
}

char *get_label_name(char *redirect_position) 
{
    int i = 0;
    int len = 0;

    if (!redirect_position) {
        return NULL;
    }

    // Skip whitespace and redirect characters
    while (redirect_position[i] && ft_isspace(redirect_position[i])) {
        i++;
    }
    while (redirect_position[i] && !ft_isspace(redirect_position[i])) {
        i++;
    }
    while (redirect_position[i] && ft_isspace(redirect_position[i])) {
        i++;
    }

    // Calculate filename length
    while (redirect_position[i + len] && !ft_isspace(redirect_position[i + len])) {
        len++;
    }

    return ft_strndup(redirect_position + i, len);
}