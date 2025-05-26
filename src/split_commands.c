#include "minishell.h"

static void	replace_pipes(char *str, char delimeter)
{
	while (*str)
	{
		if (*str == delimeter)
		{
			str++;
			while (*str && *str != delimeter)
			{
				if (*str == '|')
					*str = -1;
				str++;
			}
			if (*str)
				str++;
		}
		else
			str++;
	}
}

static void	restore_pipes(char **exec_args)
{
	while (*exec_args)
	{
		char *str = *exec_args;
		while (*str)
		{
			if (*str == -1)
				*str = '|';
			str++;
		}
		exec_args++;
	}
}

char	**split_commands(char *input)
{
	char	**commands;

	replace_pipes(input, '"');
	replace_pipes(input, '\'');
	
	commands = ft_split(input, '|');
	
	restore_pipes(commands);
	
	return (commands);
}
