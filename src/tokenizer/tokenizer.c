#include "tokenizer.h"

/*
# This function creates a linked-list with all types of tokens and it's value
# so we can do the proper verification and parsing.
*/

t_token	*new_token(t_token_type type, char *word)
{
	t_token	*new_token;

	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = word;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenizer_input(char *input)
{
	t_token	*head;
	t_token	*curr;
	t_token	*new_node;
	int		i;

	i = 0;
	head = NULL;
	new_node = NULL;
	curr = NULL;
	while (input[i] != '\0')
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '\'')
			new_node = quote_token(input, &i);
		else if (input[i] == '\"')
			new_node = dquote_token(input, &i);
		else if (is_special_char(input[i]))
			new_node = special_token(input, &i);
		else
			new_node = word_token(input, &i);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			curr = new_node;
		}
		else
		{
			curr->next = new_node;
			curr = new_node;
		}
	}
	// printf("head: %s\n", head->value);
	return (head);
}

/*
# The function word_token takes every part of the input that is a word.
# This function takes the input and the pointer for int i, que é o indice que 
# esta percorrendo o input na função principal e cria um novo token.
# Por fim limpando a variavel word e retornando o novo token criado.
*/
t_token	*word_token(char *input, int *i)
{
	int		j;
	char	*word;
	t_token	*new_node;

	word = input;
	new_node = NULL;
	j = *i;
	input += j;
	while (word[j] && word[j] != ' ' && !is_special_char(word[j]))
		j++;
	word = ft_strndup(input, j - *i);
	new_node = new_token(TOKEN_WORD, word);
	if (!(new_node))
	{
		free(word);
		return (NULL);
	}
	*i = j;
	return (new_node);
}

t_token	*special_token(char *input, int *i)
{
	t_token	*new_node;
	int		j;

	j = *i;
	new_node = NULL;
	if (input[j] == '|' && input[j + 1] != '|')
		new_node = new_token(TOKEN_PIPE, ft_strdup("|"));
	else if (input[j] == '>' && input[j + 1] == '>')
		new_node = new_token(TOKEN_REDIR_APPEND, ">>");
	else if (input[j] == '<' && input[j + 1] == '<')
		new_node = new_token(TOKEN_HEREDOC, "<<");
	else if (input[j] == '>' && input[j + 1] != '>')
		new_node = new_token(TOKEN_REDIR_OUT, ">");
	else if (input[j] == '<' && input[j + 1] != '<')
		new_node = new_token(TOKEN_REDIR_IN, "<");
	else if (input[j] == '$')
		return (variable_token(input, i));
	if (!new_node)
		return (NULL);
	if ((input[j] == '|' && input[j + 1] != '|')
		|| (input[j] == '>' && input[j + 1] != '>')
		|| (input[j] == '<' && input[j + 1] != '<'))
		j++;
	else
		j += 2;
	*i = j;
	return (new_node);
}


t_token	*variable_token(char *input, int *i)
{
	int		j;
	int		rest;
	int		count;
	t_token	*result;

	j = *i;
	count = 0;
	while (input[j] == '$')
	{
		count++;
		j++;
	}

	rest = count % 2;

	if (count > 1)
	{
		count /= 2;
		result = variable_token_utils(i, count, rest);
	}
	else
		result = variable_token_utils_1(&input[*i], i);

	return (result);
}

/*
# TODO
# special_token; tokenizer_input; variable_token_utils(double '$$'); 
*/