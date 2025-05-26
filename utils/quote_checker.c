

#include "minishell.h"

static int	print_error(void)
{
	ft_putstr_fd("You forgot to close some quotes 🌝\n", STDERR_FILENO);
	return (1);
}

int	has_unclosed_quotes(char *str)
{
	char	last_opened;

	last_opened = 0;
	while (*str && !last_opened)
	{
		if (*str == '\'' || *str == '"')
			last_opened = *str;
		str++;
	}
	while (*str && last_opened)
	{
		if (*str && *str == last_opened)
			last_opened = 0;
		str++;
	}
	if (*str)
		return (has_unclosed_quotes(str));
	else if (!last_opened)
		return (0);
	else
		return (print_error());
}
