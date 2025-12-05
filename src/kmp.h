#ifndef KMP_H
#define KMP_H

void build_compress_LPS(const char *pattern, int m, int *lps);
void KMP_compress_search(const char *text, int n, const char *pattern, int m, long long global_start);

int* LPS_simple_build(const char *padrao);
void KMP_simple_search(const char *arquivo, const char *padrao);

#endif