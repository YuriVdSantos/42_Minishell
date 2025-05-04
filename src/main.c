#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    char    *input;
    t_token *tokens;
    t_cmd   *commands;
    t_env   *env;

    (void)ac;
    (void)av;
    
    env = init_env(envp);
    if (!env)
        return (1);
    
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
    
    while (1)
    {
        input = readline("\001\033[1;32m\002minishell$> \001\033[0m\002");

/*         printf("Input recebido: %s\n", input);

        tokens = tokenizer_input(input);
        printf("Tokens criados? %s\n", tokens ? "Sim" : "Não"); 

        t_cmd *cmd = parse_tokens(tokens);
        printf("Comando parseado? %s\n", cmd ? "Sim" : "Não");

        int status = execute(cmd, &env);
        printf("Status de execução: %d\n", status); */
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
            set_exit_status(execute(commands, &env));
            free_commands(commands);
        }
    }
    
    free_env(env);
    return (get_exit_status());
}