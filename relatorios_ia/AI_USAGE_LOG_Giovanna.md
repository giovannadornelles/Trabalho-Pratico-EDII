Relatório de Uso de Inteligência Artificial Generativa - Giovanna

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

# Registro de Interações 

Interação 1 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de Arquivos / Planejamento 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Validar a escolha do algoritmo de compressão e entender como lidar com a restrição de I/O. 

● Prompt(s) Utilizado(s): 1.  Para compressão de arquivos grandes em C com baixo uso de memória, qual algoritmo (Huffman ou LZW) é mais adequado para implementação manual e como o programa deve ler o arquivo para minimizar o uso de memória ram?" 

● Resumo da Resposta da IA: A IA confirmou a escolha do Huffman como sendo mais simples para implementação manual completa e explicou que é necessário ler o arquivo em duas passagens: uma para contagem de frequência (cabeçalho) e outra para compressão. 

● Análise e Aplicação: Reforçou a decisão de utilizar Huffman, a estrutura de I/O em duas passagens e o uso do fseek para reposicionar o ponteiro do arquivo. 

● Referência no Código: A lógica inspirada por esta interação foi implementada no arquivo huffman.c , especificamente na função 

compress_file() .

Interação 2 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de Arquivos 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Entender como lidar com o requisito de "não carregar o arquivo inteiro na RAM" para a compressão Huffman, que precisa de duas passagens (contagem de frequência e codificação). 

● Prompt(s) Utilizado(s): 

1.  como o programa vai ler o arquivo grande de texto duas vezes em C, sem carregar tudo na memória? como ele vai contar a frequência de bytes e depois codificar esses bytes? 

2.  o que é o cabeçalho e o buffer size em um compressor Huffman para arquivos grandes?. 

● Resumo da Resposta da IA: A IA explicou a necessidade de usar 

fseek(input, 0, SEEK_SET) para "rebobinar" o ponteiro do arquivo para o início após a primeira leitura. Reforçou que toda a leitura deve ser feita em "chunks" ou "blocos" de tamanho fixo ( BUFFER_SIZE ), usando fread() ,mantendo apenas o array de 256 frequências na memória durante a primeira passagem. ● Análise e Aplicação: A arquitetura de duas passagens de arquivo (ambas usando buffers de tamanho BUFFER_SIZE ) foi adotada integralmente. 

● Referência no Código: Implementação do while ((bytes_read =fread(...))) e do fseek(input, 0, SEEK_SET) na função 

compress_file no src/huffman.c .

Interação 3 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de Arquivos 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Entender a melhor forma de implementar a lógica de selecionar os dois nós de menor frequência para construir a Árvore de Huffman, pois usar um array desordenado seria muito lento. 

● Prompt(s) Utilizado(s): 

1.  qual estrutura de dados devo usar para encontrar rapidamente os dois menores elementos com base na frequência ao construir a Árvore de Huffman? 

2.  como funciona o algoritmo da Min-Heap para extrair o mínimo? Preciso de um pseudocódigo. 

● Resumo da Resposta da IA: A IA sugeriu o uso de uma Min-Heap pois ela mantém o elemento de menor valor no topo e permite a extração (extract_min ) em tempo O(log n). A IA forneceu o pseudocódigo para as operações básicas ( insert , extract_min e min_heapify ). 

● Análise e Aplicação: o uso da Min-Heap foi crucial para a eficiência do algoritmo. Decidi separar a estrutura em seus próprios arquivos (min_heap.h e min_heap.c ), para manter o código limpo e legível. 

● Referência no Código: A lógica inspirada por esta interação foi implementada no arquivo min_heap.c , especificamente na função 

create_min_heap,insert_min_heap, extract_min emin_heapify .

Interação 4 ● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de Arquivos 

● Ferramenta de IA Utilizada: Gemini Advanced 

● Objetivo da Consulta: Entender como liberar a memória da estrutura da árvore de huffman. 

● Prompt(s) Utilizado(s): 

1.  como liberar a memória da huffman tree de “baixo pra cima”, sem perder a referência aos nós mais frequentes? 

● Resumo da Resposta da IA: A IA sugeriu uma travessia pós-ordem para liberar os filhos antes de liberar o pai, garantindo que o acesso aos filhos não seja perdido. 

● Análise e Aplicação: Implementei a função 

free_huffman_tree(HuffmanNode *root) com uma lógica recursiva de pós-ordem no arquivo huffman.c 

Referência no Código: Implementação da função free_huffman_tree e sua chamada nas funções compress_file e decompress_file no huffman.c .

Interação 5 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de arquivos (Geração de Códigos) 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Entender como percorrer a Árvore de Huffman pronta para gerar os códigos binários (strings de '0's e '1's) e como armazenar esses códigos temporariamente para cada símbolo/nó. 

● Prompt(s) Utilizado(s): 

1.  depois da árvore de Huffman montada. como faço para percorrer a árvore e gerar o código para cada folha, sendo esquerda 0 e direita 1? 

2.  Pode me dar um pseudocódigo de como armazenar esse código gerado em uma tabela para consulta rápida depois ● Resumo da Resposta da IA: A IA confirmou que uma travessia em profundidade (DFS) é ideal. Explicou que devemos manter um buffer de caracteres temporário e incrementar a profundidade a cada chamada recursiva. Forneceu um pseudocódigo mostrando como adicionar '0' ao buffer ao ir para a esquerda e '1' ao ir para a direita, e como copiar o buffer para uma tabela (array de structs) quando uma folha é encontrada. 

● Análise e Aplicação: A lógica de DFS sugerida foi implementada na função 

generate_huffman_codes . O pseudocódigo ajudou a entender que eu precisava passar o índice de profundidade ( top ) como argumento na recursão. Isso resultou no preenchimento correto da estrutura CodeEntry .

Referência no Código: Função generate_huffman_codes em src/huffman.c 

e a estrutura CodeEntry em src/huffman.h .

Interação 6 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de arquivos(Cabeçalho do Arquivo)) 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Definir o formato do arquivo comprimido para garantir que ele possa ser descomprimido. Precisava saber o que é essencial salvar antes dos dados. 

● Prompt(s) Utilizado(s): 

1.  O que preciso gravar no início do arquivo comprimido para conseguir reconstruir a árvore na descompressão? Salvar a árvore inteira ou apenas as frequências? 

2.  Como seria um pseudocódigo para escrever e ler esse cabeçalho em C

● Resumo da Resposta da IA: A IA explicou que salvar a tabela de frequências (256 inteiros) é mais simples do que serializar a árvore. Sugeriu também salvar o tamanho total do arquivo original ( long ) para lidar com o 

padding de bits no final. O pseudocódigo mostrou o uso de fwrite para gravar o long seguido do array frequencies , e fread para o processo inverso. ● Análise e Aplicação: Segui a sugestão de gravar o array de frequências simples. Isso simplificou muito a descompressão, pois basta ler o array e chamar a função build_huffman_tree novamente. A lógica foi aplicada nas funções write_huffman_header e no início de decompress_file .

Referência no Código: Função write_huffman_header e leitura inicial em 

decompress_file no arquivo src/huffman.c .

Interação 7 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de arquivos (Manipulação de Bits) 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Entender como escrever códigos de tamanho variável (ex: 3 bits, 5 bits) no arquivo, já que o sistema operacional só grava bytes completos (8 bits). 

● Prompt(s) Utilizado(s): 

1.  Como faço 'bit packing' em C? Tenho códigos de tamanhos variados e preciso juntá-los em bytes para gravar com fwrite? 

2.  Me explique com pseudocódigo como usar operadores bitwise para preencher um byte buffer. 

● Resumo da Resposta da IA: A IA utilizou a analogia de uma "van" que precisa encher 8 lugares. Explicou o uso de uma variável buffer (byte) e um contador de bits. O pseudocódigo demonstrou como iterar pelos caracteres do código ('0' ou '1'), usar deslocamento de bits ( << ) para posicionar o bit na posição correta e o operador OR ( |) para gravá-lo. Quando o contador chega a 8, faz-se o fwrite e zera o buffer. 

● Análise e Aplicação: Essa foi a parte mais técnica. O pseudocódigo foi essencial para implementar a função compress_data_block . Entendi como tratar o último byte incompleto (padding) escrevendo-o no final se o contador de bits for maior que zero. 

Referência no Código: Lógica de bitwise dentro da função 

compress_data_block em src/huffman.c .Interação 8 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de arquivos (Descompressão) 

● Ferramenta de IA Utilizada: Gemini PRO 

● Objetivo da Consulta: Implementar a lógica inversa da compressão: ler bits e navegar na árvore até encontrar o símbolo original. 

● Prompt(s) Utilizado(s): 

1.  Na descompressão, como leio bit a bit de um byte que acabei de ler do arquivo? 

2.  Qual a lógica para percorrer a árvore Huffman enquanto leio os bits? Pseudocódigo, por favor 

● Resumo da Resposta da IA: A IA explicou que devemos manter um ponteiro para o nó atual ( current_node ), começando na raiz. Para cada bit lido (extraído com máscara de bits >> e & 1), se for 0, movemos para left ;se for 1, para right . O pseudocódigo mostrou que, ao atingir um nó folha (left e right nulos), escrevemos o byte recuperado no arquivo de saída e resetamos o ponteiro para a raiz. 

● Análise e Aplicação: A IA explicou que devemos manter um ponteiro para o nó atual ( current_node ), começando na raiz. Para cada bit lido (extraído com máscara de bits >> e & 1), se for 0, movemos para left ; se for 1, para 

right . O pseudocódigo mostrou que, ao atingir um nó folha ( left e right 

nulos), escrevemos o byte recuperado no arquivo de saída e resetamos o ponteiro para a raiz. 

Referência no Código: Loop principal da função decompress_file em 

src/huffman.c .

Interação 9 

● Data: 28/11/2025 

● Etapa do Projeto: 1 - Compressão de arquivos (Interface e Arquivos (CLI)) 

● Ferramenta de IA Utilizada: Gemini PRO ● Objetivo da Consulta: Criar a interface de linha de comando para receber os arquivos e comandos do usuário. 

● Prompt(s) Utilizado(s): 

1.  Como uso argc e argv em C para pegar o nome do arquivo de entrada e saída? 

2.  Onde os arquivos criados (output) são salvos por padrão se eu não especificar o caminho completo? Como garantir que o programa ache os arquivos? 

● Resumo da Resposta da IA: A IA explicou que argv[0] é o nome do programa e os argumentos começam em argv[1] . Mostrou como comparar strings ( strcmp ) para identificar os comandos ("compactar", "descompactar"). Sobre os arquivos, explicou que se o caminho não for absoluto, eles são salvos no diretório de trabalho atual (onde o terminal está aberto) e sugeriu usar caminhos relativos ou absolutos para evitar erros de "arquivo não encontrado". 

● Análise e Aplicação: Implementei a função main para tratar os argumentos. Usei a explicação sobre diretórios para testar o programa corretamente, entendendo que precisava rodar o executável a partir da pasta correta ou passar o caminho completo dos arquivos .txt e .huf .

Referência no Código: Estrutura de if/else e tratamento de argv no arquivo 

src/main.c .