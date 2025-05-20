/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:13:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/16 17:14:19 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_context	*init_context(char **envp)
{
	t_context	*ctx;

	ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	ctx->env = NULL;
	ctx->allocations = NULL;
	ctx->is_interactive = isatty(STDIN_FILENO);
	if (!load_environment(ctx, envp))
	{
		free(ctx);
		return (NULL);
	}
	return (ctx);
}

int	load_environment(t_context *ctx, char **envp)
{
	int		i;
	t_env	*node;
	char	*eq;

	i = 0;
	while (envp[i] != NULL)
	{
		eq = strchr(envp[i], '=');
		if (!eq)
			continue ;
		node = safe_malloc(ctx, sizeof(t_env));
		if (!node)
			return (0);

		node->key = strndup(envp[i], eq - envp[i]);
		node->value = strdup(eq + 1);
		node->next = ctx->env;
		ctx->env = node;
		i++;
	}
	return (1);
}
