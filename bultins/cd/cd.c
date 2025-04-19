#include <stdio.h>
#include <unistd.h>  // Para chdir()
#include <stdlib.h>  // Para exit()
#include <string.h>  // Para strcmp()

void cd(const char *path) {
    // Se não for passado um caminho, vai para o diretório home
    if (path == NULL || strcmp(path, "~") == 0) {
        path = getenv("HOME");
    }
    // Se for "-", volta para o diretório anterior
    else if (strcmp(path, "-") == 0) {
        path = getenv("OLDPWD");
    }

    if (chdir(path) != 0) {
        perror("cd falhou");
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Uso: %s [diretório]\n", argv[0]);
        return 1;
    }

    cd(argv[1]);
    return 0;
}