#include "minishell.h"

static char	*generate_heredoc_filename(int heredoc_number)
{
	char	filename[MAX_FILENAME_LENGTH];
	char	*number_str;

	ft_bzero(filename, MAX_FILENAME_LENGTH);
	number_str = ft_itoa(heredoc_number);
	ft_strlcat(filename, HEREDOC_FILENAME_PREFIX, MAX_FILENAME_LENGTH);
	ft_strlcat(filename, number_str, MAX_FILENAME_LENGTH);
	free(number_str);
	return (ft_strdup(filename));
}

void	redirect_heredoc(char *command, int heredoc_number)
{
	char	*filename;
	int		tmp_file_fd;

	filename = generate_heredoc_filename(heredoc_number);
	if (!filename)
		return;
		
	tmp_file_fd = open(filename, O_RDONLY);
	free(filename);
	
	if (tmp_file_fd == -1)
	{
		print_error_msg("open", filename);
		return;
	}
	
	redirect_fd(tmp_file_fd, STDIN_FILENO);
	move_one_forward(get_redirect_position(command, heredoc_number));
}