#include "minishell.h"

char	*get_redirect_position(char *str, char redirect_char)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str && *str != '\'')
				str++;
		}
		if (*str == '"')
		{
			str++;
			while (*str && *str != '"')
				str++;
		}
		if (*str == redirect_char)
			return ((char *)str);
		str++;
	}
	return (NULL);
}

char	get_next_redirect(char *str)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str && *str != '\'')
				str++;
		}
		if (*str == '"')
		{
			str++;
			while (*str && *str != '"')
				str++;
		}
		if (*str == '<' || *str == '>')
			return (*str);
		str++;
	}
	return (0);
}

void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

void	redirect_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		redirect_fd(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO)
		redirect_fd(fd_out, STDOUT_FILENO);
}

char	*get_label_name(char *redirect_position)
{
	int		i;
	int		len;
	char	*name;

	if (!redirect_position)
		return (NULL);
	i = 0;
	while (redirect_position[i] && ft_isspace(redirect_position[i]))
		i++;
	while (redirect_position[i] && !ft_isspace(redirect_position[i]))
		i++;
	while (redirect_position[i] && ft_isspace(redirect_position[i]))
		i++;
	len = 0;
	while (redirect_position[i + len] && !ft_isspace(redirect_position[i + len]))
		len++;
	name = ft_strndup(redirect_position + i, len);
	return (name);
}