
#include "minishell.h"

int	skip_quotes(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'')
			i++;
	}
	if (str[i] == '"')
	{
		i++;
		while (str[i] != '"')
			i++;
	}
	return (i);
}
