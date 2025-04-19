#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>
// flags:  -lreadline -lncurses

int main(int ac, char **av, char **env)
{
	if(!env)
		return(1);
	char *input;
	while(ac && av)
	{
		
		input = readline("yuri&Jhulia@minishell> ");
		if(!input)
			break;
		if(strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
	}
	return 0;
}