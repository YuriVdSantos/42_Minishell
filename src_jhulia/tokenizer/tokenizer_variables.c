/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 22:52:37 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/16 16:07:08 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
# The expected behaviour for de values "$$" in bash is to get 
# the pid of the current process; and when the value "$" is to create a variable
*/
t_token	*variable_token_get_pid(int *index, int num_pairs, int remainder)
{
	t_token	*head;
	int		total_chars;

	head = NULL;
	total_chars = 0;
	if (!create_double_dollar_tokens(&head, num_pairs))
		return (NULL);
	if (!create_single_dollar_token(&head, remainder))
		return (NULL);
	total_chars = (num_pairs * 2) + remainder;
	*index += total_chars;
	return (head);
}

int	create_double_dollar_tokens(t_token **head, int count)
{
	t_token	*new_node;
	t_token	*tail;
	int		i;

	tail = NULL;
	i = 0;
	while (i++ < count)
	{
		new_node = new_token(TOKEN_VARIABLE, ft_strdup("$$"));
		if (!add_token_node(head, &tail, new_node))
			return (0);
	}
	return (1);
}

int	create_single_dollar_token(t_token **head, int remainder)
{
	t_token	*new_node;
	t_token	*tail;

	if (remainder <= 0)
		return (1);
	tail = *head;
	while (tail && tail->next)
		tail = tail->next;
	new_node = new_token(TOKEN_VARIABLE, ft_strdup("$"));
	return (add_token_node(head, &tail, new_node));
}

t_token	*variable_token_env(char *input, int *i)
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
