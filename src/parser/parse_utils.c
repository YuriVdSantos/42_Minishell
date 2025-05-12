#include "minishell.h"

void add_arg_to_cmd(t_cmd *cmd, char *value)
{
    int i;
    char **new_args;

    if (!cmd || !value)
        return;

    // Conta quantos argumentos já existem
    i = 0;
    if (cmd->args)
        while (cmd->args[i]) i++;

    // Aloca novo array com espaço extra
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;

    // Copia os argumentos existentes
    for (int j = 0; j < i; j++)
        new_args[j] = cmd->args[j];

    // Adiciona o novo argumento
    new_args[i] = ft_strdup(value);
    new_args[i + 1] = NULL;

    // Libera o array antigo e atualiza
    if (cmd->args)
        free(cmd->args);
    cmd->args = new_args;
}