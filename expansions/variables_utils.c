#include "minishell.h"

int	is_varname(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_');
}

int	is_valid_varname(char *name)
{
    if (!((*name >= 'a' && *name <= 'z') || (*name >= 'A' && *name <= 'Z')))
        return (FALSE);
    while (*name)
    {
        if (!is_varname(*name))
            return (FALSE);
        name++;
    }
    return (TRUE);
}