#include "minishell.h"

char *get_heredoc_position(char *str)
{
    while (*str && str[1])
    {
        if (*str == '\'')
        {
            str++;
            while (*str && *str != '\'')
                str++;
        }
        if (*str == '"')
        {
            str++;
            while (*str && *str != '"')
                str++;
        }
        if (*str == '<' && str[1] == '<')
            return ((char *)str);
        str++;
    }
    return (NULL);
}