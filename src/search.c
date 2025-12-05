#include "search.h"
#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "kmp.h" 

/*
    fopen("arquivo, "rb"): abre o arquivo para ler bytes(rb);

    fseek(f, posição, SEEK_SET): move o cursor do arquivo para um byte específico

    fread(buffer, tamanho, quantidade, arquivo): lê bytes do arquivo para memória

    fclose(f): fecha o arquivo

    se fseek move o cursor do disco, fread pega os bytes para memória
    
*/
/*
    ESSA FUNÇÃO ABRE O ARQUIVO, LÊ O ÍNDICE E PARA CADA BLOCO:

        pegar somente o bloco comprimido do disco

        descomprimir só esse bloco

        procurar palavra dentro dele

        reportar a posição original no arquivo descomprimido
*/ 


int buscar_compactado (const char *comp_filename, const char* pattern){

    FILE *f = fopen(comp_filename, "rb"); // read bynary (leitura em bytes)

    if(!f) { perror("fopen"); return 1; }

    size_t m = strlen(pattern); // tamanho da substring que queremos achar


    /*
        O Footer está no final do arquivo

        para lermos ele, temos que ir até o final, e voltar o tamanho de bytes de FileFooter (sizeof) para trás,
        e ler para a memória
    */

    FileFooter footer;
    fseek(f, - (long)sizeof(FileFooter), SEEK_END); // pula até o final menos o tamanho do footer
    fread(&footer, sizeof(FileFooter), 1, f); // lê o footer

    fseek(f, footer.index_offset, SEEK_SET); // vai até o início do indíce

    /*
        O índice precisa dizer:
            onde começa no arquivo comprimido
            tamanho dele comprimido
            tamanho original antes da compressão
            posição original no arquivo descomprimido
    */

    BlockIndexEntry *index = malloc(sizeof(BlockIndexEntry) * footer.num_blocks);
    fread(index, sizeof(BlockIndexEntry), footer.num_blocks, f);

    // PREPARANDO OS BUFFERS
    size_t max_uncompressed_block = footer.block_size;
    char* block_raw = malloc(max_uncompressed_block); // onde o bloco comprimido será armazenado
    char *overlap = malloc(m > 1 ? m - 1 : 1); // armazena últimos bytes do bloco anterior
    size_t overlap_len = 0;

    /*
        Por que precisamos de um overlap? Porque uma substring pode estar dividida entre dois blocos

        [bloco1] ... "par"
        [bloco2] ... "tida" -> "partida"
    */

    fseek(f, 0, SEEK_SET);
    long original_size;
    int freq[256];
    
    fread(&original_size, sizeof(long), 1, f);
    fread(freq, sizeof(int), 256, f);

    HuffmanNode *root = build_huffman_tree(freq);


    for(uint32_t bi = 0; bi < footer.num_blocks; bi++){

        BlockIndexEntry *e = &index[bi]; // pega o bloco comprimido
 
        size_t out_size = decompress_block(f, e->compressed_offset, e->compressed_size, e->bit_count, root, (uint8_t*)block_raw, e->uncompressed_size);        
        
        // monta a área de busca com overlap
        size_t combined_len = overlap_len + out_size;
        char *combined = malloc(combined_len);

        //combina dois blocos de memória
        memcpy(combined, overlap, overlap_len);
        memcpy(combined + overlap_len, block_raw, out_size);

        uint64_t global_start = e->original_offset - overlap_len;


        // procura a string dentro de combined
        printf("\n=== PROCESSANDO BLOCO %d ===\n", bi);
        // printf("\n[DEGUG] Texto descompactado no bloco\n", bi);
        // fwrite(combined, 1, combined_len, stdout);
        // printf("\n------------------------------------------------\n");

        KMP_compress_search(combined, combined_len, pattern, m, global_start);

        if(m > 1){
            size_t new_overlap = (combined_len >= m - 1) ? (m - 1) : combined_len;
            memcpy(overlap, combined + combined_len - new_overlap, new_overlap);
            overlap_len = new_overlap;
        }

        free(combined);
    }
    
    free_huffman_tree(root);
    free(overlap);
    free(block_raw);
    free(index);
    fclose(f);
    return 0;


}
