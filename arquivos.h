#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <vector>
#include <string>
#include <iostream>
#include "processo.h"

/**
 * @brief Representa um arquivo armazenado no disco.
 */
struct Arquivo
{
    std::string nome;  ///< Nome do arquivo.
    int bloco_inicial; ///< Primeiro bloco ocupado.
    int tamanho;       ///< Quantidade de blocos.
    int criador_pid;   ///< PID do processo criador.
};

/**
 * @brief Gerencia arquivos no disco, incluindo criação e deleção.
 */
class GerenciadorArquivos
{
public:
    int total_blocos;                     ///< Total de blocos do disco.
    std::vector<Arquivo> tabela_arquivos; ///< Lista de arquivos existentes.

    GerenciadorArquivos() : total_blocos(0) {};

    /**
     * @brief Inicializa o disco com tamanho e arquivos pré-existentes.
     * @param tam Quantidade total de blocos.
     * @param iniciais Arquivos já presentes no disco.
     */
    void inicializar(int tam, std::vector<Arquivo> iniciais);
    /**
     * @brief Executa uma operação de criação ou remoção de arquivo.
     * @param p Processo solicitante.
     * @param inst Instrução contendo código, nome e tamanho.
     * @return Mensagem indicando sucesso ou falha.
     */
    std::string operar(Processo *p, Instrucao inst);
    /**
     * @brief Imprime o mapa de ocupação dos blocos do disco.
     */
    void imprimir_mapa();
};

#endif
