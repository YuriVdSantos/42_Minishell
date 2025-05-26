#include "minishell.h"

static char	*get_target_path(char **args, t_env *minienv)
{
    if (args[1] && !str_equal(args[1], "~"))
        return (args[1]);
    return (minienv_value("__HOME", minienv));
}

static void	update_env_vars(t_env *minienv)
{
    char	cwd[PATH_MAX];
    char	*pwd;

    pwd = minienv_value("PWD", minienv);
    if (pwd && *pwd)
    {
        minienv_update("OLDPWD", pwd, minienv);
        minienv_update("PWD", getcwd(cwd, PATH_MAX), minienv);
    }
}

int	cd(char **args, t_env *minienv)
{
    char	*path;

    if (args[1] && args[2])
        return (cd_error());
    path = get_target_path(args, minienv);
    if (chdir(path) != 0)
    {
        print_perror_msg("cd", path);
        return (EXIT_FAILURE);
    }
    update_env_vars(minienv);
    return (EXIT_SUCCESS);
}

int	cd_error(void)
{
    print_error_msg("cd", "too many arguments");
    return (EXIT_FAILURE);
}