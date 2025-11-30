#include <stdio.h>
#include <string.h>
#include "huffman.h"

// Ponto de entrada do programa
int main(int argc, char *argv[]) {
    // O programa requer pelo menos 4 argumentos: meu_programa, comando, arquivo_origem, arquivo_destino
    if (argc < 4) {
        fprintf(stderr, "Uso: meu_programa compactar <arquivo_original> <arquivo_compactado>\n");
        return 1;
    }

    // Verifica o comando compactar
    if (strcmp(argv[1], "compactar") == 0) {
        const char *input_file = argv[2];
        const char *output_file = argv[3];

        printf("Iniciando compressao de '%s' para '%s'...\n", input_file, output_file);
        
        //chama a função principal de compressão
        if (compress_file(input_file, output_file)) {
            printf("Compressao concluida com sucesso!\n");
            return 0;
        } else {
            fprintf(stderr, "Erro na compressao. Verifique os caminhos dos arquivos.\n");
            return 1;
        }
    } 

    // Verifica o comando descompactar
    else if (strcmp(argv[1], "descompactar") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Uso: meu_programa descompactar <arquivo_compactado> <arquivo_destino>\n");
            return 1;
        }
        const char *input_file = argv[2];
        const char *output_file = argv[3];

        printf("Iniciando descompressao de '%s' para '%s'...\n", input_file, output_file);
        
        if (decompress_file(input_file, output_file)) {
            printf("Descompressao concluida com sucesso!\n");
            return 0;
        } else {
            fprintf(stderr, "Erro na descompressao.\n");
            return 1;
        }
    }
    
    else {
        fprintf(stderr, "Comando desconhecido: %s\n", argv[1]);
        return 1;
    }

    return 0;
}