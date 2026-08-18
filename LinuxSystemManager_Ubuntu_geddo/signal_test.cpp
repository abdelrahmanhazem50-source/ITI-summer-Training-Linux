#include <csignal>
#include <iostream>
#include <unistd.h>

using namespace std;

volatile sig_atomic_t keepRunning = 1;

void stopProcess(int)
{
    keepRunning = 0;
}

int main()
{
    signal(SIGTERM, stopProcess);
    signal(SIGINT, stopProcess);

    cout << "Signal test process started.\n";
    cout << "PID: " << getpid() << '\n';
    cout << "Use this PID with the Linux System Manager signal menu.\n";
    cout << "Press Ctrl+C or send SIGTERM to exit.\n";

    while (keepRunning)
        sleep(1);

    cout << "Signal test process is exiting.\n";
    return 0;
}
