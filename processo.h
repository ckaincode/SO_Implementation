#ifndef PROCESSO_H
#define PROCESSO_H

#include <vector>
#include <string>

// Estrutura que representa uma instrução de I/O
struct Instrucao
{
    int pid;
    int codigo; // (0)Criar, (1)Deletar
    std::string arquivo;
    int blocos;
    int id_global;
};
// Enumeração para os estados do processo
enum Estado
{
    PRONTO,
    EXECUTANDO,
    BLOQUEADO,
    FINALIZADO
};
/**
 * @brief Representa uma entidade de processo no pseudo-so.
 *
 * Armazena informações essenciais do processo, como PID, prioridades, tempos de execução,
 * requisitos de memória e hardware, estado atual, e lista de instruções de I/O.
 */
class Processo
{
public:
    int PID;
    int prioridade_original; // Prioridade que veio no .txt, define se é tempo real(0) ou usuário(1-5)
    int prioridade_atual;    // Prioridade que muda dinamicamente, utilizamos aging para alterar essa prioridade

    int t_chegada;
    int t_total;
    int t_restante;
    int quantum_restante;

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
    int pc; // Program Counter para instrucoes de I/O

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

    /**
     * @brief Reseta o quantum restante baseado na prioridade atual do processo.
     * Define o quantum conforme as regras:
     * - Prioridade 0 (Tempo Real): 10000 (não sofre preempção por tempo)
     * - Prioridade 1: 6 unidades de tempo
     * - Prioridade 2: 5 unidades de tempo
     * - Prioridade 3: 4 unidades de tempo
     * - Prioridade 4: 3 unidades de tempo
     * - Prioridade 5: 2 unidades de tempo
     */
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