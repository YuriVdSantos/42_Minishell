#include "minishell.h"

static char	*find_var_position(char *input)
{
    while (*input)
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
                if (*input == '$' && is_varname(input[1]))
                    return (input);
                input++;
            }
        }
        else if (*input == '$' && is_varname(input[1]))
            return (input);
        input++;
    }
    return (NULL);
}

static void	update_input(char **input, const char *var_value, const char *second_part)
{
    char	*first_part;
    char	*updated_input;

    first_part = (!*input[0] && !var_value) ? ft_strdup("") :
                 (!*input[0] && var_value) ? ft_strdup(var_value) :
                 (!var_value) ? ft_strdup(*input) :
                 ft_strjoin(*input, var_value);
    updated_input = ft_strjoin(first_part, second_part);
    free(first_part);
    free(*input);
    *input = updated_input;
}

void	expand_variables(char **input, t_env *minienv)
{
    char	*var_position;
    char	*var_name;
    char	*var_value;
    int		name_size;

    while ((var_position = find_var_position(*input)))
    {
        name_size = 0;
        while (is_varname(var_position[name_size + 1]))
            name_size++;
        var_name = ft_substr(var_position, 1, name_size);
        *var_position = '\0';
        var_value = minienv_value(var_name, minienv);
        update_input(input, var_value, var_position + 1 + name_size);
        free(var_name);
    }
}