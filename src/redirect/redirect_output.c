#include "redirect.h"

int	redirect_output(char *command)
{
	char	*output_redirect;
	char	*file_name;
	int		fd;
	int		open_flags;

	output_redirect = get_redirect_position(command, '>');
	if (!output_redirect)
		return (SUCCESS);
	if (output_redirect[1] == '>')
		open_flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	file_name = get_label_name(output_redirect);
	if (!file_name)
		return (FAILED);
	fd = open(file_name, open_flags, 0644);
	if (fd == -1)
	{
		print_error("open", file_name, strerror(errno));
		free(file_name);
		return (FAILED);
	}
	redirect_fd(fd, STDOUT_FILENO);
	free(file_name);
	return (SUCCESS);
}