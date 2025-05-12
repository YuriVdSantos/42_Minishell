#include "minishell.h"

/* Implementações das funções static */
static int	check_consecutive_pipes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (input[i] == '|')
				return (TRUE);
		}
		if (input[i])
			i++;
	}
	return (FALSE);
}

static int	check_redirection_syntax(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			i++;
			while (input[i] && input[i] != '\'' && input[i] != '"')
				i++;
		}
		if (input[i] == '>' || input[i] == '<')
		{
			if (input[i] == input[i + 1])
				i++;
			i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (!input[i] || input[i] == '|' || input[i] == '>' || input[i] == '<')
				return (TRUE);
		}
		if (input[i])
			i++;
	}
	return (FALSE);
}

static int	check_empty_pipes(char *input)
{
	int	i;

	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
		return (TRUE);
	i = ft_strlen(input) - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && input[i] == '|')
		return (TRUE);
	return (FALSE);
}

static int	check_invalid_tokens(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' || input[i] == '\\' || input[i] == '&')
			return (TRUE);
		if ((input[i] == '>' && input[i + 1] == '<') ||
			(input[i] == '<' && input[i + 1] == '>'))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

/* Funções públicas */
int	starts_with_pipe(char *input)
{
	if (input[0] == '|')
		return (syntax_error("|"));
	return (FALSE);
}

int redirect_without_label(char *input)
{
    char    *redirect_position;
    char    next_redirect;

    next_redirect = get_next_redirect(input);
    if (!next_redirect)
        return (FALSE);
        
    redirect_position = get_redirect_position(input, next_redirect);
    if (!redirect_position)
        return (FALSE);
    
    if (redirect_position[0] == '<' && redirect_position[1] == '<')
        redirect_position++;
    else if (redirect_position[0] == '>' && redirect_position[1] == '>')
        redirect_position++;
    redirect_position++;
    
    while (*redirect_position == ' ' || *redirect_position == '\t')
        redirect_position++;
        
    if (*redirect_position == '\0')
        return (syntax_error("newline"));
    if (is_invalid_token(*redirect_position))
        return (unexpected_token(redirect_position));
        
    return (redirect_without_label(redirect_position));
}

int	has_empty_pipe(char *input)
{
	char	*next_pipe;

	next_pipe = get_next_pipe(input);
	if (!next_pipe)
		return (FALSE);
	next_pipe++;
	while (*next_pipe == ' ' || *next_pipe == '\t')
		next_pipe++;
	if (*next_pipe == '|')
		return (syntax_error("|"));
	return (has_empty_pipe(next_pipe));
}

int	is_invalid_syntax(char *input)
{
	if (check_consecutive_pipes(input))
	{
		print_error("syntax error", NULL, "unexpected token `|'");
		return (TRUE);
	}
	if (check_redirection_syntax(input))
	{
		print_error("syntax error", NULL, "unexpected token `newline'");
		return (TRUE);
	}
	if (check_empty_pipes(input))
	{
		print_error("syntax error", NULL, "unexpected token `|'");
		return (TRUE);
	}
	if (check_invalid_tokens(input))
	{
		print_error("syntax error", NULL, "unexpected token");
		return (TRUE);
	}
	return (FALSE);
}