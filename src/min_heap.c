#include "min_heap.h"
#include <stdlib.h>

// Troca dois nós da Min-Heap (Função auxiliar, static a torna privada a este arquivo)
static void swap_min_heap_node(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Função que mantém a propriedade Min-Heap (Função auxiliar)
static void min_heapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Se o filho da esquerda existe E é menor que o 'smallest' atual
    if (left < minHeap->size && 
        minHeap->array[left]->node->freq < minHeap->array[smallest]->node->freq) {
        smallest = left;
    }

    // Se o filho da direita existe E é menor que o 'smallest' atual
    if (right < minHeap->size && 
        minHeap->array[right]->node->freq < minHeap->array[smallest]->node->freq) {
        smallest = right;
    }

    // Se o menor não é o nó pai (idx), troca e chama recursivamente
    if (smallest != idx) {
        swap_min_heap_node(&minHeap->array[smallest], &minHeap->array[idx]);
        min_heapify(minHeap, smallest);
    }
}

// ----------------------------------------------------
// Implementações das Funções Públicas (declaradas em min_heap.h)
// ----------------------------------------------------

// Função para criar uma Min-Heap vazia
MinHeap* create_min_heap(int capacity) {
    MinHeap *minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!minHeap) return NULL;
    
    minHeap->size = 0;
    minHeap->capacity = capacity;
    
    // Aloca a memória para o array de ponteiros para MinHeapNode
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    if (!minHeap->array) {
        free(minHeap);
        return NULL;
    }
    return minHeap;
}

// Função para criar um nó da Min-Heap a partir de um Nó de Huffman
MinHeapNode* create_min_heap_node(HuffmanNode *huffman_node) {
    MinHeapNode *minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    if (!minHeapNode) return NULL;
    minHeapNode->node = huffman_node;
    return minHeapNode;
}

// Insere um novo nó na Min-Heap e ajusta para cima
void insert_min_heap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    if (minHeap->size == minHeap->capacity) {
        return; 
    }

    int i = minHeap->size;
    minHeap->array[i] = minHeapNode;
    minHeap->size++;

    // Ajusta para cima (heapify-up)
    while (i > 0 && minHeap->array[(i - 1) / 2]->node->freq > minHeapNode->node->freq) {
        swap_min_heap_node(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Extrai o nó com a menor frequência (a raiz da Min-Heap)
MinHeapNode* extract_min(MinHeap* minHeap) {
    if (minHeap->size == 0) {
        return NULL;
    }

    // O nó raiz é o de menor frequência
    MinHeapNode* root = minHeap->array[0];

    // Move o último elemento para a raiz
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;

    // Restaura a propriedade Min-Heap
    min_heapify(minHeap, 0);

    return root;
}