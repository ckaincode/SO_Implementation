
#ifndef GERENCIADOR_MEMORIA_H
#define GERENCIADOR_MEMORIA_H

#include <iostream>
#include <vector>
#include "bloco.h"
#include "processo.h"

class GerenciadorMemoria
{
private:
    std::vector<Bloco> memoria;
    const int TAM_TOTAL = 1024;
    const int TAM_TEMPO_REAL = 64;

public:
    GerenciadorMemoria() : memoria(TAM_TOTAL) {}

    int alocar(Processo *p)
    {
        bool isTempoReal = (p->prioridade_original == 0);
        int inicioBusca = isTempoReal ? 0 : TAM_TEMPO_REAL;
        int fimBusca = isTempoReal ? TAM_TEMPO_REAL : TAM_TOTAL;

        int endereco = buscarGravarBlocoDisponivel(inicioBusca, fimBusca, p->PID, p->blocos_mem_req);

        if (endereco == -1)
            return false;

        p->offset_memoria = endereco;
        return true;
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

    void desalocar(Processo *p)
    {
        int pid = p->PID;
        int offset = p->offset_memoria;
        int tamanho = p->blocos_mem_req;
        if (offset == -1)
            return;

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