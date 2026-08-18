#include "NetworkManager.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <set>
#include <string>

using namespace std;

void NetworkManager::showNetworkInfo()
{
    ifaddrs *interfaces;

    if (getifaddrs(&interfaces) == -1)
    {
        perror("getifaddrs");
        return;
    }

    cout << "\n--- Network Interfaces ---\n";
    bool found = false;

    for (ifaddrs *current = interfaces; current != nullptr; current = current->ifa_next)
    {
        if (current->ifa_addr == nullptr)
            continue;

        if (current->ifa_addr->sa_family != AF_INET)
            continue;

        char address[INET_ADDRSTRLEN];
        sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(current->ifa_addr);

        if (inet_ntop(AF_INET, &ipv4->sin_addr, address, sizeof(address)) == nullptr)
            continue;

        cout << "Interface : " << current->ifa_name << '\n';
        cout << "IP        : " << address << '\n';
        cout << "State     : " << ((current->ifa_flags & IFF_UP) ? "UP" : "DOWN") << "\n\n";
        found = true;
    }

    freeifaddrs(interfaces);

    if (!found)
        cout << "No IPv4 interfaces found.\n";
}

void NetworkManager::pingHost()
{
    cout << "Enter host: ";
    string host;
    getline(cin, host);

    if (host.empty())
    {
        cout << "Host cannot be empty.\n";
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        execlp("ping", "ping", "-c", "3", host.c_str(), static_cast<char *>(nullptr));
        perror("exec ping");
        _exit(1);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        cout << "Ping finished successfully.\n";
    else
        cout << "Ping failed or host is unreachable.\n";
}

void NetworkManager::dnsLookup()
{
    cout << "Enter hostname: ";
    string hostname;
    getline(cin, hostname);

    if (hostname.empty())
    {
        cout << "Hostname cannot be empty.\n";
        return;
    }

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *result;
    int code = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);

    if (code != 0)
    {
        cout << "DNS lookup failed: " << gai_strerror(code) << '\n';
        return;
    }

    set<string> shownAddresses;
    cout << "\n--- DNS Lookup ---\n";

    for (addrinfo *current = result; current != nullptr; current = current->ai_next)
    {
        char host[NI_MAXHOST];
        int nameCode = getnameinfo(current->ai_addr,
                                   current->ai_addrlen,
                                   host,
                                   sizeof(host),
                                   nullptr,
                                   0,
                                   NI_NUMERICHOST);

        if (nameCode != 0)
            continue;

        string address = host;
        if (shownAddresses.insert(address).second)
        {
            string family = (current->ai_family == AF_INET) ? "IPv4" : "IPv6";
            cout << family << " : " << address << '\n';
        }
    }

    freeaddrinfo(result);
}
