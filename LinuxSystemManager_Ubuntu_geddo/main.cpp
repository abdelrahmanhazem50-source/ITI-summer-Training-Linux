#include "FileManager.h"
#include "NetworkManager.h"
#include "ProcessManager.h"
#include "SystemInfo.h"
#include "ThreadDemo.h"

#include <iostream>
#include <string>

using namespace std;

void showMenu()
{
    cout << "\n========================================\n";
    cout << "        LINUX SYSTEM MANAGER\n";
    cout << "========================================\n\n";

    cout << "SYSTEM\n";
    cout << " 1. System Information\n";
    cout << " 2. Memory Information\n";
    cout << " 3. Disk Information\n\n";

    cout << "PROCESSES\n";
    cout << " 4. List Processes\n";
    cout << " 5. Process Information\n";
    cout << " 6. Create Child Process\n";
    cout << " 7. Send Signal to Process\n\n";

    cout << "FILES\n";
    cout << " 8. File Information\n";
    cout << " 9. Change File Permissions\n\n";

    cout << "NETWORK\n";
    cout << "10. Network Information\n";
    cout << "11. Ping Host\n";
    cout << "12. DNS Lookup\n\n";

    cout << "THREADS\n";
    cout << "13. Multi-Thread Demo\n\n";

    cout << " 0. Exit\n";
    cout << "========================================\n";
}

void pauseScreen()
{
    cout << "\nPress Enter to return to the menu...";
    string line;
    getline(cin, line);
}

int main()
{
    SystemInfo systemInfo;
    ProcessManager processManager;
    FileManager fileManager;
    NetworkManager networkManager;
    ThreadDemo threadDemo;

    while (true)
    {
        showMenu();
        cout << "Enter choice: ";

        string input;
        getline(cin, input);

        int choice;
        try
        {
            size_t position;
            choice = stoi(input, &position);
            if (position != input.length())
                throw invalid_argument("extra characters");
        }
        catch (...)
        {
            cout << "Invalid input. Please enter a number from 0 to 13.\n";
            pauseScreen();
            continue;
        }

        if (choice == 0)
        {
            cout << "Exiting Linux System Manager. Goodbye!\n";
            break;
        }

        switch (choice)
        {
        case 1:
            systemInfo.showSystemInfo();
            break;
        case 2:
            systemInfo.showMemoryInfo();
            break;
        case 3:
            systemInfo.showDiskInfo();
            break;
        case 4:
            processManager.listProcesses();
            break;
        case 5:
            processManager.showProcessInfo();
            break;
        case 6:
            processManager.createChildProcess();
            break;
        case 7:
            processManager.sendSignal();
            break;
        case 8:
            fileManager.showFileInfo();
            break;
        case 9:
            fileManager.changePermissions();
            break;
        case 10:
            networkManager.showNetworkInfo();
            break;
        case 11:
            networkManager.pingHost();
            break;
        case 12:
            networkManager.dnsLookup();
            break;
        case 13:
            threadDemo.run();
            break;
        default:
            cout << "Invalid menu option. Choose a number from 0 to 13.\n";
            break;
        }

        pauseScreen();
    }

    return 0;
}
