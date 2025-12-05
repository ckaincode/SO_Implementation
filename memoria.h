#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include "processo.h"

class GerenciadorMemoria
{
public:
    // Vetor representando a RAM. -1 = Livre, Outro valor = PID do dono
    std::vector<int> ram;

    GerenciadorMemoria();

    // Algoritmo First-Fit
    // Retorna true se conseguiu alocar, false se não coube.
    bool alocar(Processo *p);

    // Libera a memoria usada pelo processo
    void desalocar(Processo *p);

};

inline GerenciadorMemoria::GerenciadorMemoria(){
// A especificação pede tamanho fixo de 1024 blocos, inicializa tudo com -1 para indicar que está vazia
    for(int i = 0; i < 1024; i++)
    {
        ram.push_back(-1);
    }
}


#endif