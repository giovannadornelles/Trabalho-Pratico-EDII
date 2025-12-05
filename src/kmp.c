#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kmp.h"
#include "windows.h"


#define TAMANHO_BLOCO 8192
#define TAMANHO_MAXIMO_PADRAO 1024
#define SOBREPOSICAO (TAMANHO_MAXIMO_PADRAO - 1) 

// Função que monta tabela de prefixo (lps)
// lps[i] = tamanho do maior prefixo que também é sufixo até posição i
void build_compress_LPS(const char *pattern, int m, int *lps) {
    int len = 0; // tamanho do prefixo atual
    lps[0] = 0;

    // printf("\n[DEBUG] Construindo tabela LPS:\n");
    // printf("index\tchar\tlps\n");

    for (int i = 1; i < m; i++) {
        while (len > 0 && pattern[i] != pattern[len])
            len = lps[len - 1];

        if (pattern[i] == pattern[len])
            len++;

        lps[i] = len;
        // printf("%d\t%c\t%d\n", i, pattern[i], lps[i]);
    }

}

// Busca KMP com prints explicativos
void KMP_compress_search(const char *text, int n, const char *pattern, int m, long long global_start) {

    int lps[m];
    build_compress_LPS(pattern, m, lps);

    // printf("\n[DEBUG] Iniciando KMP...\n");
    // printf("Scan: text vs pattern\n");

    int i = 0; // índice do texto
    int j = 0; // índice do padrão

    while (i < n) {
        // printf("Comparando texto[%d]='%c' com padrao[%d]='%c'\n", i, text[i], j, pattern[j]);

        if (text[i] == pattern[j]) {
            i++;
            j++;

            if (j == m) {
                Sleep(300);
                printf("\n>>> \"%s\" encontrado. Offset global = %lld\n", pattern, global_start + i - m);
                j = lps[j - 1];
            }
        } else {
            if (j > 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    printf("\n");
}


int* LPS_simple_build(const char *padrao) {
    int tamanho = strlen(padrao);
    int *tabela = malloc(tamanho * sizeof(int));
    tabela[0] = 0;

    for (int indice = 1, casamento = 0; indice < tamanho; indice++) {
        while (casamento > 0 && padrao[casamento] != padrao[indice])
            casamento = tabela[casamento - 1];

        if (padrao[casamento] == padrao[indice])
            casamento++;

        tabela[indice] = casamento;
    }

    return tabela;
}

void KMP_simple_search(const char *arquivo, const char *padrao) {
    FILE *fluxo = fopen(arquivo, "rb");
    if (!fluxo) {
        perror("Erro abrindo arquivo");
        return;
    }

    int tamanho_padrao = strlen(padrao);
    
   
    if (tamanho_padrao > TAMANHO_MAXIMO_PADRAO) {
        fprintf(stderr, "Erro: padrão muito grande (máximo %d caracteres)\n", TAMANHO_MAXIMO_PADRAO);
        fclose(fluxo);
        return;
    }
    
    int *tabelaLPS = LPS_simple_build(padrao);
    
    
    char *buffer = malloc(TAMANHO_BLOCO + tamanho_padrao - 1);
    if (!buffer) {
        perror("Erro alocando buffer");
        free(tabelaLPS);
        fclose(fluxo);
        return;
    }
    
    long long posicao_arquivo = 0;
    int casamento = 0;
    size_t bytes_lidos;
    int deslocamento = 0; 
    
    while ((bytes_lidos = fread(buffer + deslocamento, 1, TAMANHO_BLOCO, fluxo)) > 0) {
        size_t total_bytes = bytes_lidos + deslocamento; 
        
        for (size_t indice_buffer = 0; indice_buffer < total_bytes; indice_buffer++) {
            // Algoritmo KMP 
            while (casamento > 0 && padrao[casamento] != buffer[indice_buffer])
                casamento = tabelaLPS[casamento - 1];
            
            if (padrao[casamento] == buffer[indice_buffer])
                casamento++;
            
            // Padrão encontrado
            if (casamento == tamanho_padrao) {
                long long posicao_encontrada = posicao_arquivo + (long long)indice_buffer - tamanho_padrao + 1;
                printf("%I64d\n", posicao_encontrada);
                casamento = tabelaLPS[casamento - 1];  
            }
        }
        
        // Se não leu um bloco completo, chegou ao final
        if (bytes_lidos < TAMANHO_BLOCO)
            break;
        
        
        // Garante que padrões na fronteira não sejam perdidos
        int tamanho_sobreposicao = tamanho_padrao - 1;
        if (tamanho_sobreposicao > 0) {
            memcpy(buffer, buffer + total_bytes - tamanho_sobreposicao, tamanho_sobreposicao);
            deslocamento = tamanho_sobreposicao;
            posicao_arquivo += bytes_lidos - tamanho_sobreposicao;
        } else {
            deslocamento = 0;
            posicao_arquivo += bytes_lidos;
        }
    }
    
    free(buffer);
    free(tabelaLPS);
    fclose(fluxo);
}



