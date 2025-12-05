# Pseudo-SO Multiprogramado - Universidade de Brasília (UnB)

Este projeto consiste na implementação de um **Pseudo-Sistema Operacional Multiprogramado** como parte da avaliação prática da disciplina de **Sistemas Operacionais**, ministrada pela Profa. Aletéia Patrícia Favacho de Araújo.

O sistema simula a execução concorrente de processos, gerenciando memória, recursos de E/S e sistemas de arquivos, utilizando uma abordagem de arquitetura monolítica modular.

## Equipe
* **Aluno 1:** [Caio César Gonçalves Ribeiro] - [21/1038217]
* **Aluno 2:** [João Victor Cavallin Pereira] - [21/2008894]
* **Aluno 3:** [Jhonatan Borges de Souza] - [18/0122975]

## Tecnologias Utilizadas
* **Linguagem:** C++ (Padrão C++11 ou superior)
* **Documentação:** Doxygen
* **Ambiente de Desenvolvimento:** Unix/Linux

## Estrutura do Sistema
O projeto está dividido nos 5 módulos fundamentais exigidos:
1.  **Processos:** Gerencia a criação e o contexto dos processos.
2.  **Filas:** Implementa o escalonador com Múltiplas Filas com Realimentação (Feedback) e Aging.
    * 1 Fila de Tempo Real (FIFO, prioridade 0).
    * 5 Filas de Usuário (Round-Robin com quantum dinâmico).
3.  **Memória:** Gerencia 1024 blocos de memória contígua (64 reservados para Tempo Real, 960 para Usuário).
4.  **Recursos:** Gerencia a exclusão mútua de dispositivos (Scanner, Impressoras, Modem, SATA).
5.  **Arquivos:** Simula operações de criação/deleção (Alocação contígua, método First-fit).

## Compilação e Execução

### Pré-requisitos
* Compilador `g++` (GCC).
* `make` (Opcional, caso utilize Makefile).
* `doxygen` (Para gerar a documentação).

### Como Compilar - Usando Make (recomendado)

```bash
make
```

### Como Executar 

```bash
./main
```