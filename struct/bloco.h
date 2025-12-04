#ifndef BLOCO_H
#define BLOCO_H

struct Bloco {
    int pid_dono; 
    Bloco(int pid = -1) : pid_dono(pid) {}
};

#endif