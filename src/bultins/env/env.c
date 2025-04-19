#include <unistd.h>
#include <string.h> // Para strlen()

void print_env(char **env) {
    int i = 0;
    while (env[i] != NULL) {
        write(STDOUT_FILENO, env[i], strlen(env[i]));
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
}