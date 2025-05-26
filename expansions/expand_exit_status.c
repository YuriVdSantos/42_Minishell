#include "minishell.h"

static char	*locate_exit_status(char *input)
{
    while (input && *input)
    {
        if (*input == '\'')
        {
            input++;
            while (*input && *input != '\'')
                input++;
        }
        else if (*input == '\"')
        {
            input++;
            while (*input && *input != '\"')
            {
                if (*input == '$' && input[1] == '?')
                    return (input);
                input++;
            }
        }
        else if (*input == '$' && input[1] == '?')
            return (input);
        input++;
    }
    return (NULL);
}

static void	replace_input(char **input, const char *exit_code, const char *remaining_part)
{
    char	*temp_part;
    char	*new_input;

    temp_part = ft_strjoin(*input, exit_code);
    new_input = ft_strjoin(temp_part, remaining_part);
    free(*input);
    free(temp_part);
    *input = new_input;
}

void	expand_exit_status(char **input, int exit_status)
{
    char	*exit_status_pos;
    char	*exit_status_str;

    while ((exit_status_pos = locate_exit_status(*input)))
    {
        *exit_status_pos = '\0';
        exit_status_str = ft_itoa(exit_status);
        replace_input(input, exit_status_str, exit_status_pos + 2);
        free(exit_status_str);
    }
}