#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdint.h>

// Estrutura responsavel pela gestão dos blocos de índice
typedef struct {
    uint64_t original_offset;   // posição inicial do bloco no arquivo original
    uint64_t compressed_offset; // posição em bytes dentro do arquivo compactado onde começa o bloco
    uint64_t uncompressed_size; // tamanho em bytes do arquivo original
    uint64_t compressed_size;   // tamanho em bytes do arquivo compactado
    uint32_t bit_count;         // quantidade exata de bits válidos no bloco comprimido
} BlockIndexEntry; 

// Estrutura responsável pelo armazenamento de informações do arquivo
typedef struct {
    uint64_t index_offset; // posição onde começa a tabela de índices
    uint32_t num_blocks;   // quantidade de blocos criados
    uint32_t block_size;   // tamanho padrão descompactado
} FileFooter;


int buscar_compactado (const char *comp_filename, const char* pattern);

#endif