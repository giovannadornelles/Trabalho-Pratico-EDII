#include "kmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_BLOCO 8192
#define TAMANHO_MAXIMO_PADRAO 1024
#define SOBREPOSICAO (TAMANHO_MAXIMO_PADRAO - 1)  

int* calcularTabelaLPS(const char *padrao) {
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

void kmp(const char *arquivo, const char *padrao) {
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
    
    int *tabelaLPS = calcularTabelaLPS(padrao);
    
    
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
