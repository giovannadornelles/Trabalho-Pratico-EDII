#include <stdio.h>
#include <string.h>
#include "kmp-simples.h"

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Uso:\n");
        printf("  %s buscar_simples <arquivo> <substring>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "buscar_simples") == 0) {
        kmp(argv[2], argv[3]);
        return 0;
    }

    printf("Comando invalido.\n");
    return 1;
}
