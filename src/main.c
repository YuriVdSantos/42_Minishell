#include "minishell.h"

static void	initialize_shell(char **envp, t_env **env)
{
	*env = init_env(envp);
	if (!*env)
	{
		ft_putstr_fd("minishell: error initializing environment\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	setup_signals();
}

void	main_loop(t_env *env)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (*input)
			add_history(input);
        tokens = tokenizer_input(input);
		cmds = parser(tokens);
		if (has_input_error(input, &status, env))
			continue ;
		if (cmds->next)
			status = execute_pipeline(cmds, &env);
		else
			status = execute_command(cmds, &env);
		set_exit_status(status);
		free_cmds(cmds);
		free_tokens(tokens);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	initialize_shell(envp, &env);
	main_loop(env);
	free_env(env);
	return (get_exit_status());
}