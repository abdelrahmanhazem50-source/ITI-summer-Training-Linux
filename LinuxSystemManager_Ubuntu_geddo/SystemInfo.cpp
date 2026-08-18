#include "SystemInfo.h"

#include <sys/utsname.h>
#include <sys/statvfs.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static string formatBytes(unsigned long long bytes)
{
    const double KB = 1024.0;
    const double MB = KB * 1024.0;
    const double GB = MB * 1024.0;

    stringstream out;
    out << fixed << setprecision(2);

    if (bytes >= GB)
        out << bytes / GB << " GB";
    else if (bytes >= MB)
        out << bytes / MB << " MB";
    else if (bytes >= KB)
        out << bytes / KB << " KB";
    else
        out << bytes << " bytes";

    return out.str();
}

void SystemInfo::showSystemInfo()
{
    struct utsname info;

    if (uname(&info) == -1)
    {
        perror("uname");
        return;
    }

    cout << "\n--- System Information ---\n";
    cout << "Operating System     : " << info.sysname << '\n';
    cout << "Hostname             : " << info.nodename << '\n';
    cout << "Kernel Version       : " << info.release << '\n';
    cout << "Machine Architecture : " << info.machine << '\n';
}

void SystemInfo::showMemoryInfo()
{
    ifstream file("/proc/meminfo");

    if (!file)
    {
        cerr << "Could not open /proc/meminfo\n";
        return;
    }

    unsigned long long totalKB = 0;
    unsigned long long availableKB = 0;
    string key;
    unsigned long long value;
    string unit;

    while (file >> key >> value >> unit)
    {
        if (key == "MemTotal:")
            totalKB = value;
        else if (key == "MemAvailable:")
            availableKB = value;

        if (totalKB != 0 && availableKB != 0)
            break;
    }

    cout << "\n--- Memory Information ---\n";
    cout << "Total Memory     : " << formatBytes(totalKB * 1024ULL) << '\n';
    cout << "Available Memory : " << formatBytes(availableKB * 1024ULL) << '\n';
}

void SystemInfo::showDiskInfo()
{
    struct statvfs disk;

    if (statvfs("/", &disk) == -1)
    {
        perror("statvfs");
        return;
    }

    unsigned long long total =
        static_cast<unsigned long long>(disk.f_blocks) * disk.f_frsize;
    unsigned long long freeSpace =
        static_cast<unsigned long long>(disk.f_bavail) * disk.f_frsize;
    unsigned long long used = total -
        static_cast<unsigned long long>(disk.f_bfree) * disk.f_frsize;

    cout << "\n--- Disk Information (/) ---\n";
    cout << "Total Disk Space : " << formatBytes(total) << '\n';
    cout << "Used Disk Space  : " << formatBytes(used) << '\n';
    cout << "Free Disk Space  : " << formatBytes(freeSpace) << '\n';
}
