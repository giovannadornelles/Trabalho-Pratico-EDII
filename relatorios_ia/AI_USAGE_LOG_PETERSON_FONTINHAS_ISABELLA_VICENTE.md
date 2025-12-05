# Relatório de Uso de Inteligencia Artificial - Peterson Fontinhas

### Registro de Interações

### Interação 1
- Data: 04/12/2025
- Etapa do Projeto : 3 - Busca de Substring em Arquivo Comprimido 
- Ferramenta de IA utilizada: GPT 5.1
- Objetivo da Consulta: Eu estava querendo entender melhor sobre como fazer algorítmos de busca de subtring em arquivos de texo compactado e pedi fontes de vídeos explicando como fazer esse tipo de algoritmo. Até recebi vídeos interessantes mas não eram úteis para o que eu estava procurando, então pedi para o GPT como eu poderia realizar uma pesquisa de substring em arquivo compactado
- Prompt(s) Utilizado(s):
    1. "Você pode encontrar vídeos que ensinam sobre a pesquisa de substring em arquivo de texto compactado em C?"
    2. "Você tem noção de como fazer substring search em arquivos de texto compactados?"

- Resumo da Resposta da IA: Ela me enviou vídeos explicando como buscas de substring funcionam, bem como, quando eu perguntei se ela tinha conhecimento sobre pesquisa em arquivos de texto compactados, ela me gerou um pseudo-código, passando pela estrutura necessária para o mapeamento dos blocos de substring até a pesquisa de fato.
- Análise e Aplicação: Eu apliquei o pseudo-código em um novo arquivo e fui adaptando ao projeto atual. O código fornecido pela IA facilitou muito o entendimento e agilizou a aplicação do algortmo no sistema.
- Referência no código: A lógica inspirada por essa interação foi implementada nos arquivos `src/search.h` e `src/search.c`
---

### Interação 2
- Data: 04/12/2025
- Etapa do Projeto : 3 - Busca de Substring em Arquivo Comprimido 
- Ferramenta de IA utilizada: GPT 5.1
- Objetivo da Consulta: Depois de implementar o código de pesquisa ao sistema, tive que fazer a função de descomprimir apenas o bloco escolhido. Me baseando na função já existente `decompress_file(parameters)` do arquivo `src/huffman.c`, perguntei como para a IA como eu faria a descompressão de apenas o bloco escolhido e não do arquivo inteiro. No meio tempo entre a primeira e a segunda interação, a IA também falou sobre o algoritmo de busca do KMP, então resolvi pedir um exemplo da aplicação desse algoritmo
- Prompt(s) Utilizado(s):
    1. "Você poderia me explicar como eu faria a descompressão de apenas o bloco escolhido? Eu poderia usar essa função decompress_file?"
    2. "Você mencionou o KMP para pesquisa de substring, como eu aplicaria isso?"

- Resumo da Resposta da IA: Ela me falou sobre a estrutura necessária para a descompressão do arquivo e disse que era bastante parecida com a que já existia, e me gerou um exemplo de descompressão em blocos para que eu pudesse adaptar ao código. Depois da função de descompressão, ela me falou mais sobre o algoritmo KMP e a tabela LPS
- Análise e Aplicação: Fui adaptando o código de descompressão no arquivo me baseando no código gerado pela IA. Após isso apliquei o algoritmo de busca em um arquivo separado
- Referência no código: A lógica inspirada por essa interação foi implementada nos arquivos `src/huffman.c` e `src/kmp.c` e `src/kmp.h`
---

### Interação 3
- Data: 04/12/2025
- Etapa do Projeto : 3 - Busca de Substring em Arquivo Comprimido 
- Ferramenta de IA utilizada: GPT 5.1
- Objetivo da Consulta: Depois de conseguir aplicar os algoritimos de busca e descompressão, a busca ainda não funcionava como devia. Sem entender o por que, pedi para a IA dar uma olhada nos algoritmos do sistema para me dizer o que estava acontecendo que pesquisa não funcionava
- Prompt(s) Utilizado(s):
    1. "O código roda na main, mas não aparece nada no terminal"

- Resumo da Resposta da IA: Ela deu uma breve olhada nos algoritmos já existentes e pontou um tópico crucial para o funcionamento do sistema. A função de compressão de arquivos `compress_data_block()` não separava o arquivo por blocos, e por causa disso a função de busca não funcionava, porque não achava nenhum bloco no índice. Então ela me gerou as alterações necessárias para aplicar na função e conseguir gerar os blocos e inserir no arquivo compactado
- Análise e Aplicação: Realizei as alterações necessárias para que a compressão do arquivo seja feita corretamente
- Referência no código: A lógica inspirada por essa interação foi implementada nos arquivos `src/huffman.c`
---
### Interação 4...
- Data: 04/12/2025
- Etapa do Projeto : 3 - Busca de Substring em Arquivo Comprimido 
- Ferramenta de IA utilizada: GPT 5.1
- Objetivo da Consulta: A partir da quarta interação, a fim de estudar o algoritmo completo, realizei várias perguntas para o GPT, sobre como o a função funcionava, como cada parte do algoritmo funcionava. Realizei perguntas pontuais sobre funções como `fseek()` `fread()` e etc. Com isso, fui gerando vários comentários ao longo do meu código para ver se eu tinha entendido o algoritmo de fato. E a cada comentário que eu fazia sobre um bloco de código, eu mandava para a IA para ela validar minha explicação do conceito e dizer que tinha acertado ou não 
- Prompt(s) Utilizado(s):
    1. "Pode me explicar de fato como ela função da tabela LPS funciona? Como o KMP se liga a ela"
    2. "Pode me explicar como essa função (`fseek`, `fread`...)" funciona de fato?
    3. "Realizei comentários sobre a função, pode me dizer se cheguei perto do conceito real?
    3. "Por que essa função está sendo usada aqui, e não essa?

- A IA foi me gerando explicações cada vez mais detalhadas para cada pergunta que eu fizesse e sempre corrigia meus comentários se eles não fossem tão precisos. Em certo momento até indicou uma alteração na função `compress_data_block()` que inicialmente alocava um tamanho de memória fixo, o que poderia causar um gasto de memória que não fazia sentido para arquivos de texto pequenos. Então me deu as instruções para a junção de `malloc` com `realloc`, para que o tamanho da memória usada seja alterado apenas quando a capacidade atingir seu ponto máximo 
- Análise e Aplicação: Para cada comentário e explicação eu realizava meus próprios comentários ao longo de toda a aplicação feita por mim, deixando a parte das minhas colegas para elas realizarem seus comentários. Cada explicação e correção melhorava meu entendimento do código
- Referência no código: A lógica inspirada por essa interação foi implementada nos arquivos `src/huffman.c` `src/search.c` `src/kmp.c`
---
