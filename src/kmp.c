#include <stdio.h>
#include <string.h>
#include "kmp.h"

// Função que monta tabela de prefixo (lps)
// lps[i] = tamanho do maior prefixo que também é sufixo até posição i
void buildLPS(const char *pattern, int m, int *lps) {
    int len = 0; // tamanho do prefixo atual
    lps[0] = 0;

    printf("\n[DEBUG] Construindo tabela LPS:\n");
    printf("index\tchar\tlps\n");

    for (int i = 1; i < m; i++) {
        while (len > 0 && pattern[i] != pattern[len])
            len = lps[len - 1];

        if (pattern[i] == pattern[len])
            len++;

        lps[i] = len;
        printf("%d\t%c\t%d\n", i, pattern[i], lps[i]);
    }

    printf("-------------------------------\n");
}

// Busca KMP com prints explicativos
void KMP_search(const char *text, int n, const char *pattern, int m, long long global_start) {

    int lps[m];
    buildLPS(pattern, m, lps);

    printf("\n[DEBUG] Iniciando KMP...\n");
    printf("Scan: text vs pattern\n");

    int i = 0; // índice do texto
    int j = 0; // índice do padrão

    while (i < n) {
        printf("Comparando texto[%d]='%c' com padrao[%d]='%c'\n", i, text[i], j, pattern[j]);

        if (text[i] == pattern[j]) {
            i++;
            j++;

            if (j == m) {
                printf("\n>>> MATCH ENCONTRADO! Offset global = %lld\n\n", global_start + i - m);
                j = lps[j - 1];
            }
        } else {
            if (j > 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
}
