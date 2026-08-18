#include "ProcessManager.h"

#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool ProcessManager::isNumber(const string &text)
{
    if (text.empty())
        return false;

    for (char c : text)
    {
        if (c < '0' || c > '9')
            return false;
    }

    return true;
}

bool ProcessManager::parsePid(const string &text, int &pid)
{
    if (!isNumber(text))
        return false;

    try
    {
        long long value = stoll(text);
        if (value < 0 || value > 2147483647LL)
            return false;

        pid = static_cast<int>(value);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

string ProcessManager::stateDescription(char state)
{
    switch (state)
    {
    case 'R':
        return "Running";
    case 'S':
        return "Interruptible Sleep";
    case 'D':
        return "Uninterruptible Sleep";
    case 'Z':
        return "Zombie";
    case 'T':
    case 't':
        return "Stopped";
    case 'I':
        return "Idle";
    default:
        return "Other";
    }
}

bool ProcessManager::readProcessStatus(int pid,
                                       string &name,
                                       string &state,
                                       int &ppid,
                                       int &threads,
                                       string &vmSize,
                                       string &vmRss)
{
    string path = "/proc/" + to_string(pid) + "/status";
    ifstream file(path);

    if (!file)
        return false;

    name = "Unknown";
    state = "Unknown";
    ppid = -1;
    threads = -1;
    vmSize = "Not available";
    vmRss = "Not available";

    string line;
    while (getline(file, line))
    {
        if (line.rfind("Name:", 0) == 0)
            name = line.substr(6);
        else if (line.rfind("State:", 0) == 0)
            state = line.substr(7);
        else if (line.rfind("PPid:", 0) == 0)
            ppid = stoi(line.substr(6));
        else if (line.rfind("Threads:", 0) == 0)
            threads = stoi(line.substr(9));
        else if (line.rfind("VmSize:", 0) == 0)
            vmSize = line.substr(8);
        else if (line.rfind("VmRSS:", 0) == 0)
            vmRss = line.substr(7);
    }

    return true;
}

void ProcessManager::listProcesses()
{
    DIR *directory = opendir("/proc");

    if (directory == nullptr)
    {
        perror("opendir");
        return;
    }

    vector<int> pids;
    dirent *entry;

    while ((entry = readdir(directory)) != nullptr)
    {
        string name = entry->d_name;
        if (isNumber(name))
            pids.push_back(stoi(name));
    }

    closedir(directory);
    sort(pids.begin(), pids.end());

    cout << "\n--- Running Processes ---\n";
    cout << left << setw(8) << "PID"
         << setw(8) << "PPID"
         << setw(24) << "NAME"
         << "STATE\n";
    cout << string(72, '-') << '\n';

    for (int pid : pids)
    {
        string name;
        string state;
        string vmSize;
        string vmRss;
        int ppid;
        int threads;

        if (!readProcessStatus(pid, name, state, ppid, threads, vmSize, vmRss))
            continue;

        char stateCode = state.empty() ? '?' : state[0];
        string fullState = string(1, stateCode) + " (" + stateDescription(stateCode) + ")";

        if (name.length() > 22)
            name = name.substr(0, 22);

        cout << left << setw(8) << pid
             << setw(8) << ppid
             << setw(24) << name
             << fullState << '\n';
    }
}

void ProcessManager::showProcessInfo()
{
    string input;
    cout << "Enter PID: ";
    getline(cin, input);

    int pid;
    if (!parsePid(input, pid))
    {
        cout << "Invalid PID.\n";
        return;
    }
    string name;
    string state;
    string vmSize;
    string vmRss;
    int ppid;
    int threads;

    if (!readProcessStatus(pid, name, state, ppid, threads, vmSize, vmRss))
    {
        cout << "Process does not exist or cannot be read.\n";
        return;
    }

    char stateCode = state.empty() ? '?' : state[0];

    cout << "\n--- Process Information ---\n";
    cout << "PID               : " << pid << '\n';
    cout << "PPID              : " << ppid << '\n';
    cout << "Name              : " << name << '\n';
    cout << "State             : " << stateCode << " - " << stateDescription(stateCode) << '\n';
    cout << "Number of Threads : " << threads << '\n';
    cout << "Virtual Memory    : " << vmSize << '\n';
    cout << "Resident Memory   : " << vmRss << '\n';
}

void ProcessManager::createChildProcess()
{
    cout.flush();
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        cout << "\n[Child]\n";
        cout << "PID  = " << getpid() << '\n';
        cout << "PPID = " << getppid() << '\n';
        cout << "Child exits with status 42\n";
        cout.flush();
        _exit(42);
    }

    cout << "\n[Parent]\n";
    cout << "PID       = " << getpid() << '\n';
    cout << "Child PID = " << pid << '\n';

    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status))
        cout << "Child exit status = " << WEXITSTATUS(status) << '\n';
    else
        cout << "Child did not exit normally.\n";
}

void ProcessManager::sendSignal()
{
    string input;
    cout << "Enter PID: ";
    getline(cin, input);

    int pid;
    if (!parsePid(input, pid))
    {
        cout << "Invalid PID.\n";
        return;
    }

    if (pid <= 1)
    {
        cout << "For safety, PID 1 and lower are blocked.\n";
        return;
    }

    if (pid == getpid())
    {
        cout << "You cannot signal the Linux System Manager itself.\n";
        return;
    }

    errno = 0;
    if (kill(pid, 0) == -1 && errno == ESRCH)
    {
        cout << "Process does not exist.\n";
        return;
    }

    cout << "\n1. SIGTERM\n";
    cout << "2. SIGSTOP\n";
    cout << "3. SIGCONT\n";
    cout << "4. SIGKILL\n";
    cout << "Choice: ";

    string choiceText;
    getline(cin, choiceText);

    int signalNumber = 0;
    string signalName;

    if (choiceText == "1")
    {
        signalNumber = SIGTERM;
        signalName = "SIGTERM";
    }
    else if (choiceText == "2")
    {
        signalNumber = SIGSTOP;
        signalName = "SIGSTOP";
    }
    else if (choiceText == "3")
    {
        signalNumber = SIGCONT;
        signalName = "SIGCONT";
    }
    else if (choiceText == "4")
    {
        signalNumber = SIGKILL;
        signalName = "SIGKILL";
    }
    else
    {
        cout << "Invalid signal choice.\n";
        return;
    }

    cout << "Send " << signalName << " to PID " << pid << "? (y/n): ";
    string confirm;
    getline(cin, confirm);

    if (confirm != "y" && confirm != "Y")
    {
        cout << "Signal cancelled.\n";
        return;
    }

    if (kill(pid, signalNumber) == -1)
    {
        perror("kill");
        return;
    }

    cout << signalName << " sent successfully.\n";
}
