#include "minishell.h"

char	*get_prompt_str(t_env *minienv)
{
	char		*user;
	char		*cwd;
	char		*directory;
	static char	prompt[PATH_MAX];

	user = minienv_value("USER", minienv) ? minienv_value("USER", minienv) : "UNKNOWN";
	cwd = minienv_value("PWD", minienv) ? minienv_value("PWD", minienv) : "/UNKNOWN";

	directory = ft_strrchr(cwd, '/') + 1;
	if (!ft_strncmp(cwd, "/", 2))
		directory = "ROOT";

	ft_bzero(prompt, PATH_MAX);
	ft_strlcat(prompt, user, PATH_MAX);
	ft_strlcat(prompt, "@", PATH_MAX);
	ft_strlcat(prompt, directory, PATH_MAX);
	ft_strlcat(prompt, " % ", PATH_MAX);

	return (prompt);
}

char	*prompt_input(t_env *minienv)
{
	char	*input;

	input = readline(get_prompt_str(minienv));

	if (!input)
		builtin_exit(NULL, &minienv);

	if (input && *input)
		add_history(input);

	return (input);
}
