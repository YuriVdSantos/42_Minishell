#include "minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp && envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (!equal_sign)
		{
			i++;
			continue;
		}
		key = ft_strndup(envp[i], equal_sign - envp[i]);
		value = ft_strdup(equal_sign + 1);
		new_node = new_env_node(key, value);
		free(key);
		free(value);
		if (!new_node)
		{
			free_env(head);
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		i++;
	}
	return (head);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*node;

	node = find_env_node(env, key);
	if (!node)
		return (NULL);
	return (node->value);
}

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*node;

	node = find_env_node(*env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(value);
	}
	else
	{
		node = new_env_node(key, value);
		if (!node)
			return ;
		node->next = *env;
		*env = node;
	}
}

int	unset_env_value(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*tmp;
	int		i;
	int		count;

	count = 0;
	for (t_env *e = env; e; e = e->next)
		count++;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	for (t_env *e = env; e; e = e->next)
	{
		tmp = ft_strjoin(e->key, "=");
		tmp = ft_strjoin_free(tmp, e->value);
		array[i++] = tmp;
	}
	array[i] = NULL;
	return (array);
}