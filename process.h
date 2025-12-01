#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>
#include <ostream>
#include <iostream>

enum EstadoProcesso
{
    PRONTO,
    EXECUTANDO,
    BLOQUEADO,
    FINALIZADO
};

class Processo
{
public:
    // Dados de identificação e controle
    int PID;                // Identificador do Processo (começa em 0)
    int prioridade_inicial; // Prioridade lida do arquivo
    int prioridade_atual;   // Prioridade que será modificada

    // Dados de tempo e execução
    int tempo_inicializacao;        // Tempo em que o processo deve ser criado
    int tempo_processador_total;    // Tempo total de CPU requisitado
    int tempo_processador_restante; // Tempo restante de CPU (para controle)
    int quantum_atual;              // Quantum dinâmico determinado pela prioridade

    // Dados de Gerenciamento de Memória
    int blocos_memoria; // Quantidade de blocos alocados na memória
    int offset_memoria; // Endereço de início na memória (offset)

    // Dados de Gerenciamento de Recursos (E/S)
    int impressora_req; // Código da impressora requisitada (0, 1 ou 2, onde 0 é "nenhuma")
    bool scanner_req;   // Requisição do scanner (true/false, lido como 0 ou 1)
    bool modem_req;     // Requisição do modem (true/false)
    int disco_req;      // Código do disco/SATA requisitado (0, 1, 2 ou 3)

    // Dados de controle do SO
    EstadoProcesso estado; // Estado atual do processo
    int tempo_espera;      // Contador para a técnica de Aging

    // Construtor
    Processo(int pid, int t_init, int prio, int t_cpu, int blocos, int imp, int scan, int mod, int disco)
        : PID(pid),
          prioridade_inicial(prio),
          prioridade_atual(prio),
          tempo_inicializacao(t_init),
          tempo_processador_total(t_cpu),
          tempo_processador_restante(t_cpu),
          blocos_memoria(blocos),
          impressora_req(imp),
          scanner_req(scan == 1),
          modem_req(mod == 1),
          disco_req(disco),
          offset_memoria(-1),
          estado(PRONTO),
          tempo_espera(0)
    {
        // Falta a lógica de quantum
    }

    void exibir_dados_lidos() const
    {
        std::cout << "--- Processo PID " << PID << " ---" << std::endl;
        std::cout << "  Init Time: " << tempo_inicializacao << std::endl;
        std::cout << "  Priority: " << prioridade_inicial << std::endl;
        std::cout << "  CPU Time: " << tempo_processador_total << std::endl;
        std::cout << "  Blocks: " << blocos_memoria << std::endl;
        std::cout << "  I/O Req: "
                  << "Imp=" << impressora_req
                  << " Scan=" << (scanner_req ? "Sim" : "Não")
                  << " Modem=" << (modem_req ? "Sim" : "Não")
                  << " Disk=" << disco_req << std::endl;
        std::cout << "----------------------" << std::endl;
    }
};

#endif