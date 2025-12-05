#ifndef KMP_H
#define KMP_H

// Calcula a tabela LPS (Longest Proper Prefix which is also Suffix)
int* calcularTabelaLPS(const char *padrao);

// Busca padrão em arquivo grande usando KMP com overlapping buffer
void kmp(const char *arquivo, const char *padrao);

#endif
