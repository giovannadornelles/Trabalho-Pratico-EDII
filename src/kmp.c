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

        // Enquanto houver mismatch, reduzimos len, tentando reaproveitar os prefixos já confirmados
        while (len > 0 && pattern[i] != pattern[len]) len = lps[len - 1]; // recua para o maior prefixo conhecido

        // se houve match do caractere atual, aumentamos o tamanho atual do prefixo
        if (pattern[i] == pattern[len]) len++; 

        lps[i] = len; // registramos o comprimento do maior prefixo válido
        
        // printf("%d\t%c\t%d\n", i, pattern[i], lps[i]);
    }

}

// Busca KMP com prints explicativos
void KMP_compress_search(const char *text, int n, const char *pattern, int m, long long global_start) {

    int lps[m];

    // LPS[J] indica o maior prefixo do padrão que também é sufixo até a posição j.
    build_compress_LPS(pattern, m, lps);

    // printf("\n[DEBUG] Iniciando KMP...\n");
    // printf("Scan: text vs pattern\n");

    int i = 0; // índice do texto descompactado
    int j = 0; // índice do padrão

    while (i < n) {
        // printf("Comparando texto[%d]='%c' com padrao[%d]='%c'\n", i, text[i], j, pattern[j]);
        
        // se os caracteres combinam, avança ambos
        if (text[i] == pattern[j]) {
            i++;
            j++;
            
            // Se j for igual tamanho de m, quer dizer que houve um match(todos os caracteres do padrão e do bloco combinam)
            if (j == m) {

                // delay só para causar um drama
                Sleep(300);

                // global_start garante o offset real no arquivo completo, não apenas no bloco atual
                printf("\n>>> \"%s\" encontrado. Offset global = %lld\n", pattern, global_start + i - m);

                // Usamos LPS para continuar a busca aproveitando as comparações já válidas
                j = lps[j - 1];
            }
        } else {

            // Se houve mismatch, mas já temos parte do padrão avançado
            // Voltamos j para o maior prefixo possível usando o LPS
            if (j > 0) j = lps[j - 1];
            
            else i++; // se não, só avançamos o texto
        }
    }

    printf("\n");
}



/* --------------- busca simples ----------------- */

int* LPS_simple_build(const char *padrao) {
    //tamanho do padrao
    int tamanho = strlen(padrao); 
    //memoria para a tabela LPS
    int *tabela = malloc(tamanho * sizeof(int));
    //unico caractere nao tem prefixo e sufixo
    tabela[0] = 0;

    //percorre o padrao ate o final, indice é a posicao atual no padrao e casamento o tamanho do maior prefixo que é sufixo
    for (int indice = 1, casamento = 0; indice < tamanho; indice++) {

        //enquanto houver mismatch, reduzimos casamento, tentando reaproveitar os prefixos já confirmados
        while (casamento > 0 && padrao[casamento] != padrao[indice])
            casamento = tabela[casamento - 1];

        //se houve match do caractere atual, aumentamos o tamanho atual do prefixo
        if (padrao[casamento] == padrao[indice])
            casamento++;

        //registramos o comprimento do maior prefixo válido
        tabela[indice] = casamento;
    }

    return tabela;
}


void KMP_simple_search(const char *arquivo, const char *padrao) {
    //abre o arquivo para leitura binaria
    FILE *fluxo = fopen(arquivo, "rb");
    if (!fluxo) {
        perror("Erro");
        return;
    }

    int tamanho_padrao = strlen(padrao);
    
   //caso digite um padrao muito grande
    if (tamanho_padrao > TAMANHO_MAXIMO_PADRAO) {
        fprintf(stderr, "Erro: padrão muito grande (máximo %d caracteres)\n", TAMANHO_MAXIMO_PADRAO);
        fclose(fluxo);
        return;
    }
    
    //construção da tabela LPS, otimizar a busca
    int *tabelaLPS = LPS_simple_build(padrao);
    
    
    //aloca buffer com espaço extra para sobreposição
    char *buffer = malloc(TAMANHO_BLOCO + tamanho_padrao - 1);

    //caso falhe a alocação
    if (!buffer) {
        perror("Erro alocando buffer");
        free(tabelaLPS);
        fclose(fluxo);
        return;
    }
    
    //inicializa variaveis de controle
    long long posicao_arquivo = 0;
    int casamento = 0;
    size_t bytes_lidos;
    int deslocamento = 0; 
    
    //leitura em blocos do arquivo
    while ((bytes_lidos = fread(buffer + deslocamento, 1, TAMANHO_BLOCO, fluxo)) > 0) {

        // Total de bytes a serem processados no buffer
        size_t total_bytes = bytes_lidos + deslocamento; 
        
        // Percorre o buffer atual
        for (size_t indice_buffer = 0; indice_buffer < total_bytes; indice_buffer++) { 

            // Algoritmo KMP no buffer
            while (casamento > 0 && padrao[casamento] != buffer[indice_buffer])
                casamento = tabelaLPS[casamento - 1];
            
            if (padrao[casamento] == buffer[indice_buffer])
                casamento++;
            
            // Padrão encontrado
            if (casamento == tamanho_padrao) {
                long long posicao_encontrada = posicao_arquivo + (long long)indice_buffer - tamanho_padrao + 1;
                printf("%I64d\n", posicao_encontrada); //imprimir um número inteiro de 64 bits
                casamento = tabelaLPS[casamento - 1];  
            }
        }
        
        // Se não leu um bloco completo, chegou ao final
        if (bytes_lidos < TAMANHO_BLOCO)
            break;
        
        
        // Garante que padrões na fronteira não sejam perdidos
        //copia os últimos (tamanho_padrao - 1) bytes para o início do buffer
        int tamanho_sobreposicao = tamanho_padrao - 1;

        if (tamanho_sobreposicao > 0) {
            memcpy(buffer, buffer + total_bytes - tamanho_sobreposicao, tamanho_sobreposicao);

            // Atualiza deslocamento e posição do arquivo
            deslocamento = tamanho_sobreposicao;
            posicao_arquivo += bytes_lidos - tamanho_sobreposicao;
        } else {
            //caso o padrão seja de tamanho 0 ou 1
            deslocamento = 0;
            posicao_arquivo += bytes_lidos;
        }
    }
    
    free(buffer);
    free(tabelaLPS);
    fclose(fluxo);
}



