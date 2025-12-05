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
    void adicionar(Processo *p);
    // Retorna o próximo da fila (ordem de prioridade)
    // Verifica a fila_tr primeiro. Se vazia olhamos a p1, e assim por diante.
    Processo *proximo();
    // Remove o processo da fila
    // Usado depois que o Dispatcher
    void remover_da_frente(Processo *p);

    // Lógica do Aging (Envelhecimento)
    // Varre da menor prioridade (5) para cima..
    void executar_aging();

};

#endif