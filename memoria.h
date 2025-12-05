#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include "processo.h"
/**
 * @class GerenciadorMemoria
 * @brief Representa a memória principal do pseudo-so.
 *
 * Além de armazenar a ocupação da memoria pelos processos, esta classe protege a alocação
 * de memoria garantindo que processos de tempo real e usuário nao se misturem.
 * Utiliza o algoritmo First-Fit para alocação de memória.
 * Desaloca a memória quando o processo é finalizado de forma segura.
 */
class GerenciadorMemoria
{
public:
    /** Inicialização da memoria com -1 em seus campos e tamanho total de 1024 blocos. */
    std::vector<int> ram;
    GerenciadorMemoria() : ram(1024, -1) {}

    /** Tenta alocar memória para o processo p.
     * @param p Ponteiro para o processo que requisita memória.
     * @return true se a alocação foi bem sucedida, false caso contrário.
     */
    bool alocar(Processo *p)
    {
        int fim_busca = 1024;
        int inicio_busca = p->prioridade_original == 0 ? 0 : 64;

        int cont_livres = 0;
        int idx_start = -1;

        for (int i = inicio_busca; i < fim_busca; i++)
        {
            if (ram[i] == -1)
            {
                if (cont_livres == 0)
                {
                    idx_start = i;
                }
                cont_livres++;

                if (cont_livres == p->blocos_mem_req)
                {
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
                cont_livres = 0;
                idx_start = -1;
            }
        }
        return false;
    }
    /** Desaloca a memória ocupada pelo processo p.
     * @param p Ponteiro para o processo que está sendo finalizado.
     */
    void desalocar(Processo *p)
    {
        if (p->offset_memoria != -1)
        {
            for (int i = p->offset_memoria; i < p->offset_memoria + p->blocos_mem_req; i++)
            {
                ram[i] = -1;
            }
            p->offset_memoria = -1;
        }
    }
};

#endif