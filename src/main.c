#include "minishell.h"

int g_exit_status = 0;

int main(int ac, char **av, char **envp)
{
    char    *input;
    t_token *tokens;
    t_cmd   *commands;
    t_env   *env;  // Removi a declaração duplicada abaixo

    (void)ac;
    (void)av;
    
    env = init_env(envp);  // Removi o 't_env *' que estava aqui
    if (!env)
        return (1);
    
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
    
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            ft_putendl_fd("exit", STDERR_FILENO);
            break;
        }
        
        if (*input)
            add_history(input);
        
        tokens = tokenizer_input(input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        
        commands = parse_tokens(tokens);
        free_tokens(tokens);
        free(input);
        
        if (commands)
        {
            g_exit_status = execute(commands, &env);
            free_commands(commands);
        }
    }
    
    free_env(env);
    return (g_exit_status);
}