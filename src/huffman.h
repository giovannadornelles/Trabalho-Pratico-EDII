#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

//TAMANHO DO BUFFER FIXO PARA LER O ARQUIVO EM BLOCOS
#define BUFFER_SIZE 8192
#define BLOCK_SIZE 8192

// Definição da estrutura do Nó da Árvore de Huffman
typedef struct HuffmanNode {
    uint8_t byte;           // O byte (símbolo) que este nó representa (0-255)
    int freq;               // Frequência de ocorrência
    struct HuffmanNode *left, *right; // Ponteiros para os filhos
} HuffmanNode;

// Definição do Tabela de Códigos (Code Table)
// Armazena o código binário (string de bits) para cada byte.
// O tamanho 256 é fixo (para todos os possíveis bytes de 0 a 255).
typedef struct {
    uint8_t code[512]; // O código em si (ex: "01011...")
    int size;          // Tamanho do código em bits
} CodeEntry;

// FUNCAO PRINCIPAL DE COMPACTAR
int compress_file(const char *input_path, const char *output_path);

// função de Descompactar (utilizada em testes)
int decompress_file(const char *input_path, const char *output_path);

// FUNCOES AUXILIARES
HuffmanNode* build_huffman_tree(int *frequencies);
void generate_huffman_codes(HuffmanNode *root, uint8_t *code, int top, CodeEntry *code_table);
void write_huffman_header(FILE *output, int *frequencies);
void compress_data_block(FILE *input, FILE *output, CodeEntry *code_table, long file_size);
void free_huffman_tree(HuffmanNode *root);
size_t decompress_block(FILE *input, long offset, long size, uint32_t bit_count, HuffmanNode *root, uint8_t *out, size_t out_limit);

#endif