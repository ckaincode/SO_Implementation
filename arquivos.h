#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <vector>
#include <string>
#include <iostream>
#include "processo.h"

struct Arquivo
{
    std::string nome;
    int bloco_inicial;
    int tamanho;
    int criador_pid;
};

class GerenciadorArquivos
{
public:
    int total_blocos;
    std::vector<Arquivo> tabela_arquivos;

    GerenciadorArquivos();

    void inicializar(int tam, std::vector<Arquivo> iniciais);

    std::string operar(Processo *p, Instrucao inst);
    
    void imprimir_mapa();

};

inline GerenciadorArquivos::GerenciadorArquivos(){
    total_blocos = 0;
}

#endif