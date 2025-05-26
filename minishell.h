#ifndef MINISHELL_H
# define MINISHELL_H

/* ====================== */
/*       DEFINES          */
/* ====================== */

// Constantes booleanas e códigos de retorno
# define TRUE      1
# define FALSE     0
# define SUCCESS   1
# define FAILED    0

// Limites do sistema
# define PATH_MAX    4096
# define LLONG_MAX   9223372036854775807
# define NO_REDIRECT -1

/* ====================== */
/*       INCLUDES         */
/* ====================== */
# include "allowed_libs.h"
# include "errors.h"
# include "minienv.h"
# include "builtins.h"
# include "executes.h"

/* ====================== */
/*       TIPOS            */
/* ====================== */

/**
 * @enum e_token_type
 * @brief Tipos de tokens reconhecidos pelo lexer
 */
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_VARIABLE,
    TOKEN_QUOTE,
    TOKEN_DQUOTE,
    TOKEN_EOF
} t_token_type;

/**
 * @struct s_token
 * @brief Estrutura que representa um token
 */
typedef struct s_token {
    t_token_type  type;
    char          *value;
    struct s_token *next;
} t_token;

/* ====================== */
/*    TOKENIZER           */
/* ====================== */
t_token  *tokenizer_input(char *input);
t_token  *process_next_token(char *input, int *i);
t_token  *word_token(char *input, int *i);
t_token  *special_token(char *input, int *i);
t_token  *variable_token(char *input, int *i);
t_token  *handle_dollar_groups(int count, int rest, int *i);
void     handle_operator(char *input, int *i, t_token **new_node);
void     free_tokens(t_token *head);

/* ====================== */
/*    CORE FUNCTIONS      */
/* ====================== */
int      minishell(t_env *minienv);
char     *prompt_input(t_env *minienv);

/* ====================== */
/*    INPUT VALIDATION    */
/* ====================== */
int      has_input_error(char *input, int *exit_status, t_env *minienv);
int      has_unclosed_quotes(char *str);
int      is_invalid_syntax(char *input);
int      is_invalid_token(char c);
int      unexpected_token(char *input);
int      syntax_error(char *token);

/* ====================== */
/*    STRING UTILS        */
/* ====================== */
char     *get_next_pipe(char *str);
char     **split_commands(char *input);
char     **split_args(char *command);
int      str_equal(const char *str1, const char *str2);
int      is_quote(char c);
int      is_empty(char *str);
int      is_name_delimeter(char c);
int      has_pipe(char *str);
void     move_one_forward(char *str);
char     *get_token_position(char *str, char *token);
int      skip_quotes(char *str);

/* ====================== */
/*    MEMORY MANAGEMENT   */
/* ====================== */
void     free_array(char **arr);
int      arr_len(char **arr);

/* ====================== */
/*    SIGNAL HANDLING     */
/* ====================== */
void     define_main_signals(void);
void     define_execute_signals(int child_pid);
void     define_heredoc_signals(int child_pid);

/* ====================== */
/*    EXPANSIONS          */
/* ====================== */
void     handle_expansions(char **input, t_env *minienv, int exit_status);
void     expand_variables(char **input, t_env *minienv);
void     expand_exit_status(char **input, int exit_status);

/* ====================== */
/*    HEREDOC             */
/* ====================== */
int      handle_heredoc(char *input, int *exit_status, t_env *minienv);

/* ====================== */
/*    CONVERSION          */
/* ====================== */
long long ft_atoll(const char *str);

#endif