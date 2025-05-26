#include "minishell.h"

static void	construct_path(char *destination, const char *part1, const char *part2, const char *part3)
{
    destination[0] = '\0';
    ft_strlcat(destination, part1, PATH_MAX);
    ft_strlcat(destination, part2, PATH_MAX);
    ft_strlcat(destination, part3, PATH_MAX);
}

static int	is_absolute_or_relative_path(const char *command)
{
    return (command[0] == '/' || 
            (command[0] == '.' && command[1] == '/') || 
            (command[0] == '.' && command[1] == '.' && command[2] == '/'));
}

static char	*resolve_local_path(const char *command, t_env *minienv)
{
    char	full_path[PATH_MAX];

    if (command[0] == '/')
        return (ft_strdup(command));
    construct_path(full_path, minienv_value("PWD", minienv), "/", command);
    return (ft_strdup(full_path));
}

static char	*search_in_path_env(char *command, char *path_env)
{
    char	**paths;
    char	**paths_start;
    char	current_path[PATH_MAX];

    paths = ft_split(path_env, ':');
    paths_start = paths;
    while (paths && *paths)
    {
        construct_path(current_path, *paths, "/", command);
        if (access(current_path, F_OK) == 0)
        {
            free_array(paths_start);
            return (ft_strdup(current_path));
        }
        paths++;
    }
    free_array(paths_start);
    return (NULL);
}

char	*get_path(char *command, t_env *minienv)
{
    char	*path_env;

    if (is_absolute_or_relative_path(command))
        return (resolve_local_path(command, minienv));
    path_env = minienv_value("PATH", minienv);
    if (!path_env)
        return (NULL);
    return (search_in_path_env(command, path_env));
}