#include <stdio.h>
#include <string.h>
#include "src/huffman.h"
#include "src/search.h"
#include "src/kmp.h"
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
    if (strcmp(argv[1], "descompactar") == 0) {
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

    if(strcmp(argv[1], "buscar_compactado") == 0){
        if(argc < 4){
            fprintf(stderr, "Uso: meu_programa buscar_comprimido <arquivo_compactado> \"substring\" \n");
            return 1;
        }

        const char *arquivo = argv[2];
        const char *substring = argv[3];

        printf("\nBuscando \"%s\" dentro do arquivo compactado \"%s\"...\n\n", substring, arquivo);

        buscar_compactado(arquivo, substring);

        return 0;
    }

    if (strcmp(argv[1], "buscar_simples") == 0) {

        if (argc < 4) {
            printf("Uso:\n");
            printf("  %s buscar_simples <arquivo> <substring>\n", argv[0]);
            return 1;
        }
    
        KMP_simple_search(argv[2], argv[3]);
        return 0;
    }

    
    fprintf(stderr, "Comando desconhecido: %s\n", argv[1]);
    return 1;

    if (argc < 4) {
        printf("Uso:\n");
        printf("  %s buscar_simples <arquivo> <substring>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "buscar_simples") == 0) {
        KMP_simple_search(argv[2], argv[3]);
        return 0;
    }

    printf("Comando invalido.\n");
    return 1;

}