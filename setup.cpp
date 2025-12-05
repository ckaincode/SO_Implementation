#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "processo.h"
#include "arquivos.h"


// Funções para ler os processos
std::vector<Processo *> ler_processos(std::string arquivo)
{
    std::vector<Processo *> lista;
    std::ifstream file(arquivo);
    std::string linha;
    int pid_counter = 0;

    if (!file.is_open())
    {
        std::cout << "Erro ao abrir arquivo de processos!" << std::endl;
        return lista;
    }

    while (std::getline(file, linha))
    {
        if (linha.empty())
            continue;
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<int> dados;

        while (std::getline(ss, segmento, ','))
        {
            dados.push_back(std::stoi(segmento));
        }

        if (dados.size() >= 8)
        {
            Processo *p = new Processo(
                pid_counter,
                dados[0], dados[1], dados[2], dados[3],
                dados[4], dados[5], dados[6], dados[7]);
            pid_counter++;
            lista.push_back(p);
        }
    }
    return lista;
}

// ALTERADO: Agora recebe o mapa de logs por referência para registrar erros de processos inexistentes
void configurar_arquivos(std::string f_files, std::vector<Processo *> &procs, GerenciadorArquivos &ga, std::map<int, std::string> &logs)
{
    std::ifstream file(f_files);
    std::string linha;

    if (!file.is_open())
        return;

    std::getline(file, linha);
    int total = std::stoi(linha);
    std::getline(file, linha);
    int ocupados = std::stoi(linha);

    std::vector<Arquivo> arquivos_iniciais;
    for (int i = 0; i < ocupados; i++)
    {
        std::getline(file, linha);
        std::stringstream ss(linha);
        std::string nome, s_start, s_len;
        std::getline(ss, nome, ',');
        std::getline(ss, s_start, ',');
        std::getline(ss, s_len, ',');

        size_t first = nome.find_first_not_of(' ');
        if (std::string::npos != first)
            nome = nome.substr(first);

        Arquivo arq;
        arq.nome = nome;
        arq.bloco_inicial = std::stoi(s_start);
        arq.tamanho = std::stoi(s_len);
        arq.criador_pid = -1;
        arquivos_iniciais.push_back(arq);
    }
    ga.inicializar(total, arquivos_iniciais);

    int op_counter = 1;

    while (std::getline(file, linha))
    {
        if (linha.empty())
            continue;
        std::stringstream ss(linha);
        std::string s_pid, s_op, nome, s_tam;
        std::getline(ss, s_pid, ',');
        std::getline(ss, s_op, ',');
        std::getline(ss, nome, ',');
        int tam = 0;
        if (std::getline(ss, s_tam, ','))
            tam = std::stoi(s_tam);

        std::string nome_limpo = "";
        for (int i = 0; i < nome.length(); i++)
            if (nome[i] != ' ')
                nome_limpo += nome[i];

        int pid = std::stoi(s_pid);
        bool processo_encontrado = false;

        for (int i = 0; i < procs.size(); i++)
        {
            if (procs[i]->PID == pid)
            {
                Instrucao inst;
                inst.pid = pid;
                inst.codigo = std::stoi(s_op);
                inst.arquivo = nome_limpo;
                inst.blocos = tam;
                inst.id_global = op_counter;
                procs[i]->instrucoes_io.push_back(inst);
                processo_encontrado = true;
            }
        }

        // Se o processo não existe, registra o erro
        if (!processo_encontrado)
        {
            std::string erro = "Operacao " + std::to_string(op_counter) + " => Falha\n O processo " + std::to_string(pid) + " nao existe.";
            logs[op_counter] = erro; // Guarda no mapa usando o ID como chave (ordenação automática)
        }

        op_counter++;
    }
}

void print_dispatcher(Processo *p)
{
    std::cout << "dispatcher =>" << std::endl;
    std::cout << "\tPID: " << p->PID << std::endl;
    std::cout << "\toffset: " << p->offset_memoria << std::endl;
    std::cout << "\tblocks: " << p->blocos_mem_req << std::endl;
    std::cout << "\tpriority: " << p->prioridade_atual << std::endl;
    std::cout << "\ttime: " << p->t_restante << std::endl;
    std::cout << "\tscanners: " << p->scanner << std::endl;
    std::cout << "\tprinters: " << p->impressora << std::endl;
    std::cout << "\tmodems: " << p->modem << std::endl;
    std::cout << "\tdrives: " << p->SATA << std::endl;
    std::cout << "\n"
              << std::endl;
}