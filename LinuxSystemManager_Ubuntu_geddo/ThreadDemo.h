#ifndef THREADDEMO_H
#define THREADDEMO_H

#include <mutex>

class ThreadDemo
{
public:
    void run();

private:
    void worker(int id, int seconds);
    std::mutex outputMutex;
};

#endif
