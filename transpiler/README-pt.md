# SB-TRADUTOR

**Para a versão em inglês, leia o [README.md](README.md).**

Tradutor de código fonte do Assembly Inventado para Assembly IA-32. Projeto desenvolvido em C++ com operações de entrada e saída em Assembly IA-32.

## Autores

Gustavo Santana Lima - 211038235
Yan Tavares - 202014323

## Uso

### Opção 1 - Usando o Makefile

Para utilizar o tradutor, basta executar o comando abaixo:

```bash
make FILE=<arquivo>
```

Onde `<arquivo>` é o arquivo que contém o código fonte **sem extensão**.

**OBS:** O arquivo **deve estar** dentro do diretório `arquivos`.

## Exemplo

Para traduzir o arquivo `in_out_var.obj` dentro do diretório `arquivos`, basta executar o comando abaixo:

```bash
make FILE=in_out_var
```

O comando irá gerar um arquivo `in_out_var.asm` no diretório `output`, irá montar e executar o código gerado.

### Opção 2 - Compilando e executando manualmente

Para compilar o tradutor, basta executar o comando abaixo:

```bash
g++ -o tradutor Tradutor.cpp
```

Para executar o tradutor, basta executar o comando abaixo:

```bash
./tradutor <arquivo>.obj
```

Onde `<arquivo>` é o arquivo `.obj` que contém o código fonte.

Para montar, ligar e executar o código gerado, basta executar o comando abaixo:

```bash
nasm -f elf32 <arquivo>.asm -o <arquivo>.o
ld -m elf_i386 -o <arquivo> <arquivo>.o
./<arquivo>
```

O arquivo executável imprimirá quantos bytes foram lidos e escritos no terminal.

**OBS.:** A quantidade de bytes lidos leva em consideração o caractere de nova linha (`\n`).

## Estrutura do projeto

```
SB-TRADUTOR
│ README.md
│ Makefile
│ Tradutor.cpp
│ Tradutor.hpp
│ io.asm
│
├── bin*
│ │ tradutor
│ │ arquivo1
│
├── output*
│ │ arquivo1.asm
│ │ arquivo1.o
│
├── Arquivos
│ │ arquivo1.obj
│ │ ...
│
```

Diretórios marcados com `*` são gerados automaticamente pelo Makefile.

## Contato

Se você quiser entrar em contato, envie um e-mail para qualquer um dos autores:

- [yantdo1@gmail.com](mailto:yantdo1@gmail.com)
- [guts.santana@gmail.com](mailto:guts.santana@gmail.com)
