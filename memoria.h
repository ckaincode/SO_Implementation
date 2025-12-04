#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include "processo.h"

class GerenciadorMemoria
{
public:
    // Vetor representando a RAM. -1 = Livre, Outro valor = PID do dono
    std::vector<int> ram;

    GerenciadorMemoria()
    {
        // A especificação pede tamanho fixo de 1024 blocos, inicializa tudo com -1 para indicar que está vazia
        for (int i = 0; i < 1024; i++)
        {
            ram.push_back(-1);
        }
    }

    // Algoritmo First-Fit
    // Retorna true se conseguiu alocar, false se não coube.
    bool alocar(Processo *p)
    {
        int inicio_busca = 0;
        int fim_busca = 0;

        // Define a area de busca dependendo se é Tempo Real ou Usuário
        // Área reservada para Tempo Real, 64 blocos
        if (p->prioridade_original == 0)
        {
            inicio_busca = 0;
            fim_busca = 64;
        }
        // Área reservada para Usuário, 960 blocos
        else
        {
            inicio_busca = 64;
            fim_busca = 1024;
        }

        int cont_livres = 0;
        int idx_start = -1;

        // Varre a memoria procurando espaco contiguo
        for (int i = inicio_busca; i < fim_busca; i++)
        {
            if (ram[i] == -1) // Se achou espaco livre
            {
                // Marca o primeiro bloco livre e então conta a quantidade contígua
                if (cont_livres == 0)
                    idx_start = i;
                cont_livres++;

                // Se achou tamanho suficiente
                if (cont_livres == p->blocos_mem_req)
                {
                    // Preenche com o PID
                    for (int j = idx_start; j < idx_start + cont_livres; j++)
                    {
                        ram[j] = p->PID;
                    }
                    p->offset_memoria = idx_start;
                    return true;
                }
            }
            else
            {
                // Se achou bloco ocupado, zera a contagem
                cont_livres = 0;
                idx_start = -1;
            }
        }
        return false; // Nao achou espaco contiguo suficiente
    }

    // Libera a memoria usada pelo processo
    void desalocar(Processo *p)
    {
        // Só desaloca se estiver alocado
        if (p->offset_memoria != -1)
        {
            for (int i = p->offset_memoria; i < p->offset_memoria + p->blocos_mem_req; i++)
            {
                ram[i] = -1; // Marca como livre
            }
            p->offset_memoria = -1;
        }
    }
};

#endif