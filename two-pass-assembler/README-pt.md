# Linguagem Inventada - liasm (PT-BR)

<img src="utils/us.png" height="20px" width="auto" alt="Bandeira EUA"  /> Para a versão em inglês, clique [aqui](README.md).

Bem-vindo ao repositório da Linguagem Inventada, uma simulação de máquina que permite montar e executar programas escritos na "Linguagem Inventada" ou `liasm`, criada pela Universidade de Brasília (UnB).

## Fluxo de Trabalho do Compilador

O compilador `liasm` opera em duas passagens:

1. **Primeira Passagem**: O compilador lê o arquivo fonte `.liasm` para construir uma tabela de rótulos, que é usada para resolver endereços durante o processo de montagem. Durante esta fase, o compilador também realiza análise léxica para identificar quaisquer erros de sintaxe no código fonte.

2. **Segunda Passagem**: Com a tabela de rótulos construída, o compilador lê o arquivo fonte uma segunda vez, desta vez traduzindo as instruções de montagem

para o código binário. O compilador também realiza verificações semânticas para garantir a integridade do programa, como validar formatos de instrução e detectar referências indefinidas.

## Estrutura do Projeto

Aqui está uma visão geral da estrutura do projeto para ajudá-lo a navegar e entender seus componentes:

- **`liasm/`**: O diretório principal contendo todos os recursos necessários para o montador e simulação.
  - **`bin/`**: Armazena os binários compilados do montador.
  - **`examples/`**: Contém exemplos de código de Montagem Inventiva para testes e aprendizado.
  - **`includes/`**: Cabeçalhos de bibliotecas usadas pelo montador.
  - **`src/`**: Código fonte do montador.
  - **`tests/`**: Testes automatizados.
  - **`utils/`**: Ferramentas auxiliares.
  - **`compiler.cpp`**: O arquivo principal do compilador.
  - **`Makefile`**: Para compilar o projeto usando `make`.
  - **`MEMdata.txt`** e **`MEMtext.txt`**: Simulam arquivos de memória de dados e de instruções, respectivamente.
- **`.gitignore`**: Lista de arquivos/diretórios a ignorar na versionamento.
- **`README.md`**: Documentação (este arquivo).

## Primeiros Passos

### Pré-requisitos

Antes de começar, certifique-se de que as seguintes ferramentas estão instaladas no seu sistema:

- **`make`**: Utilitário para automação de compilação.
- **`g++`**: O compilador C++ da Coleção de Compiladores GNU (GCC).

### Montando e Executando Programas com `licc`

Para compilar o montador `licc`:

1. **Navegue até o diretório do projeto**:

   ```bash
   cd liasm/
   ```

2. **Compile o montador** usando o comando `make`:

   ```bash
   make
   ```

Ou você pode apenas usar o binário pré-compilado no diretório `liasm`.

#### Usando Makefile

Após compilar o montador:

- **Para montar um programa**, substitua `<caminho do arquivo>` pelo seu arquivo `.asm`:

  ```bash
  ./licc <caminho do arquivo>
  ```

- **Opcional**: Você também pode definir o modo para `debug` para ver os rótulos gerados e a saída verbosa.

  ```bash
  ./licc <caminho do arquivo> debug
  ```

#### Execução Direta com `make run`

- **Para compilar o montador e executar diretamente**, substitua `<Caminho do arquivo>` pelo caminho do seu arquivo `.asm`:

  ```bash
  make run FILE=<Caminho do arquivo>
  ```

- **Opcional**: Você também pode definir o modo para `debug` para ver os rótulos gerados e a saída verbosa.

  ```bash
  make run FILE=<Caminho do arquivo> MODE=debug
  ```

### Executando Testes

Para garantir que o montador está funcionando corretamente:

```bash
make test
```

Este comando compila o montador e executa todos os testes na pasta `tests/`.

## Licença

Este projeto está sob a UNLICENSE, permitindo total liberdade para uso, modificação e distribuição.
