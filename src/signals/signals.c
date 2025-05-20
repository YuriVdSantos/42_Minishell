#include "minishell.h"

static void handle_sigint(int sig)
{
    (void)sig;
    ft_putstr_fd("\n", STDOUT_FILENO);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void init_sigaction(struct sigaction *sa, void (*handler)(int), int flags)
{
    sa->sa_handler = handler;
    sa->sa_flags = flags;
    sigemptyset(&sa->sa_mask);
}

void define_main_signals(void)
{
    struct sigaction sa;

    init_sigaction(&sa, &handle_sigint, 0);
    sigaction(SIGINT, &sa, NULL);
    
    init_sigaction(&sa, SIG_IGN, 0);
    sigaction(SIGQUIT, &sa, NULL);
}

void define_child_signals(int child_pid, bool heredoc)
{
    struct sigaction sa;

    init_sigaction(&sa, (child_pid == 0) ? SIG_DFL : SIG_IGN, 0);
    sigaction(SIGINT, &sa, NULL);
    
    init_sigaction(&sa, heredoc ? SIG_IGN : ((child_pid == 0) ? SIG_DFL : SIG_IGN), 0);
    sigaction(SIGQUIT, &sa, NULL);
}

static void exit_shell(int sig)
{
    (void)sig;
    exit(EXIT_FAILURE);
}

void setup_interactive_signals(t_context *ctx)
{
    struct sigaction sa;

    if (ctx->is_interactive) {
        define_main_signals();
    } else {
        init_sigaction(&sa, exit_shell, 0);
        sigaction(SIGINT, &sa, NULL);
        
        init_sigaction(&sa, SIG_IGN, 0);
        sigaction(SIGQUIT, &sa, NULL);
    }
}