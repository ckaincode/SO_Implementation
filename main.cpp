#include "process.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<Processo> ler_processos(const std::string &nome_arquivo)
{
    std::vector<Processo> processos;
    std::ifstream arquivo(nome_arquivo);
    std::string linha;
    int pid_contador = 0;

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de processos: " << nome_arquivo << std::endl;
        return processos;
    }

    while (std::getline(arquivo, linha))
    {
        std::stringstream ss(linha);
        std::string campo;
        std::vector<int> dados;

        while (std::getline(ss, campo, ','))
        {
            try
            {
                size_t start = campo.find_first_not_of(' ');
                size_t end = campo.find_last_not_of(' ');
                std::string trimmed_campo = (start == std::string::npos) ? "" : campo.substr(start, end - start + 1);

                if (!trimmed_campo.empty())
                {
                    dados.push_back(std::stoi(trimmed_campo));
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro de conversão de dados na linha: " << linha << std::endl;
            }
        }

        // Deve haver 8 campos lidos [cite: 109]
        if (dados.size() == 8)
        {
            Processo novo_processo(
                pid_contador++,
                dados[0], // tempo_inicializacao
                dados[1], // prioridade
                dados[2], // tempo_processador
                dados[3], // blocos_memoria
                dados[4], // impressora_req
                dados[5], // scanner_req
                dados[6], // modem_req
                dados[7]  // disco_req
            );
            processos.push_back(novo_processo);
        }
        else
        {
            std::cerr << "Linha de processo mal formatada (esperado 8 campos): " << linha << std::endl;
        }
    }

    return processos;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <caminho_para_processes.txt> [caminho_para_files.txt]" << std::endl;
        return 1;
    }
    std::string processes_file = argv[1];
    std::cout << "Iniciando leitura do arquivo de processos: " << processes_file << std::endl;
    std::vector<Processo> processos_lidos = ler_processos(processes_file);
    std::cout << "\nLeitura concluída. Total de processos lidos: " << processos_lidos.size() << std::endl;
    for (const auto &p : processos_lidos)
    {
        p.exibir_dados_lidos();
    }
    return 0;
}