#include "huffman.h"
#include "min_heap.h"
#include "search.h"
#include <stdlib.h>
#include <string.h>

// FUNCAO PARA CRIAR UM NOVO NO NA ARVORE DE HUFFMAN
HuffmanNode* create_node(uint8_t byte, int freq, HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (!node) return NULL;
    node->byte = byte;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// FUNCAO DE CONSTRUIR A ARVORE A PARTIR DAS FREQUENCIAS
HuffmanNode* build_huffman_tree(int *frequencies) {
    MinHeap *minHeap;
    HuffmanNode *left, *right, *top;
    
    // PRIMEIRO CONTA O NUM DE CARATERES UNICOS
    int unique_chars = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            unique_chars++;
        }
    }

    // CRIA A MINHEAP
    minHeap = create_min_heap(2 * unique_chars - 1);

    // CRIA OS NOS-FOLHA E INSERE NA MINHEAP
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            HuffmanNode *node = create_node(i, frequencies[i], NULL, NULL);
            MinHeapNode *heap_node = create_min_heap_node(node);
            insert_min_heap(minHeap, heap_node);
        }
    }

    // EXECUTA O ALGORITMO DE HUFFMAN
    while (minHeap->size > 1) {
        MinHeapNode *min1 = extract_min(minHeap);
        MinHeapNode *min2 = extract_min(minHeap);
        
        left = min1->node;
        right = min2->node;

        top = create_node(0, left->freq + right->freq, left, right);

        MinHeapNode *new_heap_node = create_min_heap_node(top);
        insert_min_heap(minHeap, new_heap_node);
        
        free(min1);
        free(min2);
    }
    
    // O único nó restante é a raiz
    MinHeapNode *root_wrapper = extract_min(minHeap);
    HuffmanNode *root = root_wrapper ? root_wrapper->node : NULL;

    // LIBERA A MINHEAP
    free(minHeap->array);
    free(minHeap);
    free(root_wrapper);

    return root;
}

// FUNCAO P LIBERAR A HUFFMAN TREE
void free_huffman_tree(HuffmanNode *root) {
    if (root == NULL) {
        return;
    }

    // Libera recursivamente os filhos (Post-order traversal)
    // Isso garante que você libere os filhos antes do pai, para não perder o acesso a eles.
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);

    // Libera o nó pai (depois dos filhos)
    free(root);
}

// GERA O CÓDIGO BINÁRIO
void generate_huffman_codes(HuffmanNode *root, uint8_t *code, int top, CodeEntry *code_table) {
    // Se for nó folha, copia o código binário para a tabela
    if (root->left == NULL && root->right == NULL) {
        code_table[root->byte].size = top;
        memcpy(code_table[root->byte].code, code, top);
        return;
    }

    // SE SEGUE PARA A ESQUERDA (0)
    if (root->left) {
        code[top] = '0';
        generate_huffman_codes(root->left, code, top + 1, code_table);
    }

    // SE SEGUE PARA A DIREITA (1)
    if (root->right) {
        code[top] = '1';
        generate_huffman_codes(root->right, code, top + 1, code_table);
    }
}

// ESCREVE O CABEÇALHO DO ARQUIVO HUFFMAN
void write_huffman_header(FILE *output, int *frequencies) {
    long total_size = 0;
    for (int i = 0; i < 256; i++) {
        total_size += frequencies[i];
    }

    fwrite(&total_size, sizeof(long), 1, output);
    
    for (int i = 0; i < 256; i++) {
        fwrite(&frequencies[i], sizeof(int), 1, output);
    }
}

// COMPRIME OS DADOS BLOCO A BLOCO
void compress_data_block(FILE *input, FILE *output, CodeEntry *code_table, long file_size) {

    uint8_t buffer_in[BUFFER_SIZE]; // Buffer temporário para ler pedaços do arquivo original
    size_t bytes_read;

    uint64_t original_offset = 0; // posição inicial do bloco

    //Reserva memória para no máximo 10000 blocos comprimidos. Valor de segurança para garantir que todos os blocos sejam comprimidos
    BlockIndexEntry* index = malloc(sizeof(BlockIndexEntry) * 10000);
    int block_count = 0;

    // A cada bloco lido, monta uma linha do índice e atualiza a posição no arquivo original
    // Lê os bytes do arquivo até encher o buffer e armazena na variável bytes_read
    // Isso ocorre até o fim do arquivo, quando não há nenhum byte para ser lido e retorna 0
    while ((bytes_read = fread(buffer_in, 1, BUFFER_SIZE, input)) > 0) {

        uint64_t comp_start = ftell(output); //  marca onde esse bloco comprimido começa no .huff
        int total_bits = 0; // soma de quantos bits foram usados para esse bloco, é o que vai parar em bit_count

        int bitpos = 0; // conta quantos bits já foram preenchidos em out
        uint8_t out = 0; // Byte em montagem (será preenchido bit a bit)

        for (size_t i = 0; i < bytes_read; i++) {

            // Contém, para cada byte, (0..255), a sequencia de bits do código de Huffman e o tamanho
            CodeEntry *entry = &code_table[buffer_in[i]];

            for (int b = 0; b < entry->size; b++) {
                
                // Se o resultado for 1, colocamos 1 ao bit, se for 0, deixamos 0
                // move o 1 para a direita a cada condição verdadeira. 10000000 -> 01000000...
                if(entry->code[b] == '1') out |= ( 1 << ( 7 - bitpos)); 

                bitpos++;
                total_bits++;

                // Quando 8 bits foram preenchidos, significa que 1 byte foi gerado
                if(bitpos == 8){
                    fwrite(&out, 1, 1, output); // escreve um byte comprimido no arquivo .huff
                    bitpos = 0;
                    out = 0;
                }
            }
        }

        // se não for possível gerar um byte, mas ainda tem bit armazenado, escreve em um bloco no arquivo comprimido
        if(bitpos > 0) fwrite(&out, 1, 1, output);

        // usado para saber onde começou o bloco comprimido. Vai pro índice
        uint64_t comp_end = ftell(output);
        
        // armazena os dados do indíce na estrutura BlockIndexEntry
        index[block_count].original_offset = original_offset;
        index[block_count].compressed_offset = comp_start;
        index[block_count].uncompressed_size = bytes_read;
        index[block_count].compressed_size = comp_end - comp_start;
        index[block_count].bit_count = total_bits;

        original_offset += bytes_read;
        block_count++;
    }


    uint64_t index_offset = ftell(output); // armazena a posição onde começam o índice

    //Grava o índice inteiro
    fwrite(index, sizeof(BlockIndexEntry), block_count, output);

    // Monta as informações do arquivo comprimido (quantos blocos, tamnho dos blocos e o índice para acessá-los)
    FileFooter footer = {
        .index_offset = index_offset,
        .num_blocks = block_count,
        .block_size = BLOCK_SIZE
    };

    fwrite(&footer, sizeof(FileFooter), 1, output);

    free(index);

}

// FUNCAO DE COMPRESSAO PRINCIPAL
int compress_file(const char *input_path, const char *output_path) {

    
    // CONTA AS FREQUENCIAS
    FILE *input = fopen(input_path, "rb");
    if (!input) { /* erro */ return 0; }

    int frequencies[256] = {0};
    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read;
    long file_size = 0;


    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        file_size += bytes_read;
        for (size_t i = 0; i < bytes_read; i++) {
            frequencies[buffer[i]]++;
        }
    }
    
    fseek(input, 0, SEEK_SET);

    // CONSTROI A ARVORE DE HUFFMAN
    HuffmanNode *root = build_huffman_tree(frequencies);
    CodeEntry code_table[256];
    uint8_t code_buffer[512];
    generate_huffman_codes(root, code_buffer, 0, code_table);

    // ABRE O ARQUIVO DE SAIDA E ESCREVE O CABECALHO
    FILE *output = fopen(output_path, "wb");
    if (!output) { /* erro */ return 0; }

    write_huffman_header(output, frequencies);

    // LEITURA BLOCO A BLOCO E COMPRESSAO
    compress_data_block(input, output, code_table, file_size);

    // Finalização
    fclose(input);
    fclose(output);
    free_huffman_tree(root);
    return 1; // SUCESSO
}

// FUNCAO DE DESCOMPACTAR
int decompress_file(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "rb");
    if (!input) { /* erro */ return 0; }

    FILE *output = fopen(output_path, "wb");
    if (!output) { fclose(input); return 0; }

    // LE O CABECALHO
    long original_file_size = 0;
    int frequencies[256];
    
    if (fread(&original_file_size, sizeof(long), 1, input) != 1) { /* erro */ return 0; }
    
    if (fread(frequencies, sizeof(int), 256, input) != 256) { /* erro */ return 0; }
    
    // RECONSTROI A ARVORE
    HuffmanNode *root = build_huffman_tree(frequencies);
    HuffmanNode *current = root;
    
    if (!root) { /* erro */ return 0; }

    // DESCOMPACTA BLOCO A BLOCO (bit a bit)
    uint8_t buffer_in[BUFFER_SIZE];
    size_t bytes_read;
    long bytes_decompressed = 0;
    
    uint8_t buffer_out[BUFFER_SIZE]; 
    int buffer_out_idx = 0;

    while (bytes_decompressed < original_file_size && 
           (bytes_read = fread(buffer_in, 1, BUFFER_SIZE, input)) > 0) {
        
        for (size_t i = 0; i < bytes_read; i++) {
            uint8_t current_compressed_byte = buffer_in[i];

            for (int j = 0; j < 8; j++) {
                if (bytes_decompressed >= original_file_size) {
                    goto end_decompression_loop;
                }
                
                int bit = (current_compressed_byte >> (7 - j)) & 1;

                if (bit == 0) {
                    current = current->left;
                } else {
                    current = current->right;
                }

                if (current->left == NULL && current->right == NULL) {
                    
                    buffer_out[buffer_out_idx++] = current->byte;
                    bytes_decompressed++;
                    
                    if (buffer_out_idx == BUFFER_SIZE) {
                        fwrite(buffer_out, 1, BUFFER_SIZE, output);
                        buffer_out_idx = 0; 
                    }

                    current = root;
                }
            }
        }
    }

    end_decompression_loop:
        if (buffer_out_idx > 0) {
            fwrite(buffer_out, 1, buffer_out_idx, output);
        }

    free_huffman_tree(root);

    fclose(input);
    fclose(output);
    
    return 1;
}

size_t decompress_block(FILE *input, long offset, long size, uint32_t bit_count, HuffmanNode *root, uint8_t *out, size_t out_limit){
    
    fseek(input, offset, SEEK_SET); // vai direto para a posição onde o bloco começa

    uint8_t buffer_in[BUFFER_SIZE];
    size_t bytes_read; // bytes lidos do arquivo compactado
    size_t out_count = 0; // quantidade de bytes já compactados

    HuffmanNode *current = root; // começa da razi para decodificar os bits

    int bits_lidos = 0; // controla quantos bits já consumimos desse bloco

    // Lê o bloco comprimido em partes, até consumir o tamanho de bytes 
    while ((bytes_read = fread(buffer_in, 1, size < BUFFER_SIZE ? size : BUFFER_SIZE, input)) > 0){

        size -= bytes_read; // diminuimos o restante a ler

        for(size_t i = 0; i <bytes_read; i++){

            for(int b = 0; b < 8; b++){

                // Para exatamente no último bit válido do bloco. Evita lixo caso o último byte esteja incompleto 
                if(bits_lidos >= bit_count) return out_count;

                // Extrai 1 bit
                int bit = (buffer_in[i] >> (7-b)) & 1;
                
                // Caminha na árvore de Huffman (0 esquerda, 1 direita)
                current = bit ? current->right : current->left;
                bits_lidos++;
            
                //Chegou em uma folha, achou o bit original
                if(!current->left && !current->right){
                    
                    // Adiciona ao buffer de saída se couber
                    if(out_count < out_limit){
                        out[out_count++] = current->byte;
                    }

                    // voltapara pra raiz para ler o próximo byte
                    current = root;

                    //Se o bloco já atingiu o tamanho, para
                    if(out_count == out_limit) return out_count;
                }
            }

        }

        if(size == 0) break; // termina o bloco comprimido
    }
    
    return out_count; // Retorna quantos bytes foram reconstruindos
}
