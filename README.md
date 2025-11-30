Este projeto implementa uma ferramenta de linha de comando (CLI) para processar e comprimir arquivos de texto grandes, seguindo a restrição de baixo e constante uso de memória RAM.

O algoritmo de compressão utilizado é o Huffman, com a lógica da Min-Heap modularizada para garantir a organização do código.

Como Compilar e Rodar o Programa
1. Requisitos
Compilador GCC (GNU Compiler Collection).

Ambiente de linha de comando (PowerShell, CMD, ou terminal Linux/macOS).

2. Compilação (Build)
Para compilar o projeto, você deve incluir todos os arquivos .c na pasta src/, garantindo que o main.c, huffman.c e min_heap.c sejam corretamente compilados e ligados (linked) para criar o executável.

Abra o terminal no diretório raiz do projeto e execute:

gcc src/main.c src/huffman.c src/min_heap.c -o meu_programa

3. Comando de COMPACTAR
A função compactar lê um arquivo grande e gera um arquivo binário comprimido (.huf), que inclui o cabeçalho e a tabela de frequências.

execute:

./meu_programa compactar <arquivo_original> <arquivo_compactado>

(O ARQUIVO DE DESTINO (compactado) NÃO PRECISA SER CRIADO ANTES DE RODAR O COMANDO, ELE SERÁ CRIADO APÓS EXECUTAR)

IMPORTANTE* se o arquivo original não estiver nao mesmo diretório do projeto, é necessário adicionar o caminho do arquivo entre aspas:

.\meu_programa.exe compactar "C:\caminho\ate\o\arquivo.txt" teste1_compactado.huf

(o arquivo compactado será gerado no diretório do projeto)

4. Comando de DESCOMPACTAR
A função descompactar lê o arquivo comprimido, reconstrói a árvore de Huffman e restaura o conteúdo original, salvando-o em um novo arquivo de texto.

Execute:

./meu_programa descompactar <arquivo_compactado> <arquivo_destino>

(O ARQUIVO DE DESTINO NÃO PRECISA SER CRIADO ANTES DE RODAR O COMANDO, ELE SERÁ CRIADO APÓS EXECUTAR)
