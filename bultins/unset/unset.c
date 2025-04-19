#include <string.h> 

void unset_var(char **env, const char *var_name) {
    if (env == NULL || var_name == NULL) return;

    char **current = env;
    while (*current != NULL) {
        if (ft_strncmp(*current, var_name, ft_strlen(var_name)) == 0 && 
            (*current)[ft_strlen(var_name)] == '=') {
            
            char **next = current + 1;
            while (*next != NULL) {
                *current = *next;
                current++;
                next++;
            }
            *current = NULL;
            break;
        }
        current++;
    }
}