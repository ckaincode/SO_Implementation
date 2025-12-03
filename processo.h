#ifndef PROCESSO_H
#define PROCESSO_H

#include <vector>
#include <string>

struct Instrucao
{
    int pid;
    int codigo; // 0 = Criar, 1 = Deletar
    std::string arquivo;
    int blocos; // Usado so na criacao
};

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
    int PID;
    int prioridade_original; // Prioridade que veio no .txt
    int prioridade_atual;    // Prioridade que muda (Feedback)

    int t_chegada;
    int t_total;
    int t_restante;       // Quanto falta para acabar o processo
    int quantum_restante; // Quanto falta para acabar a vez dele na CPU

    // Dados de Memoria
    int blocos_mem_req;
    int offset_memoria; // Se for -1, nao esta na memoria

    // Dados de Hardware
    int impressora;
    bool scanner;
    bool modem;
    int disco;

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
        quantum_restante = 0;
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

        disco = dsk;
        estado = PRONTO;
        tempo_espera = 0;
        pc = 0;
    }

    // Implementa a tabela da especificação
    void resetar_quantum()
    {
        if (prioridade_atual == 0)
        {
            quantum_restante = 10000; // Tempo Real nao acaba por tempo, motivo do valor alto
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