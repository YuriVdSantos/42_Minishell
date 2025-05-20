#include "minishell.h"

static void initialize_shell(char **envp, t_context *ctx)
{
    ft_memset(ctx, 0, sizeof(t_context)); // Garante que todos os campos são inicializados como 0/NULL
    
    ctx->env = init_env(envp);
    if (!ctx->env)
    {
        ft_putstr_fd("minishell: error initializing environment\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    ctx->is_interactive = isatty(STDIN_FILENO);
    tcgetattr(STDIN_FILENO, &ctx->original_termios);
    setup_interactive_signals(ctx);
}

static int validate_input(t_context *ctx)
{
    if (!ctx->input)  // Verificação adicional para NULL
        return 0;
        
    if (is_empty(ctx->input))
    {
        ctx->exit_status = 0;
        return 0;
    }
    if (has_unclosed_quotes(ctx->input))
    {
        print_error_msg(NULL, "unclosed quotes");
        ctx->exit_status = 2;
        return 0;
    }
    if (is_invalid_syntax(ctx->input))
    {
        ctx->exit_status = 2;
        return 0;
    }
    return 1;
}

static void process_input(t_context *ctx)
{
    if (!validate_input(ctx))
        return;
        
    ctx->tokens = tokenizer_input(ctx->input);
    if (!ctx->tokens)
        return;
        
    ctx->cmd = parser(ctx->tokens, ctx->env);
    if (!ctx->cmd)
        return;
        
    if (ctx->cmd->next)
        ctx->exit_status = execute_pipeline(ctx->cmd, &ctx->env);
    else
        ctx->exit_status = execute_command(ctx->cmd, &ctx->env);
}

static void cleanup_loop(t_context *ctx)
{
    if (ctx->tokens)
    {
        free_tokens(ctx->tokens);
        ctx->tokens = NULL;
    }
    if (ctx->cmd)
    {
        free_cmds(ctx->cmd);
        ctx->cmd = NULL;
    }
    if (ctx->input)
    {
        free(ctx->input);
        ctx->input = NULL;
    }
}

void main_loop(t_context *ctx)
{
    while (!ctx->sigint_received)
    {
        ctx->input = readline("minishell> ");
        if (!ctx->input)
        {
            if (ctx->is_interactive)
                ft_putendl_fd("exit", STDOUT_FILENO);
            break;
        }
        
        if (*(ctx->input))
        {
            add_history(ctx->input);
            process_input(ctx);
        }
        cleanup_loop(ctx);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_context ctx;

    if (argc > 1 && argv)
    {
        print_error_msg("minishell", "no arguments expected");
        return (EXIT_FAILURE);
    }
    
    initialize_shell(envp, &ctx);
    main_loop(&ctx);
    
    // Restaurar configurações do terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &ctx.original_termios);
    free_env(ctx.env);
    return (ctx.exit_status);
}