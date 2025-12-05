#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <deque>
#include <vector>
#include <iostream>
#include "processo.h"

/**
 * @brief Implementa o escalonador de processos com múltiplas filas de prioridade.
 * Gerencia seis filas distintas para processos de tempo real e de usuário,
 *
 */
class Escalonador
{
public:
    // ----- Filas -----
    // Especificação de trabalho exige 6 filas( tr - Tempo Real, p1-p5 - Usuário (Prioridade) )
    //
    //
    std::deque<Processo *> fila_tr;
    std::deque<Processo *> fila_p1;
    std::deque<Processo *> fila_p2;
    std::deque<Processo *> fila_p3;
    std::deque<Processo *> fila_p4;
    std::deque<Processo *> fila_p5;

    /**
     * @brief Adiciona um processo à fila correspondente com base em sua prioridade atual.
     * @param p Ponteiro para o processo a ser adicionado.
     */
    void adicionar(Processo *p)
    {
        if (p->prioridade_atual == 0)
        {
            fila_tr.push_back(p);
        }
        else if (p->prioridade_atual == 1)
        {
            fila_p1.push_back(p);
        }
        else if (p->prioridade_atual == 2)
        {
            fila_p2.push_back(p);
        }
        else if (p->prioridade_atual == 3)
        {
            fila_p3.push_back(p);
        }
        else if (p->prioridade_atual == 4)
        {
            fila_p4.push_back(p);
        }
        else
        {
            fila_p5.push_back(p);
        }
    }

    /**
     * @brief Retorna o próximo processo a ser executado com base na prioridade das filas.
     * @return Ponteiro para o próximo processo ou NULL se todas as filas estiverem vazias.
     */
    Processo *proximo()
    {
        if (!fila_tr.empty())
            return fila_tr.front();
        if (!fila_p1.empty())
            return fila_p1.front();
        if (!fila_p2.empty())
            return fila_p2.front();
        if (!fila_p3.empty())
            return fila_p3.front();
        if (!fila_p4.empty())
            return fila_p4.front();
        if (!fila_p5.empty())
            return fila_p5.front();
        return NULL;
    }

    /**
     * @brief Remove o processo da frente da fila correspondente à sua prioridade atual.
     * @param p Ponteiro para o processo a ser removido.
     */
    void remover_da_frente(Processo *p)
    {
        int prioridade = p->prioridade_atual;
        if (prioridade == 0 && !fila_tr.empty())
            fila_tr.pop_front();
        else if (prioridade == 1 && !fila_p1.empty())
            fila_p1.pop_front();
        else if (prioridade == 2 && !fila_p2.empty())
            fila_p2.pop_front();
        else if (prioridade == 3 && !fila_p3.empty())
            fila_p3.pop_front();
        else if (prioridade == 4 && !fila_p4.empty())
            fila_p4.pop_front();
        else if (prioridade == 5 && !fila_p5.empty())
            fila_p5.pop_front();
    }

    /**
     * @brief Executa o mecanismo de aging para evitar starvation.
     * Processos que aguardam há 10 ciclos em suas filas têm sua prioridade aumentada
     */
    void executar_aging()
    {

        // Verifica Fila 5
        int tamanho = fila_p5.size();
        for (int i = 0; i < tamanho; i++)
        {
            Processo *p = fila_p5.front(); // Pega o primeiro
            fila_p5.pop_front();           // Tira da fila

            p->tempo_espera++; // Envelhece o processo (mais um ciclo sem CPU)

            // Se esperou 10 ciclos, sobre a prioridade
            if (p->tempo_espera >= 10)
            {
                p->prioridade_atual--; // Sobe para p4
                p->tempo_espera = 0;   //
                // Promove para a fila de cima (No caso p4)
                fila_p4.push_back(p);
                std::cout << ">>> [AGING] Processo " << p->PID << " subiu para P4" << std::endl;
            }
            else
            {
                fila_p5.push_back(p);
            }
        }

        tamanho = fila_p4.size();
        for (int i = 0; i < tamanho; i++)
        {
            Processo *p = fila_p4.front();
            fila_p4.pop_front();

            p->tempo_espera++;
            if (p->tempo_espera >= 10)
            {
                p->prioridade_atual--; // Sobe para 3
                p->tempo_espera = 0;
                fila_p3.push_back(p);
                std::cout << ">>> [AGING] Processo " << p->PID << " subiu para P3" << std::endl;
            }
            else
            {
                fila_p4.push_back(p);
            }
        }

        // Verifica Fila 3
        tamanho = fila_p3.size();
        for (int i = 0; i < tamanho; i++)
        {
            Processo *p = fila_p3.front();
            fila_p3.pop_front();

            p->tempo_espera++;
            if (p->tempo_espera >= 10)
            {
                p->prioridade_atual--; // Sobe para 2
                p->tempo_espera = 0;
                fila_p2.push_back(p);
                std::cout << ">>> [AGING] Processo " << p->PID << " subiu para P2" << std::endl;
            }
            else
            {
                fila_p3.push_back(p);
            }
        }

        // Verifica Fila 2
        tamanho = fila_p2.size();
        for (int i = 0; i < tamanho; i++)
        {
            Processo *p = fila_p2.front();
            fila_p2.pop_front();

            p->tempo_espera++;
            if (p->tempo_espera >= 10)
            {
                p->prioridade_atual--; // Sobe para 1
                p->tempo_espera = 0;
                fila_p1.push_back(p);
                std::cout << ">>> [AGING] Processo " << p->PID << " subiu para P1" << std::endl;
            }
            else
            {
                fila_p2.push_back(p);
            }
        }
    }
};

#endif