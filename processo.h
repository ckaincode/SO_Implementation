#ifndef PROCESSO_H
#define PROCESSO_H

#include <vector>
#include <string>

// Armazena as operações de arquivos (files.txt)
struct Instrucao
{
    int pid;
    int codigo; // (0)Criar, (1)Deletar
    std::string arquivo;
    int blocos;    // Usado só na criacao
    int id_global; // Identificador global
};

// Estados possíveis do processo
enum Estado
{
    PRONTO,
    EXECUTANDO,
    BLOQUEADO,
    FINALIZADO
};

class Processo
{
public:
    int PID;                 // ID do processo(unico)
    int prioridade_original; // Prioridade que veio no .txt, definine se é tempo real(0) ou usuário(1-5)
    int prioridade_atual;    // Prioridade que muda dinamicamente, utilizamos aging para alterar essa prioridade

    int t_chegada;
    int t_total;
    int t_restante;       // Quanto falta para acabar o processo
    int quantum_restante; // Quanto falta para acabar a vez dele na CPU

    // Dados de Memoria
    int blocos_mem_req;
    int offset_memoria; // Se for -1, nao está na memória

    // Dados de Hardware
    int impressora;
    bool scanner;
    bool modem;
    int SATA;

    Estado estado;
    int tempo_espera; // Contador para o Aging

    // Lista de instrucoes desse processo
    std::vector<Instrucao> instrucoes_io;
    int pc; // Contador

    // Construtor inicializando as variaveis
    Processo(int id, int t0, int prio, int cpu, int mem, int imp, int scan, int mod, int dsk);

    // --- Lógica do Quantum Dinâmico ---
    // Implementa a tabela da especificação
    void resetar_quantum();

};

#endif