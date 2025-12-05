#ifndef RECURSOS_H
#define RECURSOS_H

#include "processo.h"

/**
 * @brief Gerencia os recursos de hardware do sistema.
 *
 * Controla a alocação e liberação de recursos como scanner, impressoras, modem e discos SATA.
 * Garante que os recursos sejam alocados de forma atômica para evitar conflitos entre processos.
 */
class GerenciadorRecursos
{
public:
    bool scanner_ocupado;   // Apenas 1 scanner, optado por booleano
    int impressoras_livres; // 2 total
    bool modem_ocupado;     // Apenas 1 modem, optado por booleano
    int satas_livres;       // 3 total

    // Inicializa os recursos, todos disponiveis
    GerenciadorRecursos();

    // Tenta alocar todos os recursos que o processo precisa de uma só vez
    // Garantimos a atomicidade da operação
    bool alocar(Processo *p);

    // Libera recursos quando o processo sai da CPU
    void liberar(Processo *p);
};

#endif