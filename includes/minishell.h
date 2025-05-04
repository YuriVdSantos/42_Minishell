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
# include <string.h>


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

typedef struct s_cmd
{
    char           **args;
    int            in_fd;
    int            out_fd;
    struct s_cmd   *next;
}   t_cmd;


t_cmd    *parse_tokens(t_token *tokens);
void     free_commands(t_cmd *cmd);

/* Tokenizer */
t_token    *tokenizer_input(char *input);
void       free_tokens(t_token *tok);

/* Parser */
t_cmd *parse_tokens(t_token *tokens);
void free_commands(t_cmd *cmd);

/* Executor */
int        execute(t_cmd *cmd, t_env **env);
int        is_builtin(char *cmd);

/* Builtins */
int        ft_exit(t_cmd *cmd, t_env **env);
int        ft_echo(t_cmd *cmd);
int        ft_cd(t_cmd *cmd, t_env **env);
int        ft_pwd(void);
int        ft_export(t_cmd *cmd, t_env **env);
int        ft_unset(t_cmd *cmd, t_env **env);
int        ft_env(t_env *env);

/* Environment */
t_env      *init_env(char **envp);
void       free_env(t_env *env);
char       *get_env_value(t_env *env, char *key);
void       set_env_value(t_env **env, char *key, char *value);
int        unset_env_value(t_env **env, char *key);
char       **env_to_array(t_env *env);
t_env      *find_env_node(t_env *env, char *key);

/* Utils */
void       	handle_signal(int sig);
void       	ft_free_array(char **array);
char       	*ft_strjoin_free(char *s1, char *s2);
void       	print_error(char *cmd, char *arg, char *error);
int			ft_isdigit_str(const char *str);
int 		get_exit_status(void);
void		set_exit_status(int status);


#endif