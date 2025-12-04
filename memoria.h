
#ifndef GERENCIADOR_MEMORIA_H
#define GERENCIADOR_MEMORIA_H

#include <iostream>
#include <vector>
#include "processo.h"

class GerenciadorMemoria
{
public:
    std::vector<int> ram;
    const int TAM_TOTAL = 1024;
    const int TAM_TEMPO_REAL = 64;
    GerenciadorMemoria() : ram(TAM_TOTAL, -1) {}

    int alocar(Processo *p)
    {
        bool isTempoReal = (p->prioridade_original == 0);
        int inicioBusca = isTempoReal ? 0 : TAM_TEMPO_REAL;

        int endereco = buscarGravarBlocoDisponivel(inicioBusca, TAM_TOTAL, p->PID, p->blocos_mem_req);

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
                if (ram[i + j] != -1)
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
                    ram[i + j] = pid;
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
            if (ram[i] == pid)
            {
                ram[i] = -1;
            }
        }
    }
};

#endif