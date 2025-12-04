#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <vector>
#include <string>
#include <iostream>
#include "processo.h"

// Representa um arquivo no disco
struct Arquivo
{
    std::string nome;
    int bloco_inicial;
    int tamanho;     // Quantos blocos ocupa
    int criador_pid; // Id do criador do arquivo
};

class GerenciadorArquivos
{
public:
    int total_blocos;                     // Tamanho total do disco
    std::vector<Arquivo> tabela_arquivos; // Lista de arquivos no disco

    GerenciadorArquivos()
    {
        total_blocos = 0;
    }

    // Carrega o estado inicial do disco (files.txt)
    void inicializar(int tam, std::vector<Arquivo> iniciais)
    {
        total_blocos = tam;
        tabela_arquivos = iniciais;
    }

    // Executa as operações de E/S
    // Retorna uma string de ser impressa na tela pelo Dispatcher
    std::string operar(Processo *p, Instrucao inst)
    {
        // Código 1 -> deletar
        if (inst.codigo == 1)
        {
            // Procura o arquivo na lista pelo nome
            for (int i = 0; i < tabela_arquivos.size(); i++)
            {
                if (tabela_arquivos[i].nome == inst.arquivo)
                {

                    // Se for Tempo Real (0) OU se for o dono do arquivo
                    if (p->prioridade_original == 0 || tabela_arquivos[i].criador_pid == p->PID)
                    {

                        // Remove o arquivo da tabela
                        tabela_arquivos.erase(tabela_arquivos.begin() + i);

                        return "Operacao 1 => Sucesso\n O processo " + std::to_string(p->PID) + " deletou o arquivo " + inst.arquivo;
                    }
                    else
                    {
                        return "Operacao 1 => Falha\n O processo " + std::to_string(p->PID) + " nao pode deletar o arquivo " + inst.arquivo + " (permissao negada).";
                    }
                }
            }
            // Arquivo não encontrado
            return "Operacao 1 => Falha\n O arquivo " + inst.arquivo + " nao existe.";
        }

        // Código 0 - criar
        else
        {
            // Algoritmo First-Fit no Disco
            // Busca o primeiro buraco onde o arquivo caiba inteiro

            // Tenta começar o arquivo em cada posição do disco
            for (int inicio = 0; inicio < total_blocos; inicio++)
            {

                // Verifica se nao ultrapassa o fim do disco
                if (inicio + inst.blocos > total_blocos)
                    break;

                bool cabe = true;

                // Verifica bloco por bloco se esta ocupado por qualquer arquivo
                for (int k = 0; k < inst.blocos; k++)
                {
                    int bloco_atual = inicio + k;

                    // Checamos se ele conflita com qualquer arquivo existente
                    for (int j = 0; j < tabela_arquivos.size(); j++)
                    {
                        int inicio_arq = tabela_arquivos[j].bloco_inicial;
                        int fim_arq = tabela_arquivos[j].bloco_inicial + tabela_arquivos[j].tamanho - 1;

                        // Se o bloco atual cai dentro do intervalo de um arquivo existente
                        if (bloco_atual >= inicio_arq && bloco_atual <= fim_arq)
                        {
                            cabe = false; // Esta ocupado
                            break;
                        }
                    }
                    if (!cabe)
                        break;
                }

                // Se couber, cria o arquivo
                if (cabe)
                {
                    Arquivo novo;
                    novo.nome = inst.arquivo;
                    novo.bloco_inicial = inicio;
                    novo.tamanho = inst.blocos;
                    novo.criador_pid = p->PID;

                    tabela_arquivos.push_back(novo);

                    return "Operacao 0 => Sucesso\n O processo " + std::to_string(p->PID) + " criou o arquivo " + inst.arquivo + " (blocos " + std::to_string(inicio) + " a " + std::to_string(inicio + inst.blocos - 1) + ").";
                }
            }
            // Se rodou o disco todo e não achou buraco
            return "Operacao 0 => Falha\n O processo " + std::to_string(p->PID) + " nao pode criar o arquivo " + inst.arquivo + " (falta de espaco).";
        }
    }

    void imprimir_mapa()
    {
        std::cout << "Mapa de ocupacao do disco:" << std::endl;

        // Cria um vetor visual para imprimir
        std::vector<std::string> visual;
        for (int i = 0; i < total_blocos; i++)
            visual.push_back("0");

        // Preenche com os nomes dos arquivos
        for (int i = 0; i < tabela_arquivos.size(); i++)
        {
            for (int k = 0; k < tabela_arquivos[i].tamanho; k++)
            {
                int pos = tabela_arquivos[i].bloco_inicial + k;
                if (pos < total_blocos)
                {
                    visual[pos] = tabela_arquivos[i].nome;
                }
            }
        }
        // Imprime o resultado final
        for (int i = 0; i < visual.size(); i++)
        {
            std::cout << visual[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif