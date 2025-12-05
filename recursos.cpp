#include "recursos.h"

GerenciadorRecursos::GerenciadorRecursos()
{
    scanner_ocupado = false;
    impressoras_livres = 2;
    modem_ocupado = false;
    satas_livres = 3;
}

bool GerenciadorRecursos::alocar(Processo *p)
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

void GerenciadorRecursos::liberar(Processo *p)
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