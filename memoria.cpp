#include <vector>

#include "processo.h"
#include "memoria.h"

bool GerenciadorMemoria::alocar(Processo *p)
{
    int inicio_busca = 0;
    int fim_busca = 0;

    // Define a area de busca dependendo se é Tempo Real ou Usuário
    if (p->prioridade_original == 0)
    {
        inicio_busca = 0;
        // Processos TR começam a busca em 0, mas podem ir até o fim da memória
        fim_busca = 1024;
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
    return false;
}

void GerenciadorMemoria::desalocar(Processo *p)
{
    int offset = p->offset_memoria;
    int tamanho = p->blocos_mem_req;
    if (offset != -1)
    {
        for (int i = offset; i < offset + tamanho; i++)
        {
            ram[i] = -1;
        }
        p->offset_memoria = -1;
    }
}