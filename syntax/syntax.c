#include "minishell.h"

static int starts_with_pipe(char *input)
{
    return (input[0] == '|') ? syntax_error("|") : FALSE;
}

static int redirect_without_label(char *input)
{
    char    *redirect_pos;
    char    next_redirect;

    next_redirect = get_next_redirect(input);
    if (!(redirect_pos = get_redirect_position(input, next_redirect)))
        return FALSE;

    if ((redirect_pos[0] == '<' && redirect_pos[1] == '<') ||
        (redirect_pos[0] == '>' && redirect_pos[1] == '>'))
        redirect_pos++;

    redirect_pos++;

    while (*redirect_pos == ' ' || *redirect_pos == '\t')
        redirect_pos++;

    if (*redirect_pos == '\0')
        return syntax_error("newline");
    if (is_invalid_token(*redirect_pos))
        return unexpected_token(redirect_pos);

    return redirect_without_label(redirect_pos);
}

static int has_empty_pipe(char *input)
{
    char    *next_pipe;

    if (!(next_pipe = get_next_pipe(input)))
        return FALSE;

    next_pipe++;

    while (*next_pipe == ' ' || *next_pipe == '\t')
        next_pipe++;

    if (*next_pipe == '|')
        return syntax_error("|");

    return has_empty_pipe(next_pipe);
}

int is_invalid_syntax(char *input)
{
    return starts_with_pipe(input) || 
           redirect_without_label(input) || 
           has_empty_pipe(input);
}