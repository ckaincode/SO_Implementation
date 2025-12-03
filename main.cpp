#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Seus headers (devem estar na mesma pasta)
#include "processo.h"
#include "recursos.h"

/*
=========================================================
Main para testes
=========================================================
*/

// --- 1. FUNÇÃO PARA LER OS PROCESSOS ---
std::vector<Processo *> ler_processos(std::string caminho_arquivo)
{
    std::vector<Processo *> lista;
    std::ifstream file(caminho_arquivo);
    std::string linha;
    int pid_counter = 0;

    if (!file.is_open())
    {
        std::cerr << "Erro: Nao foi possivel abrir " << caminho_arquivo << std::endl;
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
            try
            {
                dados.push_back(std::stoi(segmento));
            }
            catch (...)
            {
            }
        }

        if (dados.size() >= 8)
        {
            // Cria o processo usando o construtor do seu processo.h
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

// --- 2. FUNÇÃO PARA LER AS INSTRUÇÕES DE IO DO ARQUIVO FILES ---
// Esta funcao apenas preenche o vetor instrucoes_io dentro de cada processo
void vincular_io(std::string caminho_arquivo, std::vector<Processo *> &processos)
{
    std::ifstream file(caminho_arquivo);
    std::string linha;

    if (!file.is_open())
        return;

    // Pula as primeiras linhas que nao sao instrucoes de processo
    // O formato padrao e: TotalBlocos (linha 1), QtdArquivosOcupados (linha 2)
    std::getline(file, linha);
    std::getline(file, linha);
    int ocupados = 0;
    try
    {
        ocupados = std::stoi(linha);
    }
    catch (...)
    {
    }

    // Pula os arquivos pre-alocados
    for (int i = 0; i < ocupados; i++)
    {
        std::getline(file, linha);
    }

    // Agora le as instrucoes: PID, Op, Nome, Tam
    while (std::getline(file, linha))
    {
        if (linha.empty())
            continue;

        std::stringstream ss(linha);
        std::string s_pid, s_op, nome, s_tam;

        // Tenta ler PID
        if (!std::getline(ss, s_pid, ','))
            continue;

        int pid = -1;
        try
        {
            pid = std::stoi(s_pid);
        }
        catch (...)
        {
            continue;
        }

        // Se conseguiu ler PID, le o resto
        std::getline(ss, s_op, ',');
        std::getline(ss, nome, ',');
        int tam = 0;
        if (std::getline(ss, s_tam, ','))
        {
            try
            {
                tam = std::stoi(s_tam);
            }
            catch (...)
            {
            }
        }

        // Limpa espacos do nome
        std::string nome_limpo = "";
        for (char c : nome)
            if (c != ' ')
                nome_limpo += c;

        // Procura o processo e adiciona a instrucao
        for (auto p : processos)
        {
            if (p->PID == pid)
            {
                Instrucao inst;
                inst.pid = pid;
                try
                {
                    inst.codigo = std::stoi(s_op);
                }
                catch (...)
                {
                    inst.codigo = 0;
                }
                inst.arquivo = nome_limpo;
                inst.blocos = tam;
                p->instrucoes_io.push_back(inst);
                break;
            }
        }
    }
}

// --- 3. MAIN DE VALIDAÇÃO ---
int main(int argc, char **argv)
{

    // Caminhos padrao (altere se necessario)
    std::string f_proc = "Testes/processes.txt";
    std::string f_file = "Testes/files.txt";

    if (argc > 1)
        f_proc = argv[1];
    if (argc > 2)
        f_file = argv[2];

    // Carrega dados
    std::vector<Processo *> processos = ler_processos(f_proc);
    vincular_io(f_file, processos);

    // Instancia gerenciador de recursos (recursos.h)
    GerenciadorRecursos rec;

    std::cout << ">>> VALIDACAO DE PROCESSO.H E RECURSOS.H <<<\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "PID | Prio | Quantum (Calc) | Rec Solicitados (I/S/M/D) | Alocacao? | IOs Lidas\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    for (Processo *p : processos)
    {

        // 1. Valida resetar_quantum() do processo.h
        p->resetar_quantum();

        // 2. Tenta alocar recursos usando recursos.h
        bool conseguiu_alocar = rec.alocar(p);

        // Printa resultado
        std::cout << p->PID << "   | "
                  << p->prioridade_original << "    | "
                  << p->quantum_restante << "\t\t| "
                  << p->impressora << "/" << p->scanner << "/" << p->modem << "/" << p->disco << "\t\t    | ";

        if (conseguiu_alocar)
        {
            std::cout << "SUCESSO   | ";
            // Importante: Liberar para testar o proximo isoladamente
            // Se quiser testar esgotamento de recursos, remova esta linha
            rec.liberar(p);
        }
        else
        {
            std::cout << "FALHA     | ";
        }

        std::cout << p->instrucoes_io.size() << std::endl;
    }

    std::cout << "--------------------------------------------------------------------------------\n";

    // Cleanup
    for (auto p : processos)
        delete p;

    return 0;
}