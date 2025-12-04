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
    int blocos; // Usado só na criacao
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
    Processo(int id, int t0, int prio, int cpu, int mem, int imp, int scan, int mod, int dsk)
    {
        PID = id;
        prioridade_original = prio;
        prioridade_atual = prio; // Comeca igual a original
        t_chegada = t0;
        t_total = cpu;
        t_restante = cpu;
        quantum_restante = 0; // Definido quando vai para a CPU
        blocos_mem_req = mem;
        offset_memoria = -1; // -1 indica que nao alocou ainda
        impressora = imp;

        if (scan == 1)
            scanner = true;
        else
            scanner = false;
        if (mod == 1)
            modem = true;
        else
            modem = false;

        SATA = dsk;
        estado = PRONTO;
        tempo_espera = 0;
        pc = 0;
    }

    // --- Lógica do Quantum Dinâmico ---
    // Implementa a tabela da especificação
    void resetar_quantum()
    {
        if (prioridade_atual == 0)
        {
            quantum_restante = 10000; // Tempo Real nao sofre preempção por tempo, motivo do valor alto
        }
        else if (prioridade_atual == 1)
        {
            quantum_restante = 6;
        }
        else if (prioridade_atual == 2)
        {
            quantum_restante = 5;
        }
        else if (prioridade_atual == 3)
        {
            quantum_restante = 4;
        }
        else if (prioridade_atual == 4)
        {
            quantum_restante = 3;
        }
        else
        {
            quantum_restante = 2;
        }
    }
};

#endif