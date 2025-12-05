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

std::vector<Processo *> ler_processos(std::string arquivo);
void configurar_arquivos(std::string f_files, std::vector<Processo *> &procs, GerenciadorArquivos &ga, \
std::map<int, std::string> &logs);
void print_dispatcher(Processo *p);

