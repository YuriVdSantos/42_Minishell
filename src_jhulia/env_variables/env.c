/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:49:53 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/16 15:18:09 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*next;

	while (env_list)
	{
		next = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = next;
	}
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
