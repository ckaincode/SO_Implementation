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
    GerenciadorRecursos()
    {
        scanner_ocupado = false;
        impressoras_livres = 2;
        modem_ocupado = false;
        satas_livres = 3;
    }

    // Tenta alocar todos os recursos que o processo precisa de uma só vez
    // Garantimos a atomicidade da operação
    bool alocar(Processo *p)
    {
        // Nessa etapa analisamos os recursos livres
        // Caso true, passa para a proxima etapa e aloca o dispositivo
        // Caso false, aborta a missão
        if (p->scanner == true && scanner_ocupado == true)
            return false;
        if (p->impressora > 0 && impressoras_livres <= 0)
            return false;
        if (p->modem == true && modem_ocupado == true)
            return false;
        if (p->SATA > 0 && satas_livres <= 0)
            return false;

        // Se passou nas verificacoes, aloca (marca como ocupado)
        if (p->scanner == true)
            scanner_ocupado = true;

        if (p->impressora > 0)
        {
            impressoras_livres = impressoras_livres - 1;
        }

        if (p->modem == true)
            modem_ocupado = true;

        if (p->SATA > 0)
        {
            satas_livres = satas_livres - 1;
        }

        return true;
    }

    // Libera recursos quando o processo sai da CPU
    void liberar(Processo *p)
    {
        if (p->scanner == true)
            scanner_ocupado = false;

        if (p->impressora > 0)
        {
            impressoras_livres = impressoras_livres + 1;
        }

        if (p->modem == true)
            modem_ocupado = false;

        if (p->SATA > 0)
        {
            satas_livres = satas_livres + 1;
        }
    }
};

#endif