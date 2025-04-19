#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;  // Acesso direto ao ambiente

// Adiciona/atualiza uma variável no ambiente
void export_var(const char *name_value) {
    // Encontra o '=' para separar nome e valor
    const char *equal = strchr(name_value, '=');
    if (!equal) {
        const char *err = "Formato deve ser VAR=valor\n";
        write(STDERR_FILENO, err, strlen(err));
        return;
    }

    // Calcula tamanhos
    size_t name_len = equal - name_value;
    size_t value_len = strlen(equal + 1);
    size_t total_len = name_len + 1 + value_len + 1;  // +1 para '=' e +1 para '\0'

    // Aloca nova string no formato "VAR=valor\0"
    char *new_entry = malloc(total_len);
    if (!new_entry) {
        const char *err = "Falha na alocação\n";
        write(STDERR_FILENO, err, strlen(err));
        return;
    }
    memcpy(new_entry, name_value, total_len - 1);
    new_entry[total_len - 1] = '\0';

    // Remove variável existente (se houver)
    char **env = environ;
    while (*env) {
        if (strncmp(*env, name_value, name_len) == 0 && (*env)[name_len] == '=') {
            free(*env);  // Libera a string antiga
            *env = new_entry;  // Substitui pela nova
            return;
        }
        env++;
    }

    // Se não existia, adiciona ao final do environ
    size_t env_count = 0;
    while (environ[env_count]) env_count++;

    char **new_environ = realloc(environ, (env_count + 2) * sizeof(char *));
    if (!new_environ) {
        free(new_entry);
        const char *err = "Falha ao expandir environ\n";
        write(STDERR_FILENO, err, strlen(err));
        return;
    }

    new_environ[env_count] = new_entry;
    new_environ[env_count + 1] = NULL;
    environ = new_environ;
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc < 2) {
        // Lista todas as variáveis (como env)
        char **env = environ;
        while (*env) {
            write(STDOUT_FILENO, *env, strlen(*env));
            write(STDOUT_FILENO, "\n", 1);
            env++;
        }
        return 0;
    }

    // Processa cada argumento
    int i = 1;
    while (i < argc) {
        export_var(argv[i]);
        i++;
    }

    return 0;
}