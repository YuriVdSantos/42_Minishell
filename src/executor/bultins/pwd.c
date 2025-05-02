#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

// Função para obter o nome do diretório atual
char *get_pwd() {
    struct stat current_stat;
    if (stat(".", &current_stat)) {
        perror("stat falhou");
        return NULL;
    }

    // Se for a raiz, retorna "/"
    if (current_stat.st_ino == 2) {
        return strdup("/");
    }

    // Abre o diretório pai ("..")
    DIR *dir = opendir("..");
    if (!dir) {
        perror("opendir falhou");
        return NULL;
    }

    char *current_dir_name = NULL;
    struct dirent *entry;

    // Procura o diretório atual no pai
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        struct stat entry_stat;
        char entry_path[1024] = "../";
        strcat(entry_path, entry->d_name);  // Concatena manualmente (sem snprintf)

        if (stat(entry_path, &entry_stat)) {
            continue;
        }

        // Se o inode bater, encontramos o diretório atual
        if (entry_stat.st_ino == current_stat.st_ino) {
            current_dir_name = strdup(entry->d_name);
            break;
        }
    }
    closedir(dir);

    if (!current_dir_name) {
        fprintf(stderr, "Não foi possível encontrar o diretório.\n");
        return NULL;
    }

    // Sobe para o diretório pai e chama recursivamente
    chdir("..");
    char *parent_path = get_pwd();

    // Concatena o caminho
    if (parent_path) {
        char *full_path;
        if (strcmp(parent_path, "/") == 0) {
            full_path = malloc(strlen(current_dir_name) + 2);
            sprintf(full_path, "/%s", current_dir_name);
        } else {
            full_path = malloc(strlen(parent_path) + strlen(current_dir_name) + 2);
            sprintf(full_path, "%s/%s", parent_path, current_dir_name);
        }
        free(parent_path);
        free(current_dir_name);
        return full_path;
    }

    free(current_dir_name);
    return NULL;
}

// Função principal (pwd)
void pwd() {
    char *cwd = get_pwd();
    if (cwd) {
        printf("%s\n", cwd);
        free(cwd);
    } else {
        perror("pwd falhou");
    }
}
/* 
// Exemplo de uso
int main() {
    pwd();
    return 0;
} */