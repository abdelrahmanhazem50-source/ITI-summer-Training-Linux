#include "ThreadDemo.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void ThreadDemo::worker(int id, int seconds)
{
    {
        lock_guard<mutex> lock(outputMutex);
        cout << "Thread " << id << " started\n";
        cout << "Thread " << id << " working for " << seconds << " second(s)...\n";
    }

    this_thread::sleep_for(chrono::seconds(seconds));

    {
        lock_guard<mutex> lock(outputMutex);
        cout << "Thread " << id << " finished\n";
    }
}

void ThreadDemo::run()
{
    cout << "\n--- Multi-Thread Demo ---\n";

    thread thread1(&ThreadDemo::worker, this, 1, 1);
    thread thread2(&ThreadDemo::worker, this, 2, 2);
    thread thread3(&ThreadDemo::worker, this, 3, 3);

    thread1.join();
    thread2.join();
    thread3.join();

    cout << "All threads finished.\n";
}
