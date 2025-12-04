
class Bloco
{
private:
    int pid_dono;

public:
    Bloco(int pid = -1) : pid_dono(pid) {}
    int obter_pid_dono() { return pid_dono; }
    void definir_pid_dono(int pid) { pid_dono = pid; }
};
