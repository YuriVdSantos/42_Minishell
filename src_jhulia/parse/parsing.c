/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:26:05 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/19 15:05:47 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


t_cmd	*parse_tokens(t_token **tokens)
{
	t_context	*ctx;
	t_cmd		*cmd_list;
	t_cmd		*current_cmd;

	ctx = get_context();
	cmd_list = NULL;
	current_cmd = NULL;
	while (*tokens && (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens)->type == TOKEN_PIPE)
		{
			current_cmd->next = create_cmd_node(ctx);
			current_cmd = current_cmd->next;
			(*tokens) = (*tokens)->next;
		}
		else if (is_redirection((*tokens)->type))
			add_redirection(ctx, &current_cmd->redirs, tokens);
		else
		{
			add_arg_to_cmd(ctx, current_cmd, (*tokens)->value);
			(*tokens) = (*tokens)->next;
		}
	}
	return (cmd_list);
}

t_cmd	*create_cmd_node(t_context *ctx)
{
	t_cmd	*cmd;

	cmd = safe_malloc(ctx, sizeof(t_cmd));
	cmd->args = safe_malloc(ctx, sizeof(char *));
	cmd->args[0] = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Adiciona redirecionamento ao comando atual
void	add_redirection(t_context *ctx, t_redir **redirs, t_token **tokens)
{
	t_redir	*new_redir;

	new_redir = safe_malloc(ctx, sizeof(t_redir));
	new_redir->type = (*tokens)->type;
	new_redir->file = safe_strdup(ctx, (*tokens)->next->value);
	new_redir->next = *redirs;
	*redirs = new_redir;
	*tokens = (*tokens)->next->next;
}

// Adiciona argumento ao comando
void	add_arg_to_cmd(t_context *ctx, t_cmd *cmd, char *value)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args = realloc_args(ctx, cmd->args, i + 1);
	cmd->args[i] = safe_strdup(ctx, value);
	cmd->args[i + 1] = NULL;
}
