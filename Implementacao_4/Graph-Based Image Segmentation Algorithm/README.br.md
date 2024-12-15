# Projeto de Segmentação de Imagem

## Descrição

Este projeto implementa segmentação de imagem utilizando Python e C++. O script em Python processa as imagens, converte-as para escala de cinza e gera um arquivo no formato PGM (Portable GrayMap). O programa em C++ gerencia a execução do script Python e fornece uma estrutura para rodar o fluxo de trabalho de processamento de imagens.

## Como Executar o Código

1. **Pré-requisitos**

    Compilador C++: Você precisa ter um compilador C++ instalado (por exemplo, g++).

    Python: Certifique-se de que o Python está instalado no seu sistema e acessível pelo terminal. O script é compatível tanto com os comandos `python` quanto `python3`.

    Você pode verificar se o Python está instalado executando:

    ```bash
    python --version
    ```

    ou

    ```bash
    python3 --version
    ```
    
    Se o Python não estiver instalado, você pode baixá-lo e instalá-lo em [python.org](https://www.python.org).

2. **Instruções**:
    Primeiro, compile o arquivo `code.cpp` executando o seguinte comando no seu terminal:
    ```bash
    g++ code.cpp -o code
    ```


    Após compilar o código C++, você pode executar o executável:
    ```bash
    ./code
    ```

    Isso executará o código C++, permitindo que você selecione uma das imagens na pasta. O algoritmo será executado e o resultado será gerado na pasta `images`:

    - A imagem é convertida para escala de cinza e o resultado é exibido em `result_gray.png`.
    - O arquivo PGM da imagem em escala de cinza é gerado para uso interno, mas ainda é acessível em `original.pgm`.
    - Uma imagem que marca diferentes regiões com cores distintas é exibida em `converted.png`.
    - O resultado também pode ser visualizado em formato PGM em `converted.pgm`, onde cada valor numérico representa um grupo diferente.


## Testar Imagens Adicionais

Você também pode testar o resultado do programa em imagens adicionais que incluir na pasta images.

Diferentes valores de k para o algoritmo são recomendados para diferentes tamanhos de imagem. Por padrão, os valores de k são os seguintes:

    soma da altura e largura <= 260: k = 150
    soma da altura e largura <= 600: k = 300
    caso contrário: k = 600

No entanto, para resultados mais robustos, diferentes valores de k podem ser usados alterando o valor do limiar específico no arquivo code.cpp.

    