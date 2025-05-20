#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include "../Libft42/libft.h"
# include <errno.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/fcntl.h>
# include <string.h>
#include <termios.h>

# define FAILURE -1
# define TRUE 1
# define FALSE 0
# define SUCCESS 0
# define FAILED 1
# define HEREDOC_FILENAME_PREFIX "/tmp/heredoc"
# define MAX_FILENAME_LENGTH 30
# define INPUT_REDIRECT_CHAR '<'
# define FILE_OPEN_MODE O_RDONLY
# define FILE_FLAGS FD_CLOEXEC
# define OUTPUT_REDIRECT_CHAR '>'
# define FILE_PERMISSIONS 0644
# define APPEND_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
# define TRUNCATE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'
# define CMD_NOT_FOUND 127
# define NOT_EXECUTABLE 126
# define INTERRUPT 128

typedef struct s_string_builder {
    char    *buffer;
    size_t  size;
    size_t  capacity;
    size_t  real_size;
} t_string_builder;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_QUOTE,
    TOKEN_DQUOTE,
    TOKEN_VARIABLE,
}   t_token_type;

typedef struct s_token
{
    t_token_type   type;
    char           *value;
    struct s_token *next;
}   t_token;

typedef struct s_env
{
    char           *key;
    char           *value;
    struct s_env   *next;
}   t_env;

typedef struct s_cmd {
    char **args;
    int in_fd;
    int out_fd;
    char *in_file;       // Para redirecionamento de entrada
    char *out_file;      // Para redirecionamento de saída
    int append_mode;     // Para >>
    int heredoc;         // Para <<
    char *in_redirect;   // Adicionado
    char *out_redirect;  // Adicionado
    int heredoc_number;  // Adicionado
    struct s_cmd *next;  // Para pipelines
} t_cmd;

typedef struct s_context {
    bool            is_interactive;     // Modo interativo ou não
    int             exit_status;        // Último status de saída
    t_env           *env;              // Variáveis de ambiente
    t_token         *tokens;           // Tokens do parser
    t_cmd           *cmd;              // Comandos a executar
    struct termios  original_termios;  // Configurações originais do terminal
    int             heredoc_count;     // Contador de heredocs
    pid_t           child_pid;         // PID do processo filho
    bool            sigint_received;   // Flag para SIGINT
    char            *input;            // Input atual do usuário - ADICIONADO
} t_context;


t_cmd    *parse_tokens(t_token *tokens);
void     free_commands(t_cmd *cmd);
t_token    *tokenizer_input(char *input);
void       free_tokens(t_token *tok);
int        execute(t_cmd *cmd, t_env **env);
int        is_builtin(char *cmd);
int ft_exit(t_cmd *cmd);
int ft_echo(t_cmd *cmd, t_env **env);
int        ft_cd(t_cmd *cmd, t_env **env);
int        ft_pwd(void);
int        ft_export(t_cmd *cmd, t_env **env);
int        ft_unset(t_cmd *cmd, t_env **env);
int        ft_env(t_env *env);
t_env      *init_env(char **envp);
void       free_env(t_env *env);
char       *get_env_value(t_env *env, char *key);
void       set_env_value(t_env **env, char *key, char *value);
int        unset_env_value(t_env **env, char *key);
char       **env_to_array(t_env *env);
t_env      *find_env_node(t_env *env, char *key);
void       	handle_signal(int sig);
void       	ft_free_array(char **array);
char       	*ft_strjoin_free(char *s1, char *s2);
void       	print_error(char *cmd, char *arg, char *error);
int			ft_isdigit_str(const char *str);
int 		get_exit_status(void);
void		set_exit_status(int status);
char    *remove_quotes(char *str);
char    *expand_variables(char *str, t_env *env, int exit_status);
int is_valid_number(const char *str);
int ft_abs(int n);
int     ft_itoa_buf(int n, char *buf);
void *ft_realloc(void *ptr, size_t old_size, size_t new_size);
void    append_string(t_string_builder *sb, const char *str);
void    append_number(t_string_builder *sb, int num);
char    *handle_heredoc(char *delimiter);
int     handle_heredoc_for_input(char *input, int *exit_status, t_env *env);
char    *get_heredoc_position(char *str);
int     exec_heredoc(char *delimiter, int heredoc_number, int *exit_status, t_env *env);
void    read_heredoc(int *exit_status, t_env *env, char *delimiter, int heredoc_number);
int     has_input_error(char *input, int *exit_status, t_env *env);
int     is_empty(char *input);
int     has_unclosed_quotes(char *input);
int     is_invalid_syntax(char *input);
void    define_main_signals(void);
int     is_invalid_syntax(char *input);
int     starts_with_pipe(char *input);
int     redirect_without_label(char *input);
int     has_empty_pipe(char *input);
int     is_invalid_token(char c);
int     unexpected_token(char *input);
int     syntax_error(char *token);
char    *get_next_pipe(char *str);
char	*get_redirect_position(char *str, char redirect_char);
char	get_next_redirect(char *str);
void	redirect_fd(int fd_to_redirect, int fd_location);
void	redirect_fds(int fd_in, int fd_out);
char	*get_label_name(char *redirect_position);
int		redirect_input(char *command);
int		redirect_output(char *command);
void	redirect_heredoc(char *command, int heredoc_number);
void setup_signals(void);
t_token *tokenizer(char *input);
t_cmd *parser(t_token *tokens, t_env *env);
int execute_pipeline(t_cmd *cmds, t_env **env);
int execute_command(t_cmd *cmd, t_env **env);
void free_cmds(t_cmd *cmd);
void add_arg_to_cmd(t_cmd *cmd, char *value);
int setup_redirections(t_cmd *cmd);
void cleanup_redirections(t_cmd *cmd);
void close_fds(t_cmd *cmd);
int handle_redirections(t_cmd *cmd);
int execute_builtin_parent(t_cmd *cmd, t_env **env);
char *get_cmd_path(char *cmd, t_env *env);
void	print_error_msg(char *command, char *msg);
void	move_one_forward(char *str);
void setup_interactive_signals(t_context *ctx);
void define_main_signals(void);
void define_child_signals(int child_pid, bool heredoc);
void setup_interactive_signals(t_context *ctx);
void close_pipe_ends(int in_fd, int pipe_fds[2], int has_next);
int handle_signal_status(int status, int is_last_child);
void define_heredoc_signals(pid_t pid);
void define_execute_signals(int child_pid);
void init_sigaction(struct sigaction *sa, void (*handler)(int), int flags);
int execute_pipeline(t_cmd *cmds, t_env **env);

#endif