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
    void adicionar(Processo *p);
    /**
     * @brief Obtém o próximo processo a ser executado.
     *
     * A busca segue a ordem de prioridade:
     * TR → P1 → P2 → P3 → P4 → P5.
     *
     * @return Ponteiro para o próximo processo ou NULL se todas as filas estiverem vazias.
     */
    Processo *proximo();
    /**
     * @brief Remove o processo da frente da fila correspondente à sua prioridade atual.
     * @param p Processo a ser removido.
     */
    void remover_da_frente(Processo *p);

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
    void executar_aging();
};

#endif
