#include "minishell.h"

int is_empty(char *input)
{
    while (*input)
    {
        if (!ft_isspace(*input))
            return (FALSE);
        input++;
    }
    return (TRUE);
}

int has_unclosed_quotes(char *input)
{
    char    quote;

    while (*input)
    {
        if (*input == '\'' || *input == '"')
        {
            quote = *input;
            input++;
            while (*input && *input != quote)
                input++;
            if (!*input)
            {
                print_error("syntax error", NULL, "unclosed quotes");
                return (TRUE);
            }
        }
        input++;
    }
    return (FALSE);
}

int has_input_error(char *input, int *exit_status, t_env *env)
{
	(void)env;
    if (is_empty(input))
        return (TRUE);
    if (has_unclosed_quotes(input))
    {
        *exit_status = 2;
        return (TRUE);
    }
    if (is_invalid_syntax(input))  // Usa a função definida em syntax.c
    {
        *exit_status = 2;
        return (TRUE);
    }
	if (exec_heredoc(input, 1, exit_status, env) == FAILURE) //deveria entrar aqui ?
        return (TRUE);
    return (FALSE);
}