#include <vector>
#include <string>

#include "processo.h"


Processo::Processo(int id, int t0, int prio, int cpu, int mem, int imp, int scan, int mod, int dsk)
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

void Processo::resetar_quantum()
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
