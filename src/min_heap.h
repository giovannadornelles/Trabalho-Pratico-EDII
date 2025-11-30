#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdlib.h> // Para size_t e NULL
#include "huffman.h"

//MIN_HEAP -> ESTRUTURA AUXILIAR PRA CRIAÇÃO DA ARVORE DE HUFFMAN

// Estrutura para um item da Min-Heap (wrapper para o Nó de Huffman)
typedef struct {
    HuffmanNode *node;
} MinHeapNode;

// Estrutura da Min-Heap
typedef struct {
    MinHeapNode **array; // Array de ponteiros para MinHeapNode
    int size;           // Número atual de nós na Min-Heap
    int capacity;       // Capacidade máxima da Min-Heap
} MinHeap;

// Funções públicas da Min-Heap
MinHeap* create_min_heap(int capacity);
MinHeapNode* create_min_heap_node(HuffmanNode *huffman_node);
MinHeapNode* extract_min(MinHeap* minHeap);
void insert_min_heap(MinHeap* minHeap, MinHeapNode* minHeapNode);

#endif // MIN_HEAP_H