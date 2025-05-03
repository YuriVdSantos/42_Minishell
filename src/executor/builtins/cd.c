#include "minishell.h"

int ft_cd(t_cmd *cmd, t_env **env)
{
    char *path;
    char cwd[PATH_MAX];
    char *oldpwd;

    if (!getcwd(cwd, sizeof(cwd)))
        return (print_error("cd", NULL, "getcwd failed"), 1);
    oldpwd = ft_strdup(cwd);
    
    if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
        path = get_env_value(*env, "HOME");
    else if (ft_strcmp(cmd->args[1], "-") == 0)
        path = get_env_value(*env, "OLDPWD");
    else
        path = cmd->args[1];
    
    if (!path)
        return (free(oldpwd), print_error("cd", NULL, "HOME not set"), 1);
    
    if (chdir(path) != 0)
    {
        free(oldpwd);
        return (print_error("cd", path, strerror(errno)), 1);
    }
    
    set_env_value(env, "OLDPWD", oldpwd);
    free(oldpwd);
    
    if (!getcwd(cwd, sizeof(cwd)))
        return (print_error("cd", NULL, "getcwd failed"), 1);
    set_env_value(env, "PWD", cwd);
    
    return (0);
}