#include "minishell.h"

static int exit_status = 0;

int get_exit_status(void)
{
    return exit_status;
}

void set_exit_status(int status)
{
    exit_status = status;
}