#include "minishell.h"

static void	print_env_variable(t_env *env_var)
{
    ft_putstr_fd("declare -x ", STDOUT_FILENO);
    ft_putstr_fd(name_only(env_var->key_pair), STDOUT_FILENO);
    if (ft_strchr(env_var->key_pair, '='))
    {
        ft_putstr_fd("=", STDOUT_FILENO);
        ft_putstr_fd("\"", STDOUT_FILENO);
        ft_putstr_fd(value_only(env_var->key_pair), STDOUT_FILENO);
        ft_putstr_fd("\"", STDOUT_FILENO);
    }
    ft_putstr_fd("\n", STDOUT_FILENO);
}

static int	declare_env(t_env *minienv)
{
    t_env	*aux;

    aux = minienv;
    while (aux)
    {
        print_env_variable(aux);
        aux = aux->next;
    }
    return (0);
}

static int	process_argument(char *key_pair, t_env **minienv)
{
    char	*varname;
    int		status;

    varname = name_only(key_pair);
    status = EXIT_SUCCESS;
    if (!is_valid_varname(varname) || str_equal(key_pair, "="))
    {
        print_varname_error_msg("export", key_pair);
        status = EXIT_FAILURE;
    }
    else if (minienv_node(varname, *minienv))
        minienv_update(varname, value_only(key_pair), *minienv);
    else
        list_append(key_pair, minienv);
    free(varname);
    return (status);
}

int	builtin_export(char **args, t_env **minienv)
{
    int	exit_status;

    args++;
    exit_status = EXIT_SUCCESS;
    if (!*args)
        return (declare_env(*minienv));
    while (*args)
    {
        if (process_argument(*args, minienv) == EXIT_FAILURE)
            exit_status = EXIT_FAILURE;
        args++;
    }
    return (exit_status);
}