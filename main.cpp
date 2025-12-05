#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>

#include "processo.h"
#include "memoria.h"
#include "recursos.h"
#include "arquivos.h"
#include "escalonador.h"
#include "setup.h"


int main(int argc, char **argv)
{
    std::string f_proc = "processes.txt";
    std::string f_files = "files.txt";
    if (argc > 1)
        f_proc = argv[1];
    if (argc > 2)
        f_files = argv[2];

    std::vector<Processo *> processos = ler_processos(f_proc);
    GerenciadorArquivos ga;

    // Usando MAP para ordenar as mensagens pelo número da operação (1, 2, 3...)
    std::map<int, std::string> log_sistema_arquivos;

    configurar_arquivos(f_files, processos, ga, log_sistema_arquivos);

    GerenciadorMemoria mem;
    GerenciadorRecursos rec;
    Escalonador escalonador;

    int tempo = 0;
    Processo *cpu = NULL;
    int processos_finalizados = 0;
    int total_procs = processos.size();

    // Validação Inicial
    for (int i = 0; i < processos.size(); i++)
    {
        Processo *p = processos[i];
        bool matar = false;
        if (p->prioridade_original == 0 && p->blocos_mem_req > 64)
            matar = true;
        if (p->prioridade_original > 0 && p->blocos_mem_req > 960)
            matar = true;
        if (matar)
        {
            p->estado = FINALIZADO;
            processos_finalizados++;
        }
    }

    while (processos_finalizados < total_procs)
    {
        // 1. CHEGADA DE PROCESSOS
        for (int i = 0; i < processos.size(); i++)
        {
            Processo *p = processos[i];
            if (p->t_chegada == tempo && p->estado == PRONTO)
            {
                p->resetar_quantum();
                escalonador.adicionar(p);
            }
        }

        // AGING
        escalonador.executar_aging();

        std::vector<std::deque<Processo *> *> filas_ptr;
        filas_ptr.push_back(&escalonador.fila_tr);
        filas_ptr.push_back(&escalonador.fila_p1);
        filas_ptr.push_back(&escalonador.fila_p2);
        filas_ptr.push_back(&escalonador.fila_p3);
        filas_ptr.push_back(&escalonador.fila_p4);
        filas_ptr.push_back(&escalonador.fila_p5);

        for (int f = 0; f < filas_ptr.size(); f++)
        {
            std::deque<Processo *> *fila = filas_ptr[f];
            for (int i = 0; i < fila->size(); i++)
            {
                Processo *p = (*fila)[i];
                if (p->offset_memoria == -1)
                {
                    mem.alocar(p);
                }
            }
        }

        // DISPATCHER
        if (cpu == NULL)
        {
            bool escolheu_alguem = false;

            for (int f = 0; f < filas_ptr.size(); f++)
            {
                std::deque<Processo *> *fila_atual = filas_ptr[f];
                for (int i = 0; i < fila_atual->size(); i++)
                {
                    Processo *candidato = (*fila_atual)[i];
                    bool tem_memoria = (candidato->offset_memoria != -1);
                    bool tem_recursos = true;

                    if (!tem_memoria)
                    {
                        tem_recursos = rec.alocar(candidato);
                        if (tem_recursos)
                        {
                            tem_memoria = mem.alocar(candidato);
                            if (!tem_memoria)
                                rec.liberar(candidato);
                        }
                    }
                    else
                    {
                        tem_recursos = rec.alocar(candidato);
                    }

                    if (tem_memoria && tem_recursos)
                    {
                        cpu = candidato;
                        fila_atual->erase(fila_atual->begin() + i);
                        cpu->estado = EXECUTANDO;
                        print_dispatcher(cpu);
                        std::cout << "process " << cpu->PID << " =>" << std::endl;
                        std::cout << "P" << cpu->PID << " STARTED" << std::endl;
                        escolheu_alguem = true;
                        break;
                    }
                }
                if (escolheu_alguem)
                    break;
            }
        }

        // EXECUÇÃO DA CPU
        if (cpu != NULL)
        {
            std::cout << "P" << cpu->PID << " instruction " << (cpu->pc + 1) << std::endl;
            cpu->pc++;

            cpu->t_restante--;
            cpu->quantum_restante--;

            if (cpu->t_restante <= 0)
            {
                std::cout << "P" << cpu->PID << " return SIGINT \n"
                          << std::endl;

                for (int i = 0; i < cpu->instrucoes_io.size(); i++)
                {
                    std::string log = ga.operar(cpu, cpu->instrucoes_io[i]);
                    // Armazena no mapa usando o ID Global da instrução como chave
                    log_sistema_arquivos[cpu->instrucoes_io[i].id_global] = log;
                }

                // Não desaloca se for Tempo Real
                if (cpu->prioridade_original > 0)
                {
                    mem.desalocar(cpu);
                }

                rec.liberar(cpu);
                cpu->estado = FINALIZADO;
                cpu = NULL;
                processos_finalizados++;
            }
            else if (cpu->quantum_restante <= 0 && cpu->prioridade_original > 0)
            {
                if (cpu->prioridade_atual < 3)
                    cpu->prioridade_atual++;
                cpu->resetar_quantum();
                cpu->estado = PRONTO;
                escalonador.adicionar(cpu);
                cpu = NULL;
            }
        }
        tempo++;
    }

    // Impressão final ordenada pelo ID da operação graças ao map
    std::cout << "Sistema de arquivos =>" << std::endl;
    for (std::map<int, std::string>::iterator it = log_sistema_arquivos.begin(); it != log_sistema_arquivos.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }
    std::cout << "\n"
              << std::endl;

    ga.imprimir_mapa();

    for (int i = 0; i < processos.size(); i++)
        delete processos[i];
    return 0;
}