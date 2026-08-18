#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <string>

class ProcessManager
{
public:
    void listProcesses();
    void showProcessInfo();
    void createChildProcess();
    void sendSignal();

private:
    bool readProcessStatus(int pid,
                           std::string &name,
                           std::string &state,
                           int &ppid,
                           int &threads,
                           std::string &vmSize,
                           std::string &vmRss);

    bool isNumber(const std::string &text);
    bool parsePid(const std::string &text, int &pid);
    std::string stateDescription(char state);
};

#endif
