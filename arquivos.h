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

    GerenciadorArquivos() : total_blocos(0) {}

    /**
     * @brief Inicializa o disco com tamanho e arquivos pré-existentes.
     * @param tam Quantidade total de blocos.
     * @param iniciais Arquivos já presentes no disco.
     */
    void inicializar(int tam, std::vector<Arquivo> iniciais)
    {
        total_blocos = tam;
        tabela_arquivos = iniciais;
    }

    /**
     * @brief Executa uma operação de criação ou remoção de arquivo.
     * @param p Processo solicitante.
     * @param inst Instrução contendo código, nome e tamanho.
     * @return Mensagem indicando sucesso ou falha.
     */
    std::string operar(Processo *p, Instrucao inst)
    {
        std::string op_id = std::to_string(inst.id_global);

        // ---- REMOVER ARQUIVO ----
        if (inst.codigo == 1)
        {
            for (int i = 0; i < tabela_arquivos.size(); i++)
            {
                if (tabela_arquivos[i].nome == inst.arquivo)
                {
                    bool pode = (p->prioridade_original == 0 ||
                                 tabela_arquivos[i].criador_pid == p->PID);

                    if (pode)
                    {
                        tabela_arquivos.erase(tabela_arquivos.begin() + i);
                        return "Operacao " + op_id + " => Sucesso\n O processo " + std::to_string(p->PID) + " deletou o arquivo " + inst.arquivo + ".";
                    }
                    return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode deletar o arquivo " + inst.arquivo + " (permissao negada).";
                }
            }

            return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode deletar o arquivo " + inst.arquivo + " porque ele nao existe.";
        }

        // ---- CRIAR ARQUIVO ----
        for (int inicio = 0; inicio < total_blocos; inicio++)
        {
            if (inicio + inst.blocos > total_blocos)
                break;

            bool cabe = true;

            for (int k = 0; k < inst.blocos && cabe; k++)
            {
                int bloco_atual = inicio + k;

                for (const auto &arq : tabela_arquivos)
                {
                    int ini = arq.bloco_inicial;
                    int fim = arq.bloco_inicial + arq.tamanho - 1;

                    if (bloco_atual >= ini && bloco_atual <= fim)
                    {
                        cabe = false;
                        break;
                    }
                }
            }

            if (cabe)
            {
                Arquivo novo{inst.arquivo, inicio, inst.blocos, p->PID};
                tabela_arquivos.push_back(novo);

                return "Operacao " + op_id + " => Sucesso\n O processo " + std::to_string(p->PID) + " criou o arquivo " + inst.arquivo + " (blocos " + std::to_string(inicio) + " a " + std::to_string(inicio + inst.blocos - 1) + ").";
            }
        }

        return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode criar o arquivo " + inst.arquivo + " (falta de espaco).";
    }

    /**
     * @brief Imprime o mapa de ocupação dos blocos do disco.
     */
    void imprimir_mapa()
    {
        std::cout << "Mapa de ocupacao do disco:\n";

        std::vector<std::string> visual(total_blocos, " ");

        for (const auto &arq : tabela_arquivos)
        {
            for (int k = 0; k < arq.tamanho; k++)
            {
                int pos = arq.bloco_inicial + k;
                if (pos < total_blocos)
                    visual[pos] = arq.nome;
            }
        }

        for (const auto &v : visual)
            std::cout << v << " ";

        std::cout << std::endl;
    }
};

#endif
