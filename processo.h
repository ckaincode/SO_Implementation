#ifndef PROCESSO_H
#define PROCESSO_H

#include <vector>
#include <string>

/**
 * @brief Representa uma instrução de I/O do processo.
 */
struct Instrucao
{
    int pid;             ///< PID do processo dono da instrução.
    int codigo;          ///< Código da operação: 0 = Criar, 1 = Deletar.
    std::string arquivo; ///< Nome do arquivo a manipular.
    int blocos;          ///< Número de blocos envolvidos.
    int id_global;       ///< ID único da instrução.
};

/**
 * @brief Estados possíveis de um processo.
 */
enum Estado
{
    PRONTO,
    EXECUTANDO,
    BLOQUEADO,
    FINALIZADO
};

/**
 * @brief Representa um processo no pseudo-SO, incluindo prioridades,
 * recursos, estado e lista de instruções de I/O.
 */
class Processo
{
public:
    int PID;
    int prioridade_original;
    int prioridade_atual;

    int t_chegada;
    int t_total;
    int t_restante;
    int quantum_restante;

    int blocos_mem_req;
    int offset_memoria;

    int impressora;
    bool scanner;
    bool modem;
    int SATA;

    Estado estado;
    int tempo_espera;

    std::vector<Instrucao> instrucoes_io;
    int pc;

    /**
     * @brief Constrói um processo inicializando todos os atributos.
     */
    Processo(int id, int t0, int prio, int cpu, int mem, int imp, int scan, int mod, int dsk)
        : PID(id),
          prioridade_original(prio),
          prioridade_atual(prio),
          t_chegada(t0),
          t_total(cpu),
          t_restante(cpu),
          quantum_restante(0),
          blocos_mem_req(mem),
          offset_memoria(-1),
          impressora(imp),
          scanner(scan == 1),
          modem(mod == 1),
          SATA(dsk),
          estado(PRONTO),
          tempo_espera(0),
          pc(0)
    {
    }

    /**
     * @brief Reatribui o quantum do processo com base na prioridade atual.
     */
    void resetar_quantum();
};

#endif
