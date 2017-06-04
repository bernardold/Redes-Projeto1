# Sensores via sockets

Projeto 1 da disciplina de Redes de Computadores do Instituto de Ciências Matemáticas e de Computação (ICMC-USP).

Desenvolvido por:
+ Bernardo Simões Lage Gomes Duarte (8598861)
+ Bruno Lanzoni Rossi (4309596)
+ Giovani Ortolani Barbosa  (8936648)
+ Renan Rodrigues (9278132)

## Requisitos

```
gcc
```

## Compilação e execução
Compilação e execução pode ser feita por Makefile. O seguinte comando é suficiente para compilar os códigos do servidor e dos clientes.
```
$ make
```
### Executando Servidor
O comando a seguir executa o servidor, que imediatamente espera por conexões dos clientes.
```
$ ./central
```
### Clientes
Cada cliente é uma abstração de um sensor diferente, o comando a seguir executa os clientes que já iniciam automaticamente a conexão com o servidor. 
```
$ make run
```
### Observações
+ A execução dos clientes deve acontecer em uma nova janela do terminal.
+ O comando de execução dos clientes executa todos em paralelo em uma mesma janela do terminal
