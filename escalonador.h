#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <deque>
#include <vector>
#include <iostream>
#include "processo.h"

class Escalonador
{
public:
    // ----- Filas -----
    // Especificação de trabalho exige 6 filas( tr - Tempo Real, p1-p5 - Usuário (Prioridade) )
    //
    //
    std::deque<Processo *> fila_tr; // Prioridade 0
    std::deque<Processo *> fila_p1;
    std::deque<Processo *> fila_p2;
    std::deque<Processo *> fila_p3;
    std::deque<Processo *> fila_p4;
    std::deque<Processo *> fila_p5;

    // Funcao para adicionar processo na fila correta
    // Chamada quando o processo na chegada ou na preempção
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

    // Retorna o próximo da fila (ordem de prioridade)
    // Verifica a fila_tr primeiro. Se vazia olhamos a p1, e assim por diante.
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

    // Remove o processo da fila
    // Usado depois que o Dispatcher
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

    // Lógica do Aging (Envelhecimento)
    // Varre da menor prioridade (5) para cima..
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
                // Se nao envelheceu o suficiente, volta pro final da fila 5
                fila_p5.push_back(p);
            }
        }

        /*
        Desse ponto até o final da função 'executar_aging()' o processo é o mesmo que o suado na fila 5
        Esta na ordem 4,3,2 de verificação
        NÃO HÁ PROMOÇÃO PARA A FILA TR
        */

        // Verifica Fila 4
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