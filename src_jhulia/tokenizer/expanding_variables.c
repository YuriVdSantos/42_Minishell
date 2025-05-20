/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:20:31 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/18 20:15:07 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*handle_even_dollars(int count, char *value, t_context *ctx)
{
	char	*result;

	result = safe_strdup(ctx, "");
	while (count-- > 0)
		result = ft_strjoin_free(result, safe_strdup(ctx, "$$"));
	return (result);
}

char	*expand_variable(char *value, t_context *ctx)
{
	int		count;
	int		rest;

	count = count_dollars(value);
	rest = count % 2;
	if (count > 1)
		return (expand_multi_variable(value, count / 2, rest, ctx));
	else
		return (expand_one_variable(value, ctx));
}

static char	*find_env_value(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strcmp(env->key, var) == 0)
			return (safe_strdup(get_context(), env->value));
		env = env->next;
	}
	return (safe_strdup(get_context(), ""));
}

char	*expand_one_variable(char *value, t_context *ctx)
{
	char	*var;
	char	*result;

	if (value[0] == '$' && value[1] == ' ')
		return (safe_strdup(ctx, "$"));
	var = extract_var_name(value);
	result = find_env_value(ctx->env, var);
	free(var);
	return (result);
}

char	*expand_multi_variable(char *value, int count, int rest, t_context *ctx)
{
	char	*result;
	char	*temp;

	result = safe_strdup(ctx, "");
	while (count-- > 0)
	{
		temp = safe_strdup(ctx, get_pid());
		result = ft_strjoin_free(result, temp);
	}
	if (rest)
	{
		temp = expand_one_variable(value + 1, ctx);
		result = ft_strjoin_free(result, temp);
	}
	return (result);
}
