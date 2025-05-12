#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);       
}