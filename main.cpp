#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "processo.h"
#include "memoria.h"
#include "recursos.h"
#include "arquivos.h"
#include "escalonador.h"

// Funcões para ler os processos/arquivos
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

void configurar_arquivos(std::string f_files, std::vector<Processo *> &procs, GerenciadorArquivos &ga)
{
    std::ifstream file(f_files);
    std::string linha;

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
        Arquivo arq;
        arq.nome = nome;
        arq.bloco_inicial = std::stoi(s_start);
        arq.tamanho = std::stoi(s_len);
        arq.criador_pid = -1;
        arquivos_iniciais.push_back(arq);
    }
    ga.inicializar(total, arquivos_iniciais);

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
        for (int i = 0; i < procs.size(); i++)
        {
            if (procs[i]->PID == pid)
            {
                Instrucao inst;
                inst.pid = pid;
                inst.codigo = std::stoi(s_op);
                inst.arquivo = nome_limpo;
                inst.blocos = tam;
                procs[i]->instrucoes_io.push_back(inst);
            }
        }
    }
}

// Implementa o formato exigido na Seção 2.1.1 da especificação do trabalho.
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
    std::cout << "\tsata: " << p->SATA << std::endl;
}

int main(int argc, char **argv)
{
    std::string f_proc = "Testes/processes.txt";
    std::string f_files = "Testes/files.txt";
    if (argc > 1)
        f_proc = argv[1];
    if (argc > 2)
        f_files = argv[2];

    // Inicialização dos Módulos (O "Boot" do SO)
    std::vector<Processo *> processos = ler_processos(f_proc);
    GerenciadorArquivos ga;
    configurar_arquivos(f_files, processos, ga);
    GerenciadorMemoria mem;
    GerenciadorRecursos rec;
    Escalonador escalonador;

    int tempo = 0;        // Tempo do sistema
    Processo *cpu = NULL; // Quem esta usando a CPU agora
    int processos_finalizados = 0;
    int total_procs = processos.size();

    std::cout << ">>> INICIANDO SIMULAÇÃO<<<" << std::endl;

    // Validação
    // Antes de começar, verificamos se algum processo pede mais memória do que está disponível
    for (int i = 0; i < processos.size(); i++)
    {
        Processo *p = processos[i];
        bool matar = false;
        // Valida partição de Tempo Real (Max 64)
        if (p->prioridade_original == 0 && p->blocos_mem_req > 64)
            matar = true;
        // Valida partição de Usuário (Max 960)
        if (p->prioridade_original > 0 && p->blocos_mem_req > 960)
            matar = true;
        if (matar)
        {
            p->estado = FINALIZADO;
            processos_finalizados++;
            std::cout << "AVISO: Processo " << p->PID << " removido por falta de memoria." << std::endl;
        }
    }

    while (processos_finalizados < total_procs)
    {
        // Verifica na lista global se algo chegou
        for (int i = 0; i < processos.size(); i++)
        {
            Processo *p = processos[i];
            if (p->t_chegada == tempo && p->estado == PRONTO)
            {
                p->resetar_quantum();     // Define o tempo de vida na CPU
                escalonador.adicionar(p); // Coloca na fila correta (TR ou Usuário)
            }
        }

        // AGING (Envelhecimento)
        // Chama a função do escalonador que promove processos velhos de fila.
        escalonador.executar_aging();

        // DISPATCHER
        // Só tenta escolher alguém se a CPU estiver vazia.
        if (cpu == NULL)
        {
            // Cria uma lista de ponteiros para as filas para facilitar o loop
            // Evita repetir codigo 6 vezes
            std::vector<std::deque<Processo *> *> filas_ptr;
            filas_ptr.push_back(&escalonador.fila_tr);
            filas_ptr.push_back(&escalonador.fila_p1);
            filas_ptr.push_back(&escalonador.fila_p2);
            filas_ptr.push_back(&escalonador.fila_p3);
            filas_ptr.push_back(&escalonador.fila_p4);
            filas_ptr.push_back(&escalonador.fila_p5);

            bool escolheu_alguem = false;

            // Varre as filas da maior prioridade para a menor
            for (int f = 0; f < filas_ptr.size(); f++)
            {
                std::deque<Processo *> *fila_atual = filas_ptr[f];

                // Varre os processos dentro da fila
                // Se o primeiro nao der, tenta o segundo, etc.
                for (int i = 0; i < fila_atual->size(); i++)
                {
                    Processo *candidato = (*fila_atual)[i];

                    bool tem_memoria = (candidato->offset_memoria != -1);
                    bool tem_recursos = true;
                    bool alocou_memoria_agora = false;

                    if (!tem_memoria)
                    {
                        tem_recursos = rec.alocar(candidato);
                        if (tem_recursos)
                        {
                            // Verifica se tem memória o suficiente livre
                            tem_memoria = mem.alocar(candidato);
                            if (tem_memoria)
                            {
                                alocou_memoria_agora = true;
                            }
                            else
                            {
                                rec.liberar(candidato); // Nao coube, libera driver
                            }
                        }
                    }

                    // Se estiver tudo ok para rodar
                    if (tem_memoria && tem_recursos)
                    {
                        cpu = candidato;

                        // Remove da posicao 'i' dessa fila especifica
                        fila_atual->erase(fila_atual->begin() + i);

                        cpu->estado = EXECUTANDO;
                        print_dispatcher(cpu);
                        std::cout << "process " << cpu->PID << " =>" << std::endl;
                        std::cout << "P" << cpu->PID << " STARTED" << std::endl;

                        escolheu_alguem = true;
                        break; // Sai do loop da fila
                    }
                }
                if (escolheu_alguem)
                    break; // Sai do loop das filas (ja alocou)
            }
        }

        // Execução da CPU
        if (cpu != NULL)
        {
            // Simula execução de instrução
            if (cpu->pc < cpu->instrucoes_io.size())
            {
                std::cout << "P" << cpu->PID << " instruction " << (cpu->pc + 1) << std::endl;
            }

            cpu->t_restante--;
            cpu->quantum_restante--;

            // Verifica se o processo finalizou
            if (cpu->t_restante <= 0)
            {
                // Processa as operações de arquivo acumuladas
                std::cout << "P" << cpu->PID << " return SIGINT" << std::endl;
                std::cout << "Sistema de arquivos =>" << std::endl;
                for (int i = 0; i < cpu->instrucoes_io.size(); i++)
                {
                    std::string log = ga.operar(cpu, cpu->instrucoes_io[i]);
                    std::cout << log << std::endl;
                }
                mem.desalocar(cpu);
                rec.liberar(cpu);
                cpu->estado = FINALIZADO;
                cpu = NULL;
                processos_finalizados++;
            }
            // Verifica se houve preempção, quantum acabou
            else if (cpu->quantum_restante <= 0 && cpu->prioridade_original > 0)
            {
                // Se não estiver na última fila, a prioridade cai
                if (cpu->prioridade_atual < 5)
                    cpu->prioridade_atual++;
                // Recalcula novo quantum baseado na nova prioridade
                cpu->resetar_quantum();
                cpu->estado = PRONTO;
                // Devolve para o final da fila correspondente
                escalonador.adicionar(cpu);

                // Não é liberado memória nem recursos na preempção!
                cpu = NULL;
            }
        }
        tempo++;
    }

    ga.imprimir_mapa();
    return 0;
}