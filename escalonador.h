#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <deque>
#include <vector>
#include <iostream>
#include "processo.h"

/**
 * @brief Classe responsável pelo escalonamento de processos utilizando múltiplas filas de prioridade.
 *
 * O escalonador mantém 6 filas:
 *  - TR  (prioridade 0) → Tempo Real
 *  - P1  (prioridade 1) → Usuário
 *  - P2  (prioridade 2)
 *  - P3  (prioridade 3)
 *  - P4  (prioridade 4)
 *  - P5  (prioridade 5)
 *
 * Também implementa um mecanismo simples de *aging* para evitar starvation:
 * sempre que um processo passa 10 ciclos esperando, sua prioridade sobe uma fila.
 */
class Escalonador
{
public:
    // ----- Filas de escalonamento -----
    std::deque<Processo *> fila_tr;
    std::deque<Processo *> fila_p1;
    std::deque<Processo *> fila_p2;
    std::deque<Processo *> fila_p3;
    std::deque<Processo *> fila_p4;
    std::deque<Processo *> fila_p5;

    /**
     * @brief Adiciona um processo na fila correspondente à sua prioridade atual.
     * @param p Ponteiro para o processo a ser inserido.
     */
    void adicionar(Processo *p)
    {
        switch (p->prioridade_atual)
        {
        case 0:
            fila_tr.push_back(p);
            break;
        case 1:
            fila_p1.push_back(p);
            break;
        case 2:
            fila_p2.push_back(p);
            break;
        case 3:
            fila_p3.push_back(p);
            break;
        case 4:
            fila_p4.push_back(p);
            break;
        default:
            fila_p5.push_back(p);
            break;
        }
    }

    /**
     * @brief Obtém o próximo processo a ser executado.
     *
     * A busca segue a ordem de prioridade:
     * TR → P1 → P2 → P3 → P4 → P5.
     *
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
     * @param p Processo a ser removido.
     */
    void remover_da_frente(Processo *p)
    {
        switch (p->prioridade_atual)
        {
        case 0:
            if (!fila_tr.empty())
                fila_tr.pop_front();
            break;
        case 1:
            if (!fila_p1.empty())
                fila_p1.pop_front();
            break;
        case 2:
            if (!fila_p2.empty())
                fila_p2.pop_front();
            break;
        case 3:
            if (!fila_p3.empty())
                fila_p3.pop_front();
            break;
        case 4:
            if (!fila_p4.empty())
                fila_p4.pop_front();
            break;
        case 5:
            if (!fila_p5.empty())
                fila_p5.pop_front();
            break;
        }
    }

    /**
     * @brief Executa o mecanismo de aging.
     *
     * Todo processo que permanecer 10 ciclos esperando tem sua prioridade aumentada:
     *  - P5 → P4
     *  - P4 → P3
     *  - P3 → P2
     *  - P2 → P1
     *
     * Obs.: Tempo real (TR / prioridade 0) não sofre aging.
     */
    void executar_aging()
    {
        // ----- Aging da fila P5 -----
        aging_fila(fila_p5, fila_p4, 5, 4);

        // ----- Aging da fila P4 -----
        aging_fila(fila_p4, fila_p3, 4, 3);

        // ----- Aging da fila P3 -----
        aging_fila(fila_p3, fila_p2, 3, 2);

        // ----- Aging da fila P2 -----
        aging_fila(fila_p2, fila_p1, 2, 1);
    }

private:
    /**
     * @brief Função auxiliar para aplicar aging em uma fila genérica.
     *
     * @param fila_atual  Fila onde os processos estão esperando.
     * @param fila_superior Fila para onde o processo sobe em caso de aging.
     * @param prioridade_atual  Prioridade da fila atual.
     * @param prioridade_subida Prioridade ao subir.
     */
    void aging_fila(std::deque<Processo *> &fila_atual,
                    std::deque<Processo *> &fila_superior,
                    int prioridade_atual,
                    int prioridade_subida)
    {
        int tamanho = fila_atual.size();

        for (int i = 0; i < tamanho; i++)
        {
            Processo *p = fila_atual.front();
            fila_atual.pop_front();

            p->tempo_espera++;

            if (p->tempo_espera >= 10)
            {
                p->prioridade_atual = prioridade_subida;
                p->tempo_espera = 0;

                fila_superior.push_back(p);
                std::cout << ">>> [AGING] Processo " << p->PID
                          << " subiu para P" << prioridade_subida << std::endl;
            }
            else
            {
                fila_atual.push_back(p);
            }
        }
    }
};

#endif
