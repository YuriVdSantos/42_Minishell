

#include "tokenizer.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$' || c == '~'); // CRIAR TOKEN PRO TIL
}

/*
# The expected behaviour for de values "$$" in bash is to get 
# the pid of the current process; and when the value "$" is to create a variable
*/
t_token	*variable_token_utils(int *i, int count, int rest)
{
	t_token	*head = NULL;
	t_token	*tail = NULL;
	t_token	*new_node;
	int		j = count * 2 + rest;

	// Cria os tokens para cada "$$"
	while (count >= 1)
	{
		new_node = new_token(TOKEN_VARIABLE, ft_strdup("$$"));
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
		count--;
	}

	// Cria o token para "$" se tiver sobra (ímpar)
	while (rest > 0)
	{
		new_node = new_token(TOKEN_VARIABLE, ft_strdup("$"));
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
		rest--;
	}
	*i += j;
	return (head);
}

t_token	*variable_token_utils_1(char *input, int *i)
{
	int		j;
	char	*word;
	t_token	*new_node;

	j = 0;
	if (input[0] == '$')
		j++;

	while (input[j] && input[j] != ' ' && !is_special_char(input[j]))
		j++;

	word = ft_strndup(&input[1], j - 1);
	new_node = new_token(TOKEN_VARIABLE, word);

	*i += j;
	return (new_node);
}

// void	link_node(t_token **head, t_token **curr, t_token **new_node)
// {
// 	if (!head)
// 	{
// 		head = new_node;
// 		curr = new_node;
// 	}
// 	else
// 	{
// 		(*curr)->next = new_node;
// 		curr = new_node;
// 	}
// }

t_token	*dquote_token(char *input, int *i)
{
	char	*word;
	t_token	*new_node;
	int		start;
	int		j;

	start = *i + 1;
	j = start;
	while (input[j] && input[j] != '\"')
		j++;
	if (!input[j])
		return (NULL);
	word = ft_strndup(&input[start], j - start);
	new_node = new_token(TOKEN_DQUOTE, word);
	*i = j + 1;
	return (new_node);
}

t_token	*quote_token(char *input, int *i)
{
	char	*word;
	t_token	*new_node;
	int		start;
	int		j;

	start = *i + 1;
	j = start;
	while (input[j] && input[j] != '\'')
		j++;
	if (!input[j])
		return (NULL);
	word = ft_strndup(&input[start], j - start);
	new_node = new_token(TOKEN_QUOTE, word);
	*i = j + 1;
	return (new_node);
}


void	free_tokens(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok;
		tok = tok->next;
		free(tmp->value);
		free(tmp);
	}
}