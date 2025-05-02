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