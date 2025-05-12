#ifndef REDIRECT_H
# define REDIRECT_H

# include "../minishell.h"

# define SUCCESS 0
# define FAILED 1

/* redirect_utils.c */
char	*get_redirect_position(char *str, char redirect_char);
char	get_next_redirect(char *str);
void	redirect_fd(int fd_to_redirect, int fd_location);
void	redirect_fds(int fd_in, int fd_out);
char	*get_label_name(char *redirect_position);

/* redirect_input.c */
int		redirect_input(char *command);

/* redirect_output.c */
int		redirect_output(char *command);

/* redirect_heredoc.c */
void	redirect_heredoc(char *command, int heredoc_number);
char	*tmp_filename(int heredoc_number);

#endif