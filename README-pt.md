# Toolkit de Assembly Hipotético

**Para a versão em inglês, leia o [README.md](README.md).**

## Visão Geral

O Toolkit de Assembly Hipotético é uma solução abrangente para trabalhar com a linguagem assembly liasm e transpilá-la para assembly IA-32. O toolkit inclui dois componentes principais:

1. **Assembler (+ linker)**: Converte o código-fonte liasm em código de máquina para a arquitetura hipotética. Também liga 2 arquivos-objeto em um único executável.
2. **Transpilador**: Transpila o código assembly liasm para linguagem assembly IA-32.
3. **Assembler de duas passagens**: Uma versão do assembler que opera em duas passagens, feito por diversão (não faz parte da disciplina).

Este repositório serve como um ponto de entrada central, fornecendo documentação detalhada e instruções de uso para ambas as ferramentas.

## Linguagem Assembly liasm

O ambiente liasm emula uma arquitetura de computador simples com os seguintes componentes principais:

- **ACC**: Um registrador acumulador de 16 bits para operações aritméticas e lógicas.
- **PC**: Um registrador de contador de programa de 16 bits que aponta para a próxima instrução.
- **Memória**: Uma matriz de memória com \( 2^{16} \) palavras, cada uma com 16 bits.

### Formatos de Instrução

A linguagem liasm suporta três formatos de instrução:

1. **Formato 1**: Apenas o Opcode
2. **Formato 2**: Opcode seguido por um endereço
3. **Formato 3**: Opcode seguido por dois endereços

Este conjunto de instruções, embora minimalista, é completo, permitindo a implementação de diversos algoritmos, tornando o liasm uma linguagem Turing-completa.

### Conjunto de Instruções

| Mnemônico  | Opcode | Tamanho (palavras) | Ação                                                    |
|------------|--------|--------------------|---------------------------------------------------------|
| ADD        | 01     | 2                  | `ACC <- ACC + mem(OP)`                                  |
| SUB        | 02     | 2                  | `ACC <- ACC - mem(OP)`                                  |
| MUL        | 03     | 2                  | `ACC <- ACC * mem(OP)`                                  |
| DIV        | 04     | 2                  | `ACC <- ACC ÷ mem(OP)`                                  |
| JMP        | 05     | 2                  | `PC <- OP`                                              |
| JMPN       | 06     | 2                  | `Se ACC<0 então PC <- OP`                                |
| JMPP       | 07     | 2                  | `Se ACC>0 então PC <- OP`                                |
| JMPZ       | 08     | 2                  | `Se ACC=0 então PC <- OP`                                |
| COPY       | 09     | 3                  | `mem(OP2) <- mem(OP1)`                                  |
| LOAD       | 10     | 2                  | `ACC <- mem(OP)`                                        |
| STORE      | 11     | 2                  | `mem(OP) <- ACC`                                        |
| INPUT      | 12     | 2                  | `mem(OP) <- entrada`                                    |
| OUTPUT     | 13     | 2                  | `saída <- mem(OP)`                                      |
| STOP       | 14     | 1                  | Suspende a execução                                     |
| S_INPUT*   | 15     | 1                  | Recebe um rótulo para entrada e número de bytes a ler   |
| S_OUTPUT*  | 16     | 1                  | Recebe um rótulo para saída e número de bytes a escrever|

\* *S_INPUT e S_OUTPUT são chamadas de sistema que interagem com o transpilador e não funcionam no assembler.*

### Diretivas

- **CONST**: Aloca um valor constante para um endereço de memória.  
  Exemplo: `N3: CONST 1 ; Aloca 1 para o endereço N3`

- **SPACE**: Aloca espaço de memória para um endereço sem inicializá-lo.  
  Exemplo: `N1: SPACE ; Aloca memória para o endereço N1`

### Comentários

Comentários no liasm começam com um ponto e vírgula (`;`). Tudo o que segue o `;` em uma linha é considerado um comentário e é ignorado pelo assembler.

## Componentes do Toolkit

### 1. Assembler
O assembler traduz o código-fonte liasm para o código de máquina da arquitetura hipotética. Ele opera em um processo de compilação de passagem única, garantindo uma conversão direta e eficiente.

**[Assembler README](assembler/README.md)**: Informações detalhadas sobre o assembler podem ser encontradas na pasta `assembler`.

Para a versão de duas passagens do assembler, consulte [Two Pass Assembler README](two-pass-assembler/README.md).

### 2. Transpilador
O transpilador converte o código assembly liasm em linguagem assembly IA-32, permitindo que programas hipotéticos sejam executados em arquiteturas x86. As operações de entrada e saída no código transpilado são implementadas usando instruções assembly IA-32, garantindo compatibilidade com os mecanismos de I/O padrão do x86.

**[Transpilador README](transpiler/README.md)**: Informações detalhadas sobre o transpilador podem ser encontradas na pasta `transpiler`.

## Fluxo de Compilação

1. **Compilação de Passagem Única**: O assembler processa o código-fonte liasm em uma única passagem, gerando o código de máquina para a arquitetura hipotética.
2. **Transpilar**: O transpilador converte o assembly liasm para IA-32, preservando a lógica e a estrutura do código original, e implementando as operações de entrada/saída utilizando o assembly IA-32.

## Começando

Para começar, consulte os arquivos README nas respectivas pastas:

- [Assembler README](assembler/README.md)
- [Transpilador README](transpiler/README.md)

Esses documentos fornecem instruções detalhadas sobre como compilar, configurar e usar as ferramentas.

## Licença

Este projeto é licenciado sob a Licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## Agradecimentos

Agradecimentos especiais a [Guts_Santana](https://github.com/Guts-Santana) por tornar este projeto possível.
