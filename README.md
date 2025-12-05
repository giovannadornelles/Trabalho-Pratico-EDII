# Sistema de processamento de arquivos grandes
---
***Integrantes***: 
- Clarissa Morita
- Giovanna Dornelles
- Isabella Vicente
- Peterson Fontinhas
---
 Este projeto implementa uma ferramenta de linha de comando (CLI) para processar e comprimir arquivos de texto grandes, seguindo a restrição de baixo e constante uso de memória RAM.

O algoritmo de compressão utilizado é o Huffman, com a lógica da Min-Heap modularizada para garantir a organização do código.

***Como Compilar e Rodar o Programa***
- **Requisitos**
  - Compilador GCC (GNU Compiler Collection).

  - Ambiente de linha de comando (PowerShell, CMD, ou terminal Linux/macOS).
  
  - Programa Make (útil para uma compilação mais rápida), para executar arquivos `makefile`
  

Abra o terminal no diretório raiz do projeto e execute:

- Se você possuir o Make
   ```sh
    make
    ```
- Se não possuir
   ```sh
    gcc main.c src/huffman.c src/min_heap.c src/search.c src/kmp.c -o programa
    ```


## ***Comandos***
- Compactar
    - A função compactar lê um arquivo grande e gera um arquivo binário comprimido (.huf), que inclui o cabeçalho e a tabela de frequências.

    - execute:

       ```sh
        ./programa.exe compactar <arquivo_original> <arquivo_compactado>
        ```    

    - (O ARQUIVO DE DESTINO (compactado) NÃO PRECISA SER CRIADO ANTES DE RODAR O COMANDO, ELE SERÁ CRIADO APÓS EXECUTAR)

    - IMPORTANTE* se o arquivo original não estiver nao mesmo diretório do projeto, é necessário adicionar o caminho do arquivo entre aspas:

       ```sh
        .\programa.exe compactar "C:\caminho\ate\o\arquivo.txt" teste1_compactado.huf
        ```    
        - (o arquivo compactado será gerado no diretório do projeto)
        

- Descompactar
    - A função descompactar lê o arquivo comprimido, reconstrói a árvore de Huffman e restaura o conteúdo original, salvando-o em um novo arquivo de texto.

    - execute:

       ```sh
         ./programa.exe descompactar <arquivo_compactado> <arquivo_destino>
        ```    

    - (O ARQUIVO DE DESTINO NÃO PRECISA SER CRIADO ANTES DE RODAR O COMANDO, ELE SERÁ CRIADO APÓS EXECUTAR)

- Buscar em arquivo compactado
    - A função de buscar em arquivo compactado, pega os blocos do arquivo compactado, desompacta o bloco e procura o padrão no bloco descompactado, se não houver match, passa para o próximo bloco até o fim do arquivo. Pode achar zero ou muitos matchs

    - execute:

       ```sh
         ./programa.exe buscar_compactado <arquivo_compactado> "substring"
        ```    



