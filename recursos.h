#ifndef RECURSOS_H
#define RECURSOS_H

#include "processo.h"

class GerenciadorRecursos
{
public:
    bool scanner_ocupado;
    int impressoras_livres; // 2 total
    bool modem_ocupado;
    int satas_livres; // 3 total

    // Inicializa os recursos disponiveis
    GerenciadorRecursos()
    {
        scanner_ocupado = false;
        impressoras_livres = 2;
        modem_ocupado = false;
        satas_livres = 3;
    }

    // Tenta alocar todos os recursos que o processo precisa
    bool alocar(Processo *p)
    {

        if (p->scanner == true && scanner_ocupado == true)
            return false;
        if (p->impressora > 0 && impressoras_livres <= 0)
            return false;
        if (p->modem == true && modem_ocupado == true)
            return false;
        if (p->disco > 0 && satas_livres <= 0)
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

        if (p->disco > 0)
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

        if (p->disco > 0)
        {
            satas_livres = satas_livres + 1;
        }
    }
};

#endif