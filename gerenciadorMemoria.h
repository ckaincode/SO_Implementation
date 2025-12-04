
#ifndef GERENCIADOR_MEMORIA_H
#define GERENCIADOR_MEMORIA_H

#include <iostream>
#include <vector>
#include "bloco.cpp"
class GerenciadorMemoria
{
private:
    std::vector<Bloco> memoria;
    const int TAM_TOTAL = 1024;
    const int TAM_TEMPO_REAL = 64;

public:
    GerenciadorMemoria() : memoria(TAM_TOTAL) {}

    int alocar(int pid, int blocosRequeridos, int prioridade)
    {
        bool isTempoReal = (prioridade == 0);
        int inicioBusca = isTempoReal ? 0 : TAM_TEMPO_REAL;
        int fimBusca = isTempoReal ? TAM_TEMPO_REAL : TAM_TOTAL;

        int offset = buscarGravarBlocoDisponivel(inicioBusca, fimBusca, pid, blocosRequeridos);
        return offset;
    }

    int buscarGravarBlocoDisponivel(int inicio, int fim, int pid, int tamanho)
    {
        for (int i = inicio; i <= fim - tamanho; ++i)
        {
            bool espacoDisponivel = true;
            for (int j = 0; j < tamanho; ++j)
            {
                if (memoria[i + j].obter_pid_dono() != -1)
                {
                    espacoDisponivel = false;
                    i += j;
                    break;
                }
            }
            if (espacoDisponivel)
            {
                for (int j = 0; j < tamanho; ++j)
                {
                    memoria[i + j].definir_pid_dono(pid);
                }
                return i;
            }
        }
        return -1;
    }

    void desalocar(int pid, int offset, int tamanho)
    {
        if (offset < 0 || offset + tamanho > TAM_TOTAL)
        {
            return;
        }

        for (int i = offset; i < offset + tamanho; ++i)
        {
            if (memoria[i].obter_pid_dono() == pid)
            {
                memoria[i].definir_pid_dono(-1);
            }
        }
    }
};

#endif