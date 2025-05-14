#include "minishell.h"

static t_cmd *init_cmd(void)
{
    t_cmd *cmd;
    
    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->in_fd = STDIN_FILENO;
    cmd->out_fd = STDOUT_FILENO;
    cmd->next = NULL;
    return (cmd);
}

static int count_args(t_token *tokens)
{
    int count;
    
    count = 0;
    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_WORD || 
            tokens->type == TOKEN_QUOTE || 
            tokens->type == TOKEN_DQUOTE)
            count++;
        tokens = tokens->next;
    }
    return (count);
}

static void fill_args(t_cmd *cmd, t_token **tokens)
{
    int i;
    int arg_count;
    
    arg_count = count_args(*tokens);
    cmd->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!cmd->args)
        return;
    
    i = 0;
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_WORD || 
            (*tokens)->type == TOKEN_QUOTE || 
            (*tokens)->type == TOKEN_DQUOTE)
        {
            cmd->args[i] = ft_strdup((*tokens)->value);
            i++;
        }
        *tokens = (*tokens)->next;
    }
    cmd->args[i] = NULL;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *head;
    t_cmd *current;
    
    head = NULL;
    while (tokens)
    {
        if (!head)
        {
            head = init_cmd();
            current = head;
        }
        else
        {
            current->next = init_cmd();
            current = current->next;
        }
        if (!current)
        {
            free_commands(head);
            return (NULL);
        }
        fill_args(current, &tokens);
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return (head);
}

void free_commands(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;
    
    while (cmd)
    {
        tmp = cmd;
        cmd = cmd->next;
        if (tmp->args)
        {
            i = 0;
            while (tmp->args[i])
            {
                free(tmp->args[i]);
                i++;
            }
            free(tmp->args);
        }
        if (tmp->in_fd != STDIN_FILENO)
            close(tmp->in_fd);
        if (tmp->out_fd != STDOUT_FILENO)
            close(tmp->out_fd);
        free(tmp);
    }
}

t_cmd *parse_redirection(t_token *tokens)
{
    t_cmd *cmd = init_cmd();
    t_token *current = tokens;
    
    while (current) {
        if (current->type == TOKEN_REDIR_IN) {
            cmd->in_file = current->next->value;
        } else if (current->type == TOKEN_REDIR_OUT) {
            cmd->out_file = current->next->value;
            cmd->append_mode = 0;
        } else if (current->type == TOKEN_REDIR_APPEND) {
            cmd->out_file = current->next->value;
            cmd->append_mode = 1;
        } else if (current->type == TOKEN_HEREDOC) {
            cmd->heredoc = 1;
            cmd->in_file = handle_heredoc(current->next->value);
        } else if (current->type == TOKEN_WORD) {
            add_arg_to_cmd(cmd, current->value);
        }
        current = current->next;
    }
    
    return cmd;
}

t_cmd *parser(t_token *tokens, t_env *env)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    t_token *tmp = tokens;
    int heredoc_count = 1; // Contador para numerar os heredocs

    while (tmp)
    {
        // Inicializa novo comando
        if (!head)
        {
            head = init_cmd();
            current = head;
        }
        else
        {
            current->next = init_cmd();
            current = current->next;
        }
        
        if (!current)
        {
            free_commands(head);
            return (NULL);
        }

        // Processa tokens até encontrar PIPE ou final
        while (tmp && tmp->type != TOKEN_PIPE)
        {
            // Trata redirecionamentos
            if (tmp->type == TOKEN_REDIR_IN || tmp->type == TOKEN_REDIR_OUT || 
                tmp->type == TOKEN_REDIR_APPEND || tmp->type == TOKEN_HEREDOC)
            {
                // Verifica se há arquivo/delimitador após redirecionamento
                if (!tmp->next || tmp->next->type != TOKEN_WORD)
                {
                    print_error("syntax error", NULL, "missing file for redirection");
                    free_commands(head);
                    return (NULL);
                }

                // Processa cada tipo de redirecionamento
                if (tmp->type == TOKEN_REDIR_IN)
                {
                    current->in_file = ft_strdup(tmp->next->value);
                    current->in_redirect = ft_strdup(tmp->next->value);
                }
                else if (tmp->type == TOKEN_REDIR_OUT)
                {
                    current->out_file = ft_strdup(tmp->next->value);
                    current->out_redirect = ft_strdup(tmp->next->value);
                    current->append_mode = 0;
                }
                else if (tmp->type == TOKEN_REDIR_APPEND)
                {
                    current->out_file = ft_strdup(tmp->next->value);
                    current->out_redirect = ft_strdup(tmp->next->value);
                    current->append_mode = 1;
                }
                else if (tmp->type == TOKEN_HEREDOC)
                {
                    current->heredoc = 1;
                    current->heredoc_number = heredoc_count++;
                    current->in_file = ft_strdup(tmp->next->value);
                    
                    // Executa o heredoc imediatamente
                    int exit_status = 0;
                    if (exec_heredoc(tmp->next->value, current->heredoc_number, &exit_status, env) == FAILURE)
                    {
                        free_commands(head);
                        return (NULL);
                    }
                }
                tmp = tmp->next->next;  // Avança dois tokens (redirecionamento + arquivo)
            }
            // Adiciona argumentos normais (comandos e seus parâmetros)
            else if (tmp->type == TOKEN_WORD || tmp->type == TOKEN_QUOTE || 
                    tmp->type == TOKEN_DQUOTE)
            {
                // Remove aspas se necessário e expande variáveis
                char *processed = remove_quotes(tmp->value);
                char *expanded = expand_variables(processed, env, get_exit_status());
                add_arg_to_cmd(current, expanded);
                free(processed);
                free(expanded);
                tmp = tmp->next;
            }
            else
            {
                tmp = tmp->next;
            }
        }

        // Avança além do PIPE se existir
        if (tmp && tmp->type == TOKEN_PIPE)
            tmp = tmp->next;
    }
    
    return (head);
}