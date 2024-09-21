# Trabalho 1 - Montador e Ligador de uma linguagem assembly inventada

**Para a versão em inglês, leia o [README.md](README.md).**

## Autores

Gustavo Santana Lima - 211038235
Yan Tavares - 202014323

## Índice

- [Uso](#Uso)
- [Observações](#Observações)
- [Simulação](#Simulando-a-execução-do-arquivo-obj)
- [Estrutura do projeto](#Estrutura-do-projeto)
- [Contato](#Contato)

## Uso

### Opção 1 - Usando `make` para compilar os arquivos e executar manualmente:

```bash
make
```

Isso irá criar o arquivo executável `montador`. Com ele, é possível montar e ligar arquivos `.asm` e `.obj`:

1. Para visualizar todas as opções disponíveis, execute:

```bash
./montador --help
```

ou

```bash
./montador -h
```

2. Para **apenas** pré-processar um arquivo `.asm` e gerar um arquivo `.pre`, execute:

```bash
./montador -p <arquivo.asm>

```

3. Para **apenas** montar um arquivo pré processado `.pre` e gerar um arquivo `.obj` (ou `.e`, no caso de arquivos ligáveis), execute:

```bash
./montador -o <arquivo.asm>
```

4. Para **apenas** ligar dois arquivos `.e` e gerar um arquivo `.obj` resultante, execute:

```bash
./montador -l <arquivo1.e> <arquivo2.e>
```

O arquivo `obj` terá o nome do primeiro arquivo passado como argumento + `_linked.obj`

Obs.: Você também pode utilizar o arquivo `montador` pré compilado no diretório `bin/`.

### Opção 2 - Usando `make` para compilar e executar os arquivos automaticamente:

1. Para compilar o montador, gerar o arquivo `.pre` e montar um arquivo `.asm`, execute:

```bash
make as file=<arquivo.asm>
```

2. Para compilar o montador, gerar o arquivo `.pre`, montar o arquivo `.e` e ligar dois arquivos, gerando o `obj` resultante, execute:

```bash
make ld file1=<arquivo1.asm> file2=<arquivo2.asm>
```

O arquivo `obj` terá o nome do primeiro arquivo passado como argumento + `_linked.obj`

3. Para limpar os arquivos `.o` gerados, execute:

```bash
make clean
```

## Observações

- Os arquivos gerados pelo montador são salvos na mesma pasta do arquivo de entrada.
- O montador e ligador foram testados nos ambientes (Ubuntu 20.04, POP_OS 22.04 e WSL no Windows 11).
- Os arquivos utilizados para teste podem ser encontrados na pasta `Arquivos/`.
- Diretiva `CONST` aceita números decimais e hexadecimais.

## Simulando a execução do arquivo `.obj`

Para simular o processamento de um arquivo `.obj`, é necessário utilizar o simulador. Para isso, execute:

```bash
./bin/simulador <arquivo.obj>
```

## Estrutura do projeto

```
TRABALHO1
│ README.md
│ makefile
│ main.cpp
│
├── bin
│ │ montador
│ │ simulador
│
├── src
│ │ LIGADOR.cpp
│ │ MONTADOR.cpp
│ │ PRE_PROCESSADOR.cpp
│
├── include
│ │ LIGADOR.hpp
│ │ MONTADOR.hpp
│ │ PRE_PROCESSADOR.hpp
│
├── docs
│ │ documentation.md
│
├── Arquivos
│ │ arquivo1.asm
│ │ ...
│
```

## Contato

Se você quiser entrar em contato, envie um e-mail para qualquer um dos autores:

- [yantdo1@gmail.com](mailto:yantdo1@gmail.com)
- [guts.santana@gmail.com](mailto:guts.santana@gmail.com)
