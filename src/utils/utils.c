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

#include "minishell.h"

/**
 * Remove aspas de uma string, preservando o conteúdo interno
 * @param str A string a ser processada
 * @return Nova string sem aspas (deve ser liberada pelo caller)
 */
char *remove_quotes(char *str)
{
    int i = 0;
    int j = 0;
    char quote = 0;
    char *result = malloc(ft_strlen(str) + 1);
    
    if (!result)
        return (NULL);

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && !quote)
        {
            quote = str[i]; // Começa uma nova quoted string
        }
        else if (str[i] == quote)
        {
            quote = 0; // Fecha a quoted string
        }
        else
        {
            result[j++] = str[i]; // Copia caracteres normais
        }
        i++;
    }
    result[j] = '\0';
    
    // Caso especial: aspas não fechadas
    if (quote)
    {
        free(result);
        return (ft_strdup(str)); // Retorna cópia original se aspas não forem fechadas
    }
    
    return (result);
}

#include "minishell.h"

/**
 * Expande variáveis de ambiente e especiais ($?, $PWD, etc.)
 * @param str String a ser expandida
 * @param env Lista de variáveis de ambiente
 * @param exit_status Último código de saída
 * @return Nova string expandida (deve ser liberada pelo caller)
 */
char *expand_variables(char *str, t_env *env, int exit_status)
{
    t_string_builder sb;
    char *var_name;
    char *var_value;
    int i = 0;

    if (!str)
        return (NULL);
        
    init_string_builder(&sb); // Inicializa buffer dinâmico

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++;
            // Caso especial: $?
            if (str[i] == '?')
            {
                append_number(&sb, exit_status);
                i++;
            }
            // Extrai nome da variável (letras, números e underscore)
            else if (ft_isalpha(str[i]) || str[i] == '_')
            {
                var_name = extract_var_name(str + i);
                var_value = get_env_value(env, var_name);
                if (var_value)
                    append_string(&sb, var_value);
                i += ft_strlen(var_name);
                free(var_name);
            }
            else
            {
                append_char(&sb, '$');
                append_char(&sb, str[i++]);
            }
        }
        else
        {
            append_char(&sb, str[i++]);
        }
    }
    
    return (sb.buffer);
}

static char *extract_var_name(char *str)
{
    int len = 0;
    
    if (str[len] == '?')
        return (ft_strdup("?"));
        
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
        
    return (ft_strndup(str, len));
}

#include "minishell.h"

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

