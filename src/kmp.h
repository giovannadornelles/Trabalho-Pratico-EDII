#ifndef KMP_H
#define KMP_H

void buildLPS(const char *pattern, int m, int *lps);
void KMP_search(const char *text, int n, const char *pattern, int m, long long global_start);


#endif