#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/kmp-simples.h"
#include "src/huffman.h"
#include "src/search.h"

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("================================================================================\n");
        printf("  SISTEMA DE PROCESSAMENTO DE ARQUIVOS GRANDES\n");
        printf("  Compressao e Busca de Substrings em Arquivos Grandes\n");
        printf("================================================================================\n\n");
        
        printf("Uso: meu_programa <comando> [argumentos]\n\n");
        
        printf("ETAPA 1 - COMPRESSAO DE ARQUIVO (ate 40 pontos)\n");
        printf("---------------------------------------------------------------------------\n");
        printf("Comando: meu_programa compactar <arquivo_original> <arquivo_compactado>\n");
        printf("Descricao: Comprime um arquivo de texto usando algoritmo Huffman.\n");
        printf("Exemplo:   meu_programa compactar dados.txt dados.huf\n\n");
        
        printf("ETAPA 2 - BUSCA EM ARQUIVO NAO COMPRIMIDO (ate 40 pontos)\n");
        printf("---------------------------------------------------------------------------\n");
        printf("Comando: meu_programa buscar_simples <arquivo> \"<substring>\"\n");
        printf("Descricao: Busca uma substring gerando posicoes (offsets em bytes).\n");
        printf("Exemplo:   meu_programa buscar_simples dados.txt \"palavra\"\n\n");
        
        printf("ETAPA 3 - BUSCA EM ARQUIVO COMPRIMIDO (ate 40 pontos)\n");
        printf("---------------------------------------------------------------------------\n");
        printf("Comando: meu_programa buscar_compactado <arquivo_compactado> \"<substring>\"\n");
        printf("Descricao: Busca uma substring sem descompressao total.\n");
        printf("Exemplo:   meu_programa buscar_compactado dados.huf \"palavra\"\n\n");
        
        printf("================================================================================\n\n");
        return 0;
    }

    //etapa1
    if (strcmp(argv[1], "compactar") == 0) {
        if (argc < 4) {
            fprintf(stderr, "\n[ERRO] Argumentos insuficientes\n");
            fprintf(stderr, "Uso: meu_programa compactar <arquivo_original> <arquivo_compactado>\n\n");
            return 1;
        }
        
        const char *arquivo_entrada = argv[2];
        const char *arquivo_saida = argv[3];

        
        if (compress_file(arquivo_entrada, arquivo_saida)) {
            printf("\nSucesso!\n");
            printf(" Arquivo compactado gerado: %s\n\n", arquivo_saida);
            return 0;
        } else {
            fprintf(stderr, "\nErro.\n");
            fprintf(stderr, "Verifique se os caminhos dos arquivos estão corretos.\n\n");
            return 1;
        }
    }

    //etapa 2
    if (strcmp(argv[1], "buscar_simples") == 0) {
        if (argc < 4) {
            fprintf(stderr, "\n[ERRO] Argumentos insuficientes\n");
            fprintf(stderr, "Uso: meu_programa buscar_simples <arquivo> \"<substring>\"\n\n");
            return 1;
        }
        
        const char *arquivo = argv[2];
        const char *padrao = argv[3];
        
        kmp(arquivo, padrao);
        printf("\n");
        
        return 0;
    }

    //etapa 3
    if (strcmp(argv[1], "buscar_compactado") == 0) {
        if (argc < 4) {
            fprintf(stderr, "\n[ERRO] Argumentos insuficientes\n");
            fprintf(stderr, "Uso: meu_programa buscar_compactado <arquivo_compactado> \"<substring>\"\n\n");
            return 1;
        }

        const char *arquivo_compactado = argv[2];
        const char *substring = argv[3];
        
        buscar_compactado(arquivo_compactado, substring);
        printf("\n");

        return 0;
    }
}
