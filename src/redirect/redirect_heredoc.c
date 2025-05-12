#include "minishell.h"

static char	*tmp_filename(int heredoc_number)
{
	char	filename[30];
	char	*number_str;

	ft_bzero(filename, 30);
	number_str = ft_itoa(heredoc_number);
	ft_strlcat(filename, "/tmp/heredoc", 30);
	ft_strlcat(filename, number_str, 30);
	free(number_str);
	return (ft_strdup(filename));
}

void	redirect_heredoc(char *command, int heredoc_number)
{
	char	*filename;
	int		tmp_file_fd;
	char	*heredoc_position;

	filename = tmp_filename(heredoc_number);
	if (!filename)
		return ;
	tmp_file_fd = open(filename, O_RDONLY);
	if (tmp_file_fd == -1)
	{
		print_error("open", filename, strerror(errno));
		free(filename);
		return ;
	}
	free(filename);
	redirect_fd(tmp_file_fd, STDIN_FILENO);
	heredoc_position = get_redirect_position(command, heredoc_number);
	if (heredoc_position)
		heredoc_position++;
}