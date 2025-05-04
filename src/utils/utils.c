#include "minishell.h"

void ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char *ft_strjoin_free(char *s1, char *s2)
{
    char *result;

    result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

void print_error(char *cmd, char *arg, char *error)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    if (cmd)
    {
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    if (arg)
    {
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    ft_putendl_fd(error, STDERR_FILENO);
}

int ft_isdigit_str(const char *str)
{
    if (!str || !*str)
        return (0);
    
    while (*str)
    {
        if (!ft_isdigit(*str))
            return (0);
        str++;
    }
    return (1);
}

char *remove_quotes(char *str)
{
    char *result;
    int i = 0;
    int j = 0;
    char quote = 0;

    if (!str)
        return (NULL);
        
    result = malloc(ft_strlen(str) + 1);
    if (!result)
        return (NULL);

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && !quote)
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    
    // Se aspas não foram fechadas, retorna string original
    if (quote)
    {
        free(result);
        return (ft_strdup(str));
    }
    
    return (result);
}

char *expand_variables(char *str, t_env *env, int exit_status)
{
    t_string_builder sb;
    char *var_name;
    char *var_value;
    int i = 0;

    if (!str)
        return (NULL);
        
    init_string_builder(&sb);

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++;
            var_name = extract_var_name(str + i);
            if (!var_name)
                break;
                
            if (ft_strcmp(var_name, "?") == 0)
                append_number(&sb, exit_status);
            else
            {
                var_value = get_env_value(env, var_name);
                if (var_value)
                    append_string(&sb, var_value);
            }
            i += ft_strlen(var_name);
            free(var_name);
        }
        else
            append_char(&sb, str[i++]);
    }
    
    return (sb.buffer);
}

static char *extract_var_name(char *str)
{
    int i = 0;
    
    if (str[i] == '?')
        return (ft_strdup("?"));
        
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
        
    return (ft_strndup(str, i));
}

void init_string_builder(t_string_builder *sb)
{
    sb->buffer = malloc(1);
    sb->buffer[0] = '\0';
    sb->size = 0;
    sb->capacity = 1;
}

void append_char(t_string_builder *sb, char c)
{
    if (sb->size + 1 >= sb->capacity)
    {
        sb->capacity *= 2;
        sb->buffer = ft_realloc(sb->buffer, sb->capacity);
    }
    sb->buffer[sb->size++] = c;
    sb->buffer[sb->size] = '\0';
}

int is_valid_number(const char *str)
{
    int i;

    if (!str || !*str)
        return (0);

    i = 0;
    // Permite sinal + ou - no início
    if (str[i] == '+' || str[i] == '-')
        i++;

    // Verifica se todos os caracteres restantes são dígitos
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }

    // Caso especial: apenas "+" ou "-" sem dígitos
    if (i == 1 && (str[0] == '+' || str[0] == '-'))
        return (0);

    return (1);
}

int ft_abs(int n)
{
    if (n < 0)
        return -n;
    return n;
}