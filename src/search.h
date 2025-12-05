#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdint.h>

// Estrutura responsavel pela gestão do índice de blocos
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

/*
O arquivo compactado tem 3 partes:
    Cabeçalho: Tamanho + frequências
    Dados Comprimidos a cada bloco
    Indice + footer: 
        O footer dá o endereço do índice e quantos blocos existem
        O índice diz onde cada bloco está e como descompactar só aquele pedaço

BlockIndexEntry
    original_offset   = Em que posição, em bytes, o bloco começa no arquivo descompactado
    compressed_offset = Em que posição, em bytes, o bloco comprimido começa dentro do arquivo compactado
    uncompressed_size = Quantos bytes esse bloco tem quando está descompactado
    compressed_size   = Quantos bytes ele ocupou depois de comprimido
    bit_count         = Quantos bits válidos existem nesse bloco comprimido. 
                        Como Huffman escreve bit a bit, o último byte pode não estar “completo”. bit_count evita que a descompressão leia bits lixo do final.

FileFooter
    index_offset = Em que posição do arquivo começa a tabela de BlockIndexEntry
    num_blocks   = Quantos blocos existem (quantas entradas de índice você deve ter)
    block_size   = Tamanho padrão de cada bloco descompactado(menos do último)
                    Utilidade: saber o limite teórico e saber alocar buffer

*/