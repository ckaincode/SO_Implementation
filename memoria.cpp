#include <iostream>
#include "struct/bloco.h"
class GerenciadorMemoria
{
private:
    std::vector<Bloco> memoria;
    const int TAM_TOTAL = 1024;
    const int TAM_TEMPO_REAL = 64;
public:
    GerenciadorMemoria() : memoria(TAM_TOTAL, -1) {}

    int alocar(int pid, int blocosRequeridos, bool isTempoReal)
    {
        int inicioBusca = isTempoReal ? 0 : TAM_TEMPO_REAL;
        int fimBusca = isTempoReal ? TAM_TEMPO_REAL : TAM_TOTAL;

        int offset = buscarBlocoDisponivel(inicioBusca, fimBusca, blocosRequeridos);
        return offset; 
    }

    int buscarBlocoDisponivel(int inicio, int fim, int tamanho)
    {
        for (int i = inicio; i <= fim - tamanho; ++i)
        {
            bool espacoDisponivel = true;
            for (int j = 0; j < blocosRequeridos; ++j)
            {
                if (memoria[i + j].pid_dono != -1)
                {
                    espacoDisponivel = false;
                    i += j; 
                    break;
                }
            }
            if (espacoDisponivel)
            {
                for (int j = 0; j < tamanho; ++j)
                {
                    memoria[i + j].pid_dono = pid;
                }
                return i;
            }
        }
        return -1;
    }


}
