#include <vector>
#include <string>

#include "processo.h"

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
