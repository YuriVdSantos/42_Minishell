/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:41:46 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/19 15:26:14 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*process_next_token(char *input, int *i)
{
	t_context	*ctx;
	t_token		*new_node;

	ctx = get_context();
	new_node = NULL;
	if (input[*i] == '\'')
		new_node = handle_single_quote(input, i);
	else if (input[*i] == '\"')
		new_node = handle_double_quote(input, i);
	else if (is_special_char(input[*i]))
		new_node = handle_special_char(input, i);
	else
		new_node = handle_word(input, i);

	if (!new_node)
		return (NULL);
	return (new_node);
}

t_token	*tokenizer_input(char *input)
{
	t_context	*ctx;
	t_token		*head;
	t_token		*curr;
	t_token		*new_node;
	int			i;

	ctx = get_context();
	head = NULL;
	curr = NULL;
	i = 0;
	if (!input || !ctx)
		return (NULL);
	while (input[i])
	{
		new_node = process_next_token(input, &i);
		if (!new_node)
			return (free_tokens(ctx, head), NULL);
		if (!head)
			head = new_node;
		else
			curr->next = new_node;
		curr = new_node;
	}
	add_eof_token(ctx, &head);
	return (head);
}

t_token	*word_token(char *input, int *i)
{
	t_context	*ctx;
	int			start;
	char		*word;

	ctx = get_context();
	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i]))
		(*i)++;

	word = safe_strndup(ctx, input + start, *i - start);
	return (new_token(TOKEN_WORD, word));
}

t_token	*special_token(char *input, int *i)
{
	t_token	*new_node;

	new_node = NULL;
	if (input[*i] == '$')
		return (variable_token(input, i));
	handle_operator(input, i, &new_node);
	return (new_node);
}

t_token	*handle_dollar_groups(int count, int rest, int *i)
{
	t_context	*ctx;
	t_token		*result;
	int			j;

	ctx = get_context();
	result = NULL;
	j = 0;
	while (j++ < count)
		add_token(ctx, &result, "$$", TOKEN_VARIABLE);
	if (rest)
		add_token(ctx, &result, "$", TOKEN_VARIABLE);
	*i += (count * 2) + rest;
	return (result);
}

t_token	*variable_token(char *input, int *i)
{
	int	count;
	int	rest;

	count = count_consecutive_chars(input + *i, '$');
	rest = count % 2;
	return (handle_dollar_groups(count / 2, rest, i));
}

void	handle_operator(char *input, int *i, t_token **new_node)
{
	t_token_type	type;

	type = NULL;
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*new_node = new_token(TOKEN_REDIR_APPEND, ">>");
		(*i) += 2;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*new_node = new_token(TOKEN_HEREDOC, "<<");
		(*i) += 2;
	}
	else
	{
		if (input[*i] == '>')
			type = TOKEN_REDIR_OUT;
		else if (input[*i] == '<')
			type = TOKEN_REDIR_IN;
		else
			type = TOKEN_PIPE;
		*new_node = new_token(type, "|");
		(*i)++;
	}
}
