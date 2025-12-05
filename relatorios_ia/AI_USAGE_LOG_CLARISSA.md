# Relatório de Uso de Inteligencia Artificial - Clarissa

Este documento registra todas as interações significativas com ferramentas de IA generativa (como Gemini, ChatGPT, Copilot, etc.) durante o desenvolvimento deste projeto. O objetivo é promover o uso ético e transparente da IA como ferramenta de apoio, e não como substituta para a compreensão dos conceitos fundamentais. 

# Política de Uso 

O uso de IA foi permitido para as seguintes finalidades: 

● Geração de ideias e brainstorming de algoritmos. 

● Explicação de conceitos complexos. 

● Geração de código boilerplate (ex: estrutura de classes, leitura de arquivos). 

● Sugestões de refatoração e otimização de código. 

● Debugging e identificação de causas de erros. 

● Geração de casos de teste. 

É proibido submeter código gerado por IA sem compreendê-lo completamente e sem adaptá-lo ao projeto. Todo trecho de código influenciado pela IA deve ser referenciado neste log. 

### Registro de Interações

### Interação 1: Fundamentos do Algoritmo KMP

**Data:** 03/12/2024  
**Etapa do Projeto:** 2 - Busca de Substring em Arquivo Grande  
**Ferramenta de IA Utilizada:** Claude (Anthropic)

**Objetivo da Consulta:**  
Compreender a teoria básica do algoritmo KMP e como a tabela LPS (Longest Proper Prefix which is also Suffix) funciona para otimizar a busca.

**Prompts Utilizados:**
1. "Como funciona o algoritmo KMP (Knuth-Morris-Pratt)? Explique de forma didática com exemplos visuais."
2. "O que é a tabela LPS no algoritmo KMP? Como ela é construída e por que é importante?"
3. "Me dê um exemplo passo a passo de construção da tabela LPS para o padrão 'ABABC'."

**Resumo da Resposta da IA:**  
A IA explicou que o KMP evita re-comparações desnecessárias usando uma tabela de "falhas" (LPS). A tabela LPS armazena o comprimento do maior prefixo próprio que também é sufixo para cada posição do padrão. Forneceu exemplos visuais mostrando como construir a tabela para "ABABC" resultando em [0, 0, 1, 2, 0]. Explicou que quando ocorre um mismatch, o algoritmo consulta a tabela para saber quanto do padrão já foi validado e pode ser reaproveitado.

**Análise e Aplicação:**  
A explicação visual foi fundamental para entender a lógica do algoritmo. O conceito de "aproveitar prefixos já confirmados" foi a chave para implementar corretamente o loop while que trata mismatches. A explicação ajudou a evitar a armadilha de simplesmente "voltar uma posição" e entender que a tabela LPS indica exatamente quanto pode ser reaproveitado.

**Referência no Código:**  
Função `LPS_simple_build()`, especialmente as linhas:
```c
while (casamento > 0 && padrao[casamento] != padrao[indice])
    casamento = tabela[casamento - 1];
```

---

### Interação 2: Processamento de Arquivos Grandes

**Data:** 04/12/2024  
**Etapa do Projeto:** 2 - Busca de Substring em Arquivo Grande  
**Ferramenta de IA Utilizada:** Claude (Anthropic)

**Objetivo da Consulta:**  
Entender como processar arquivos grandes que não cabem na memória RAM usando leitura em blocos e evitar perder padrões que estão divididos entre blocos.

**Prompts Utilizados:**
1. "Como fazer busca de substring em arquivos muito grandes que não cabem na memória? Preciso ler em blocos."
2. "Se eu ler um arquivo em blocos de 4096 bytes, como garantir que não vou perder um padrão que está dividido entre dois blocos?"
3. "Explique a técnica de sobreposição de buffer para busca em arquivos grandes."

**Resumo da Resposta da IA:**  
A IA explicou a técnica de "buffer overlapping" (sobreposição de buffer), onde os últimos (tamanho_padrao - 1) bytes do bloco atual são copiados para o início do próximo bloco. Isso garante que padrões na fronteira entre blocos sejam detectados. Forneceu exemplos visuais mostrando como "ABELHA" poderia estar dividido entre blocos e como a sobreposição resolve isso.

**Análise e Aplicação:**  
Esta foi a parte mais crítica do projeto. A técnica de sobreposição não era intuitiva inicialmente. A IA ajudou a calcular corretamente: tamanho do buffer (TAMANHO_BLOCO + tamanho_padrao - 1), uso de memcpy para copiar os últimos bytes, e ajuste da posicao_arquivo considerando a sobreposição. Sem essa orientação, provavelmente teria implementado um sistema que perderia ocorrências na fronteira dos blocos.

**Referência no Código:**  
Função `KMP_simple_search()`, linhas de preparação do próximo bloco:
```c
int tamanho_sobreposicao = tamanho_padrao - 1;
memcpy(buffer, buffer + total_bytes - tamanho_sobreposicao,
       tamanho_sobreposicao);
posicao_arquivo += bytes_lidos - tamanho_sobreposicao;
```

---

### Interação 3: Cálculo de Posições (Offsets)

**Data:** 04/12/2024

**Objetivo da Consulta:**  
Compreender como calcular corretamente a posição (offset) do padrão encontrado no arquivo original, considerando que estamos processando em blocos e com sobreposição.

**Prompts Utilizados:**
1. "Como calcular a posição exata (offset em bytes) de um padrão encontrado quando estou lendo o arquivo em blocos?"
2. "Por que a fórmula é 'posicao_arquivo + indice_buffer - tamanho_padrao + 1'? Explique cada parte."
3. "Me dê exemplos práticos de cálculo de posição com arquivo de 10000 bytes lido em blocos de 4096."

**Resumo da Resposta da IA:**  
A IA explicou que indice_buffer aponta para o último caractere do padrão encontrado, não o primeiro. Para obter o início do padrão, é necessário subtrair o tamanho e ajustar com +1. A IA também alertou sobre a necessidade de usar long long para arquivos grandes (>4GB).

**Análise e Aplicação:**  
O cálculo de offset era fonte de bugs sutis. A explicação detalhada ajudou a entender por que não basta fazer posicao_arquivo + indice_buffer. O ajuste (- tamanho_padrao + 1) parecia contra-intuitivo, mas os exemplos visuais esclareceram. Também aprendi sobre a importância de usar tipos de 64 bits para posições em arquivos grandes.

**Referência no Código:**
```c
long long posicao_encontrada = posicao_arquivo + (long long)indice_buffer -
                                tamanho_padrao + 1;
```

---

### Interação 4: Detecção de Múltiplas Ocorrências

**Data:** 04/12/2024

**Objetivo da Consulta:**  
Entender como detectar múltiplas ocorrências do padrão, incluindo ocorrências sobrepostas (por exemplo, "ABA" em "ABABABA").

**Prompts Utilizados:**
1. "No KMP, depois de encontrar um padrão, como continuar buscando outras ocorrências?"
2. "Por que usar 'casamento = tabelaLPS[casamento - 1]' após encontrar o padrão, em vez de 'casamento = 0'?"
3. "Explique como o KMP detecta padrões sobrepostos, como 'ABA' em 'ABABABA'."

**Resumo da Resposta da IA:**  
A IA explicou que resetar casamento = 0 após encontrar o padrão faria o algoritmo perder ocorrências sobrepostas. Usar casamento = tabelaLPS[casamento - 1] permite aproveitar a parte do padrão que já foi validada. Forneceu exemplos visuais mostrando como "ABA" aparece 3 vezes em "ABABABA" com sobreposição.

**Análise e Aplicação:**  
Inicialmente, eu havia implementado casamento = 0 após encontrar o padrão, o que fazia o programa perder ocorrências sobrepostas. A IA esclareceu que a mesma lógica de "reaproveitar prefixos" usada durante a busca também se aplica após encontrar o padrão. Isso garantiu que o algoritmo encontrasse TODAS as ocorrências, não apenas algumas.

**Referência no Código:**
```c
if (casamento == tamanho_padrao) {
    printf("%I64d\n", posicao_encontrada);
    casamento = tabelaLPS[casamento - 1]; // Continua buscando
}
```

---

### Interação 5: Boas Práticas em C

**Data:** 04/12/2024

**Objetivo da Consulta:**  
Compreender aspectos de boas práticas em C, como uso de size_t, fread, gerenciamento de memória e tratamento de erros.

**Prompts Utilizados:**
1. "O que é size_t em C e quando devo usá-lo?"
2. "Como funciona fread em C? O que é o ponteiro FILE e como ele mantém a posição no arquivo?"
3. "Quais são as boas práticas para liberar memória e fechar arquivos em C?"

**Resumo da Resposta da IA:**  
A IA explicou que size_t é um tipo unsigned usado para tamanhos e índices, que se adapta automaticamente ao sistema (32 ou 64 bits). Explicou que FILE* é um ponteiro que mantém um cursor interno que avança automaticamente a cada leitura. Alertou sobre a importância de verificação de erros, liberação de memória e fechamento de arquivos.

**Análise e Aplicação:**  
Esses conceitos ajudaram a escrever código mais robusto e portável. O entendimento de como FILE* funciona como um "cursor automático" simplificou muito a lógica de leitura sequencial. A ênfase em tratamento de erros evitou memory leaks e deixou o código mais profissional.

**Referência no Código:**  
Uso de size_t para índices e tamanhos, verificações de erro após malloc/fopen, sequência de liberação de recursos.

---

### Interação 6: Debugging e Modo de Arquivo

**Data:** 04/12/2024

**Objetivo da Consulta:**  
Debugging: entender por que o programa estava perdendo algumas ocorrências em arquivos de teste.

**Prompts Utilizados:**
1. "Meu KMP está perdendo algumas ocorrências. Como depurar isso?"
2. "Qual a diferença entre leitura de arquivo em modo texto ('r') vs binário ('rb')?"
3. "Como garantir que estou processando até o último byte do arquivo, incluindo o último bloco parcial?"

**Resumo da Resposta da IA:**  
A IA identificou dois problemas comuns: usar modo texto ('r') em vez de binário ('rb') pode causar conversões indesejadas de caracteres (especialmente \n no Windows), e não processar corretamente o último bloco quando bytes_lidos < TAMANHO_BLOCO. Sugeriu sempre usar modo binário e garantir que o loop principal processe todos os bytes antes do break.

**Análise e Aplicação:**  
O bug estava justamente no modo de abertura do arquivo. Mudar de "r" para "rb" resolveu inconsistências com arquivos contendo quebras de linha. A IA também ajudou a reorganizar a lógica para garantir que o último bloco fosse processado antes de sair do loop.

**Referência no Código:**
```c
FILE *fluxo = fopen(arquivo, "rb"); // Modo binário
```

---

*Este registro demonstra uso responsável e documentado da IA como ferramenta de aprendizado, conforme solicitado nas especificações do projeto.*