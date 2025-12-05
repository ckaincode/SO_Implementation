#include <vector>
#include <string>
#include <iostream>

#include "processo.h"
#include "arquivos.h"


void GerenciadorArquivos::inicializar(int tam, std::vector<Arquivo> iniciais)
{
    total_blocos = tam;
    tabela_arquivos = iniciais;
}

std::string GerenciadorArquivos::operar(Processo *p, Instrucao inst)
{
    std::string op_id = std::to_string(inst.id_global);

    // DELETAR
    if (inst.codigo == 1)
    {
        for (int i = 0; i < tabela_arquivos.size(); i++)
        {
            if (tabela_arquivos[i].nome == inst.arquivo)
            {
                if (p->prioridade_original == 0 || tabela_arquivos[i].criador_pid == p->PID)
                {
                    tabela_arquivos.erase(tabela_arquivos.begin() + i);
                    return "Operacao " + op_id + " => Sucesso\n O processo " + std::to_string(p->PID) + " deletou o arquivo " + inst.arquivo + ".";
                }
                else
                {
                    return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode deletar o arquivo " + inst.arquivo + " (permissao negada).";
                }
            }
        }
        return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode deletar o arquivo " + inst.arquivo + " porque ele nao existe.";
    }

    // CRIAR
    else
    {
        for (int inicio = 0; inicio < total_blocos; inicio++)
        {
            if (inicio + inst.blocos > total_blocos)
                break;

            bool cabe = true;
            for (int k = 0; k < inst.blocos; k++)
            {
                int bloco_atual = inicio + k;
                for (int j = 0; j < tabela_arquivos.size(); j++)
                {
                    int inicio_arq = tabela_arquivos[j].bloco_inicial;
                    int fim_arq = tabela_arquivos[j].bloco_inicial + tabela_arquivos[j].tamanho - 1;

                    if (bloco_atual >= inicio_arq && bloco_atual <= fim_arq)
                    {
                        cabe = false;
                        break;
                    }
                }
                if (!cabe)
                    break;
            }

            if (cabe)
            {
                Arquivo novo;
                novo.nome = inst.arquivo;
                novo.bloco_inicial = inicio;
                novo.tamanho = inst.blocos;
                novo.criador_pid = p->PID;
                tabela_arquivos.push_back(novo);

                return "Operacao " + op_id + " => Sucesso\n O processo " + std::to_string(p->PID) + " criou o arquivo " + inst.arquivo + " (blocos " + std::to_string(inicio) + " a " + std::to_string(inicio + inst.blocos - 1) + ").";
            }
        }
        return "Operacao " + op_id + " => Falha\n O processo " + std::to_string(p->PID) + " nao pode criar o arquivo " + inst.arquivo + " (falta de espaco).";
    }
}

void GerenciadorArquivos::imprimir_mapa()
{
    std::cout << "Mapa de ocupacao do disco:" << std::endl;
    std::vector<std::string> visual;
    for (int i = 0; i < total_blocos; i++)
    {
        visual.push_back(" ");
    }

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
    for (int i = 0; i < visual.size(); i++)
    {
        std::cout << visual[i] << " ";
    }
    std::cout << std::endl;
}