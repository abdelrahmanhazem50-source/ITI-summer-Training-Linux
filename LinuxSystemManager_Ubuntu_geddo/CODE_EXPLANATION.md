# Linux System Manager - Simple Code Explanation

This guide explains the project in simple language, section by section. The goal is to help you understand what every important part does well enough to explain it during a discussion or viva.

---

# 1. The Big Picture

The program is split into classes so that every class has one job:

```text
main.cpp
   |
   +-- SystemInfo       -> system, memory and disk information
   +-- ProcessManager   -> processes, fork, wait and signals
   +-- FileManager      -> file information and permissions
   +-- NetworkManager   -> interfaces, ping and DNS
   +-- ThreadDemo       -> multithreading example
```

`main.cpp` does not contain the Linux implementation itself. It creates objects from those classes, shows the menu, reads the user's choice and calls the correct function.

That separation is the main OOP idea in this project: **one class = one responsibility**.

---

# 2. Header Files - What Are They For?

The `.h` files tell the compiler what each class contains. The actual function code is placed in the matching `.cpp` file.

For example:

```cpp
class SystemInfo
{
public:
    void showSystemInfo();
    void showMemoryInfo();
    void showDiskInfo();
};
```

This means the `SystemInfo` class has three functions that other parts of the program are allowed to call.

The header does **not** explain how those functions work. Their implementation is inside `SystemInfo.cpp`.

This keeps the project organized and prevents `main.cpp` from becoming one huge file.

---

# 3. main.cpp

## 3.1 Including our classes

```cpp
#include "FileManager.h"
#include "NetworkManager.h"
#include "ProcessManager.h"
#include "SystemInfo.h"
#include "ThreadDemo.h"
```

These lines allow `main.cpp` to create and use the five classes in the project.

The quotes `"..."` are used because these are our own header files, not standard C++ headers.

Then we include standard C++ tools:

```cpp
#include <iostream>
#include <string>
```

- `iostream` gives us `cout` and `cin`.
- `string` gives us `std::string`.

```cpp
using namespace std;
```

This lets us write `cout` instead of `std::cout`, `string` instead of `std::string`, and so on.

---

## 3.2 The menu function

```cpp
void showMenu()
{
    cout << "\n========================================\n";
    cout << "        LINUX SYSTEM MANAGER\n";
    ...
}
```

`showMenu()` only prints the menu.

It returns `void` because it does not need to return a value.

Keeping menu printing in its own function makes `main()` easier to read.

---

## 3.3 Pause before returning to the menu

```cpp
void pauseScreen()
{
    cout << "\nPress Enter to return to the menu...";
    string line;
    getline(cin, line);
}
```

After a feature finishes, this waits for the user to press Enter.

`getline()` is used instead of `cin >>` so input handling stays consistent throughout the program.

---

## 3.4 Creating the manager objects

```cpp
SystemInfo systemInfo;
ProcessManager processManager;
FileManager fileManager;
NetworkManager networkManager;
ThreadDemo threadDemo;
```

This creates one object from each class.

Think of it like this:

```text
Class = blueprint
Object = actual usable instance of that blueprint
```

For example, `ProcessManager` is the class and `processManager` is the object we call functions on.

---

## 3.5 Keeping the program running

```cpp
while (true)
{
    showMenu();
    ...
}
```

This is an infinite loop. The program keeps showing the menu until the user chooses `0`.

---

## 3.6 Reading and validating the menu choice

```cpp
string input;
getline(cin, input);
```

The choice is first read as text.

Why not read it directly as an integer? Reading text first makes invalid input such as `abc` easier to handle safely.

Then:

```cpp
choice = stoi(input, &position);
```

`stoi()` converts the string into an integer.

For example:

```text
"12" -> 12
```

The `position` variable tells us how many characters were successfully converted.

```cpp
if (position != input.length())
    throw invalid_argument("extra characters");
```

This rejects input such as:

```text
12abc
```

because only part of it is a valid number.

The conversion is surrounded by:

```cpp
try
{
    ...
}
catch (...)
{
    cout << "Invalid input...";
}
```

So bad user input does not crash the application.

---

## 3.7 Exiting

```cpp
if (choice == 0)
{
    cout << "Exiting Linux System Manager. Goodbye!\n";
    break;
}
```

`break` exits the `while (true)` loop, so the program reaches the end of `main()` and finishes.

---

## 3.8 Calling the correct class

```cpp
switch (choice)
{
case 1:
    systemInfo.showSystemInfo();
    break;
case 2:
    systemInfo.showMemoryInfo();
    break;
...
}
```

The `switch` connects menu numbers to class functions.

Example:

```text
User enters 4
    -> case 4
    -> processManager.listProcesses()
    -> ProcessManager handles /proc
```

This is why `main.cpp` stays simple: it controls the application but does not perform all the Linux work itself.

---

# 4. SystemInfo Class

The class declaration is intentionally small:

```cpp
class SystemInfo
{
public:
    void showSystemInfo();
    void showMemoryInfo();
    void showDiskInfo();
};
```

It handles three system-related tasks.

---

# 5. SystemInfo.cpp

## 5.1 Converting bytes into readable sizes

```cpp
static string formatBytes(unsigned long long bytes)
```

Linux APIs often give sizes as raw byte numbers. This helper changes something like:

```text
17179869184 bytes
```

into:

```text
16.00 GB
```

The constants are:

```cpp
const double KB = 1024.0;
const double MB = KB * 1024.0;
const double GB = MB * 1024.0;
```

Then the code decides which unit is suitable:

```cpp
if (bytes >= GB)
    out << bytes / GB << " GB";
else if (bytes >= MB)
    ...
```

`fixed << setprecision(2)` limits the result to two decimal places.

The function is marked `static` because it is only a helper used inside this `.cpp` file.

---

## 5.2 Basic system information with uname()

```cpp
struct utsname info;
```

`utsname` is a Linux structure designed to hold operating-system information.

Then:

```cpp
if (uname(&info) == -1)
{
    perror("uname");
    return;
}
```

`uname()` asks the Linux kernel to fill the `info` structure.

If it fails, it returns `-1` and `perror()` prints the Linux error message.

After success:

```cpp
cout << "Operating System     : " << info.sysname << '\n';
cout << "Hostname             : " << info.nodename << '\n';
cout << "Kernel Version       : " << info.release << '\n';
cout << "Machine Architecture : " << info.machine << '\n';
```

Important fields:

- `sysname` -> operating system name
- `nodename` -> hostname
- `release` -> kernel release/version
- `machine` -> CPU architecture, such as `x86_64`

You can compare this feature with:

```bash
uname -a
```

---

## 5.3 Memory information from /proc/meminfo

```cpp
ifstream file("/proc/meminfo");
```

`/proc` is a virtual filesystem created by the Linux kernel. It contains live system information.

`/proc/meminfo` contains values such as:

```text
MemTotal:       ... kB
MemAvailable:   ... kB
```

The loop:

```cpp
while (file >> key >> value >> unit)
```

reads each row as three pieces:

```text
key       value      unit
MemTotal: 16000000   kB
```

Then:

```cpp
if (key == "MemTotal:")
    totalKB = value;
else if (key == "MemAvailable:")
    availableKB = value;
```

Only the two values required by the project are stored.

Finally:

```cpp
formatBytes(totalKB * 1024ULL)
```

The `/proc/meminfo` value is in kilobytes, so multiplying by `1024` converts it to bytes before `formatBytes()` formats it.

You can compare the output with:

```bash
free -h
```

---

## 5.4 Disk information with statvfs()

```cpp
struct statvfs disk;
```

This structure stores filesystem statistics.

```cpp
statvfs("/", &disk)
```

asks Linux for information about the filesystem mounted at `/`, which is the root filesystem.

The main values used are:

- `f_blocks` -> total blocks
- `f_bfree` -> total free blocks
- `f_bavail` -> free blocks available to a normal user
- `f_frsize` -> size of one block

Total disk size:

```cpp
unsigned long long total =
    static_cast<unsigned long long>(disk.f_blocks) * disk.f_frsize;
```

The API works in blocks, so number of blocks is multiplied by block size.

Free space:

```cpp
unsigned long long freeSpace =
    static_cast<unsigned long long>(disk.f_bavail) * disk.f_frsize;
```

Used space:

```cpp
unsigned long long used = total -
    static_cast<unsigned long long>(disk.f_bfree) * disk.f_frsize;
```

The results are then passed to `formatBytes()`.

You can compare this with:

```bash
df -h /
```

---

# 6. ProcessManager Class

This is the biggest class because process management is one of the main topics of the project.

Public functions:

```cpp
void listProcesses();
void showProcessInfo();
void createChildProcess();
void sendSignal();
```

Private helper functions:

```cpp
bool readProcessStatus(...);
bool isNumber(...);
bool parsePid(...);
string stateDescription(...);
```

The private functions are implementation details. `main.cpp` does not need to call them directly.

---

# 7. ProcessManager.cpp

## 7.1 Checking whether text contains only digits

```cpp
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
```

This function is used for two things:

1. determining whether `/proc` directory names are PIDs
2. validating PID input from the user

Examples:

```text
"1234" -> true
"12a4" -> false
""     -> false
```

---

## 7.2 Safely converting a PID

```cpp
bool ProcessManager::parsePid(const string &text, int &pid)
```

First:

```cpp
if (!isNumber(text))
    return false;
```

So non-numeric text is rejected.

Then:

```cpp
long long value = stoll(text);
```

The string is temporarily converted to `long long` so very large numbers can be checked before converting them to `int`.

```cpp
if (value < 0 || value > 2147483647LL)
    return false;
```

This prevents integer overflow.

Then:

```cpp
pid = static_cast<int>(value);
```

The safe value is stored in the output reference variable `pid`.

---

## 7.3 Translating process state letters

Linux represents process states using letters.

```cpp
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
}
```

The important states are:

```text
R = Running or ready to run
S = Interruptible sleep, normally waiting for something
D = Uninterruptible sleep, usually waiting on kernel/I/O work
T = Stopped
Z = Zombie; process finished but parent has not collected its status yet
I = Idle kernel thread
```

This directly connects to the process-state material covered in the training notes.

---

## 7.4 Reading /proc/<PID>/status

```cpp
string path = "/proc/" + to_string(pid) + "/status";
ifstream file(path);
```

For a process with PID `1234`, this becomes:

```text
/proc/1234/status
```

That file contains information such as:

```text
Name:
State:
PPid:
Threads:
VmSize:
VmRSS:
```

The function gives default values first:

```cpp
name = "Unknown";
state = "Unknown";
ppid = -1;
threads = -1;
vmSize = "Not available";
vmRss = "Not available";
```

Then it reads the file line by line:

```cpp
while (getline(file, line))
```

This check:

```cpp
if (line.rfind("Name:", 0) == 0)
```

means: "Does this line start with `Name:`?"

The same idea is used for `State`, `PPid`, `Threads`, `VmSize` and `VmRSS`.

Example:

```cpp
name = line.substr(6);
```

removes the `Name:` label and keeps the actual process name.

`VmSize` is total virtual memory used by the process.

`VmRSS` is the resident memory currently held in RAM.

---

## 7.5 Listing every process

```cpp
DIR *directory = opendir("/proc");
```

This opens the `/proc` directory.

Inside `/proc`, process directories look like:

```text
1
500
1234
8420
```

but there are also non-process directories/files such as:

```text
cpuinfo
meminfo
sys
net
```

The program reads entries using:

```cpp
while ((entry = readdir(directory)) != nullptr)
```

Then:

```cpp
string name = entry->d_name;
if (isNumber(name))
    pids.push_back(stoi(name));
```

Only names made entirely of digits are treated as process IDs.

After scanning:

```cpp
closedir(directory);
sort(pids.begin(), pids.end());
```

The directory is closed and the PIDs are sorted from smallest to largest.

For every PID:

```cpp
readProcessStatus(pid, name, state, ppid, threads, vmSize, vmRss)
```

is called to collect its information.

The final result is printed as a table containing:

```text
PID | PPID | NAME | STATE
```

A process can disappear while `/proc` is being scanned. If that happens, `readProcessStatus()` fails and the code simply continues:

```cpp
if (!readProcessStatus(...))
    continue;
```

That prevents a normal process exit from crashing the program.

---

## 7.6 Showing one process

```cpp
cout << "Enter PID: ";
getline(cin, input);
```

The PID is validated by `parsePid()`.

Then `readProcessStatus()` reads the process data.

If `/proc/<PID>/status` cannot be opened:

```cpp
cout << "Process does not exist or cannot be read.\n";
```

Otherwise the program prints:

- PID
- PPID
- process name
- state
- number of threads
- virtual memory
- resident memory

This is basically a small custom version of reading process information that commands such as `ps` also obtain from the kernel.

---

## 7.7 Creating a child with fork()

```cpp
pid_t pid = fork();
```

`fork()` creates a new process by duplicating the current process.

After `fork()`, both parent and child continue from the next line, but they see different return values.

```text
pid < 0  -> fork failed
pid == 0 -> this code is running in the child
pid > 0  -> this code is running in the parent; pid is the child's PID
```

This is implemented as:

```cpp
if (pid < 0)
{
    perror("fork");
    return;
}

if (pid == 0)
{
    // child
}

// parent continues here
```

### Child section

```cpp
cout << "PID  = " << getpid() << '\n';
cout << "PPID = " << getppid() << '\n';
```

- `getpid()` returns the child's own PID.
- `getppid()` returns its parent's PID.

Then:

```cpp
_exit(42);
```

The child terminates with exit status `42`.

`_exit()` is useful directly after `fork()` because it terminates the child immediately without normal C++ stream cleanup being performed again.

### Parent section

The parent prints its own PID and the child's PID.

Then:

```cpp
waitpid(pid, &status, 0);
```

The parent waits until that specific child finishes.

This is important because the parent needs to collect the child's exit status. It also prevents the finished child from remaining as a zombie process.

Then:

```cpp
if (WIFEXITED(status))
    cout << WEXITSTATUS(status);
```

- `WIFEXITED(status)` asks whether the child exited normally.
- `WEXITSTATUS(status)` extracts its exit code, which should be `42`.

The lifecycle is:

```text
Parent
  |
 fork()
  |
  +-------- Child
  |           |
  |         _exit(42)
  |           |
  +--- waitpid()
        |
      receives 42
```

---

## 7.8 Sending Linux signals

The signal function first asks for a PID and validates it.

For safety:

```cpp
if (pid <= 1)
```

blocks PID 1 and lower.

PID 1 is normally the system init/service manager and should not be used for experiments.

The manager also protects itself:

```cpp
if (pid == getpid())
```

so you cannot accidentally kill the Linux System Manager from its own menu.

### Checking whether a process exists

```cpp
if (kill(pid, 0) == -1 && errno == ESRCH)
```

`kill(pid, 0)` does **not** actually send a signal.

Signal `0` is commonly used to check whether the PID exists and whether the caller can address it.

`ESRCH` means there is no such process.

### Choosing a signal

The menu maps choices to real Linux signal constants:

```cpp
signalNumber = SIGTERM;
signalNumber = SIGSTOP;
signalNumber = SIGCONT;
signalNumber = SIGKILL;
```

Meaning:

```text
SIGTERM -> asks a process to terminate; it can normally handle this signal
SIGSTOP -> immediately pauses/stops the process
SIGCONT -> resumes a stopped process
SIGKILL -> immediately kills a process and cannot be handled/ignored
```

The confirmation:

```cpp
Send SIGTERM to PID 1234? (y/n)
```

adds another safety step.

Finally:

```cpp
kill(pid, signalNumber)
```

asks the kernel to send the chosen signal to the process.

---

# 8. FileManager Class

The public functions are:

```cpp
void showFileInfo();
void changePermissions();
```

Three private helpers keep the main functions clean:

```cpp
getFileType()
getPermissionString()
validPermission()
```

---

# 9. FileManager.cpp

## 9.1 Detecting a file type

Linux stores file type information inside the `mode` field returned by `stat()`/`lstat()`.

The code checks it using macros:

```cpp
if (S_ISREG(mode))
    return "Regular file";
if (S_ISDIR(mode))
    return "Directory";
if (S_ISLNK(mode))
    return "Symbolic link";
```

Other supported types include:

- character device
- block device
- FIFO/pipe
- socket

This is important because in Linux many different object types are represented through filesystem entries.

---

## 9.2 Converting permission bits to rwxrwxrwx

Permissions are stored as bits inside `mode_t`.

Example:

```cpp
permissions += (mode & S_IRUSR) ? 'r' : '-';
```

`S_IRUSR` means:

```text
owner read permission
```

If that bit exists, append `r`; otherwise append `-`.

The code repeats this for:

```text
Owner:  r w x
Group:  r w x
Others: r w x
```

So a mode can become:

```text
rwxr-xr-x
```

which is octal:

```text
755
```

---

## 9.3 Validating an octal permission

```cpp
if (permission.length() != 3 && permission.length() != 4)
    return false;
```

Normal permission inputs are commonly three digits, such as:

```text
755
644
700
```

Four digits are also accepted for special mode bits.

Then every character is checked:

```cpp
if (c < '0' || c > '7')
    return false;
```

Octal numbers only use digits `0` through `7`.

So:

```text
755 -> valid
644 -> valid
999 -> invalid
7a5 -> invalid
```

---

## 9.4 Getting file information

```cpp
struct stat info;
```

This structure will hold the file's metadata.

```cpp
lstat(path.c_str(), &info)
```

fills it.

`lstat()` is used here instead of normal `stat()` so if the path is a symbolic link, the program can report the link itself as a symbolic link instead of automatically following it to its target.

Important fields:

```text
st_size -> file size
st_mode -> file type + permissions
st_uid  -> owner's user ID
st_gid  -> group's ID
```

Permission text is produced by:

```cpp
getPermissionString(info.st_mode)
```

The octal form is printed with:

```cpp
(info.st_mode & 0777)
```

The `0777` mask keeps only the normal owner/group/other permission bits.

`oct` temporarily tells C++ to print the number in octal instead of decimal.

---

## 9.5 Changing permissions with chmod()

First the program checks that the file exists:

```cpp
stat(path.c_str(), &info)
```

Then it asks for text such as `755`.

The input is converted from base 8:

```cpp
mode_t mode = static_cast<mode_t>(stoul(permission, nullptr, 8));
```

The `8` tells `stoul()` that the string is octal.

Then:

```cpp
chmod(path.c_str(), mode)
```

changes the file's permissions.

For example:

```text
755 = rwxr-xr-x
644 = rw-r--r--
700 = rwx------
```

---

# 10. NetworkManager Class

It has three public tasks:

```cpp
void showNetworkInfo();
void pingHost();
void dnsLookup();
```

The project purposely keeps networking basic.

---

# 11. NetworkManager.cpp

## 11.1 Getting network interfaces

```cpp
ifaddrs *interfaces;
getifaddrs(&interfaces);
```

`getifaddrs()` asks Linux for a linked list of network interfaces and their addresses.

The loop:

```cpp
for (ifaddrs *current = interfaces;
     current != nullptr;
     current = current->ifa_next)
```

walks through the linked list one interface/address entry at a time.

Some entries may not have an address:

```cpp
if (current->ifa_addr == nullptr)
    continue;
```

The project only displays IPv4:

```cpp
if (current->ifa_addr->sa_family != AF_INET)
    continue;
```

`AF_INET` means IPv4.

Then:

```cpp
sockaddr_in *ipv4 =
    reinterpret_cast<sockaddr_in *>(current->ifa_addr);
```

converts the generic socket-address pointer into an IPv4-specific structure.

`inet_ntop()` converts the binary IP address into normal readable text:

```text
binary address -> "192.168.1.10"
```

The interface state is checked with:

```cpp
(current->ifa_flags & IFF_UP) ? "UP" : "DOWN"
```

Finally:

```cpp
freeifaddrs(interfaces);
```

releases the linked list allocated by `getifaddrs()`.

---

## 11.2 Ping without system()

The project requirement specifically says not to do this:

```cpp
system("ping ...");
```

Instead the program demonstrates real process management.

First:

```cpp
pid_t pid = fork();
```

A child process is created.

Inside the child:

```cpp
execlp("ping", "ping", "-c", "3", host.c_str(), nullptr);
```

`execlp()` replaces the child process with the Linux `ping` program.

Arguments become approximately:

```bash
ping -c 3 google.com
```

Important difference:

```text
fork() -> creates another process
exec() -> replaces the current process program with another executable
```

If `exec()` succeeds, it never returns to the old child code.

If it fails:

```cpp
perror("exec ping");
_exit(1);
```

The parent waits:

```cpp
waitpid(pid, &status, 0)
```

Then:

```cpp
if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
```

checks whether `ping` ended normally with exit code `0`, which normally means success.

So the sequence is:

```text
Linux System Manager
       |
     fork()
       |
   +---+---+
   |       |
Parent   Child
   |       |
waitpid  execlp("ping")
   |       |
   |      ping program
   |       |
   +-------+
      status
```

---

## 11.3 DNS lookup

```cpp
addrinfo hints{};
```

This creates a configuration structure and initializes it to zero.

```cpp
hints.ai_family = AF_UNSPEC;
```

means both IPv4 and IPv6 are acceptable.

```cpp
hints.ai_socktype = SOCK_STREAM;
```

asks for addresses suitable for stream sockets, which helps avoid some duplicate records.

Then:

```cpp
getaddrinfo(hostname.c_str(), nullptr, &hints, &result)
```

asks the resolver to turn a hostname such as:

```text
google.com
```

into network addresses.

The returned result is another linked list.

For each item:

```cpp
getnameinfo(..., NI_NUMERICHOST)
```

converts the socket address into numeric IP text.

A `set<string>` is used:

```cpp
set<string> shownAddresses;
```

so the same IP address is not printed more than once.

Finally:

```cpp
freeaddrinfo(result);
```

releases memory allocated by `getaddrinfo()`.

---

# 12. ThreadDemo Class

Header:

```cpp
class ThreadDemo
{
public:
    void run();

private:
    void worker(int id, int seconds);
    std::mutex outputMutex;
};
```

`run()` starts the demo.

`worker()` is the function each thread runs.

`outputMutex` prevents several threads from writing to `cout` at exactly the same time.

---

# 13. ThreadDemo.cpp

## 13.1 What each worker does

```cpp
void ThreadDemo::worker(int id, int seconds)
```

Each worker receives:

- an ID, such as `1`
- how many seconds it should simulate work

Before printing:

```cpp
lock_guard<mutex> lock(outputMutex);
```

This locks the mutex.

Only one thread can hold that mutex at a time, so messages do not get mixed together on the terminal.

When the block ends, `lock_guard` automatically unlocks the mutex.

Then:

```cpp
this_thread::sleep_for(chrono::seconds(seconds));
```

pauses only that thread for the chosen time.

It does not stop the other threads.

---

## 13.2 Creating three threads

```cpp
thread thread1(&ThreadDemo::worker, this, 1, 1);
thread thread2(&ThreadDemo::worker, this, 2, 2);
thread thread3(&ThreadDemo::worker, this, 3, 3);
```

Each line creates a new thread that runs `ThreadDemo::worker`.

Breaking the first one down:

```cpp
&ThreadDemo::worker
```

is the member function to run.

```cpp
this
```

is the current `ThreadDemo` object.

```cpp
1, 1
```

are arguments passed to the worker:

```text
id = 1
seconds = 1
```

So all three threads can work concurrently.

---

## 13.3 Why join() is needed

```cpp
thread1.join();
thread2.join();
thread3.join();
```

`join()` makes the current thread wait until the selected worker thread has finished.

Without joining a still-running `std::thread`, destroying the thread object would cause the program to terminate.

So `join()` also ensures the demo does not print:

```text
All threads finished.
```

until they have actually all finished.

---

# 14. Process vs Thread - Simple Explanation

A **process** is an independent running instance of a program.

A **thread** is a unit of execution inside a process.

Simple comparison:

| Process | Thread |
|---|---|
| Has its own virtual address space | Shares the process address space |
| More isolated | Less isolated |
| Created here with `fork()` | Created here with `std::thread` |
| IPC is normally needed to share data | Can directly share globals/heap/files of the process |
| Heavier | Lighter |

Threads of the same process share things such as:

- program code
- global variables
- heap memory
- open files

Each thread has its own:

- stack
- CPU registers
- program counter
- thread ID

---

# 15. signal_test.cpp

This small program exists only so you can safely test signals.

## 15.1 Signal-safe flag

```cpp
volatile sig_atomic_t keepRunning = 1;
```

This is a simple variable that the signal handler can safely change.

`1` means keep looping.

---

## 15.2 Signal handler

```cpp
void stopProcess(int)
{
    keepRunning = 0;
}
```

When a handled termination signal arrives, the flag becomes `0`.

---

## 15.3 Registering the handler

```cpp
signal(SIGTERM, stopProcess);
signal(SIGINT, stopProcess);
```

This tells Linux/C++:

```text
When SIGTERM or SIGINT arrives, call stopProcess().
```

`SIGINT` is what you normally send with `Ctrl+C`.

---

## 15.4 Printing its PID

```cpp
cout << "PID: " << getpid() << '\n';
```

This gives you a safe PID to enter in menu option 7 of the main application.

---

## 15.5 Waiting until a signal arrives

```cpp
while (keepRunning)
    sleep(1);
```

The test process stays alive but uses almost no CPU because it sleeps most of the time.

You can experiment with:

```text
SIGSTOP -> process stops
SIGCONT -> process continues
SIGTERM -> handler runs and program exits cleanly
SIGKILL -> kernel kills it immediately; the handler cannot run
```

---

# 16. Makefile

The Makefile lets you build the whole project by typing:

```bash
make
```

instead of writing a long `g++` command every time.

---

## 16.1 Compiler

```make
CXX = g++
```

Use the GNU C++ compiler.

---

## 16.2 Compiler options

```make
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -pthread
```

Meaning:

```text
-std=c++17 -> compile using C++17
-Wall      -> enable common warnings
-Wextra    -> enable extra warnings
-pedantic  -> warn about non-standard C++
-pthread   -> enable/link POSIX threading support needed by std::thread
```

---

## 16.3 Main executable name

```make
TARGET = linux_system_manager
```

This is the final main program name.

---

## 16.4 Source files

```make
SOURCES = main.cpp SystemInfo.cpp ProcessManager.cpp FileManager.cpp NetworkManager.cpp ThreadDemo.cpp
```

These are the files that form the main application.

---

## 16.5 Turning .cpp names into .o names

```make
OBJECTS = $(SOURCES:.cpp=.o)
```

This automatically changes:

```text
main.cpp -> main.o
SystemInfo.cpp -> SystemInfo.o
...
```

Object files are compiled pieces that are later linked together.

---

## 16.6 Default build target

```make
all: $(TARGET) signal_test
```

Typing `make` builds both:

```text
linux_system_manager
signal_test
```

---

## 16.7 Linking the main application

```make
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
```

Once all `.o` files exist, `g++` links them into the final executable.

---

## 16.8 Generic compile rule

```make
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```

This means:

```text
For any .cpp file, compile it into its matching .o file.
```

For example:

```bash
g++ ... -c SystemInfo.cpp -o SystemInfo.o
```

---

## 16.9 Cleaning generated files

```make
clean:
	rm -f $(OBJECTS) $(TARGET) signal_test
```

Running:

```bash
make clean
```

removes compiled files so you can perform a clean rebuild.

---

# 17. Complete Program Flow

This is the easiest way to understand the whole application:

```text
START
  |
  v
main()
  |
  +-- create five manager objects
  |
  v
while(true)
  |
  +-- print menu
  |
  +-- read choice safely
  |
  +-- switch(choice)
  |      |
  |      +-- 1-3  -> SystemInfo
  |      +-- 4-7  -> ProcessManager
  |      +-- 8-9  -> FileManager
  |      +-- 10-12 -> NetworkManager
  |      +-- 13   -> ThreadDemo
  |
  +-- wait for Enter
  |
  +-- show menu again
  |
  +-- choice 0 -> break
  v
END
```

---

# 18. Linux Concepts Demonstrated by Each Menu Option

| Menu | Feature | Linux/C++ concept |
|---:|---|---|
| 1 | System Information | `uname()` |
| 2 | Memory Information | `/proc/meminfo` |
| 3 | Disk Information | `statvfs()` |
| 4 | List Processes | `/proc`, `opendir()`, `readdir()` |
| 5 | Process Information | `/proc/<PID>/status` |
| 6 | Child Process | `fork()`, `getpid()`, `getppid()`, `_exit()`, `waitpid()` |
| 7 | Signals | `kill()`, `SIGTERM`, `SIGSTOP`, `SIGCONT`, `SIGKILL` |
| 8 | File Information | `lstat()`, `struct stat` |
| 9 | Permissions | `chmod()` and octal permissions |
| 10 | Network Information | `getifaddrs()`, `inet_ntop()` |
| 11 | Ping | `fork()`, `execlp()`, `waitpid()` |
| 12 | DNS Lookup | `getaddrinfo()`, `getnameinfo()` |
| 13 | Threads | `std::thread`, `sleep_for()`, `mutex`, `join()` |

---

# 19. Error Handling Used in the Project

There are two main styles of error handling.

## Linux API errors

Functions such as:

```cpp
uname()
statvfs()
fork()
waitpid()
chmod()
getifaddrs()
```

usually signal failure through return values.

The code checks those values and often uses:

```cpp
perror("function name");
```

`perror()` prints the text corresponding to the current Linux `errno` value.

For example, a missing file might produce:

```text
stat: No such file or directory
```

## User input errors

Examples:

- invalid menu choice
- invalid PID text
- empty hostname
- invalid permission number
- nonexistent process

These are handled with checks and messages instead of allowing the program to crash.

---

# 20. Why system() Is Not Used

The project requirement is about learning Linux APIs, so doing this would defeat the purpose:

```cpp
system("ps");
system("df -h");
system("ip addr");
system("ping ...");
```

Instead the project directly uses:

```text
/proc
statvfs()
getifaddrs()
fork() + exec() + waitpid()
```

This proves that the application itself interacts with Linux rather than simply asking the shell to do all the work.

---

# 21. Useful Viva / Discussion Answers

## What is `/proc`?

`/proc` is a virtual filesystem maintained by the Linux kernel. It is not normal data permanently stored on the disk. It exposes live information about the system and running processes.

## Why does every process have a PID?

The PID uniquely identifies a process while it is running so the kernel and applications can refer to it.

## What is PPID?

PPID is the parent process ID. It identifies the process that created the current process.

## What does fork() do?

`fork()` creates a child process based on the current process. It returns `0` in the child and the child's PID in the parent.

## Why call waitpid()?

The parent uses `waitpid()` to wait for a child and collect its exit status. Collecting the status also prevents a terminated child from remaining as a zombie.

## Why does the child exit with 42?

`42` is simply a chosen demonstration value. It proves that the parent can retrieve a specific exit status from the child.

## What is a zombie?

A zombie is a child process that has already finished but whose parent has not yet collected its exit status using `wait()` or `waitpid()`.

## What is a signal?

A signal is a notification sent to a process to cause or request some action, such as stop, continue or terminate.

## SIGTERM vs SIGKILL?

`SIGTERM` requests termination and can normally be handled by the process. `SIGKILL` immediately kills the process and cannot be caught, blocked or ignored.

## What does chmod 755 mean?

Each digit represents permissions:

```text
7 = rwx
5 = r-x
5 = r-x
```

So `755` means:

```text
owner  = rwx
group  = r-x
others = r-x
```

## Why is ping implemented using fork and exec?

The requirement is to demonstrate process creation and program execution without using `system()`. The parent creates a child with `fork()`, and the child becomes the `ping` program using `execlp()`.

## Why use getaddrinfo()?

It performs hostname/address resolution in a standard protocol-independent way and can return both IPv4 and IPv6 addresses.

## What is a thread?

A thread is a unit of execution inside a process. Multiple threads allow one process to perform multiple tasks concurrently.

## What do threads share?

Threads in the same process share the process's code, global variables, heap memory and open files. Each thread has its own stack and CPU execution state.

## Why use a mutex?

Without the mutex, several threads may write to the terminal at the same time and their messages could become mixed together. The mutex allows only one thread at a time into the protected output section.

## Why is join() required?

`join()` waits for the thread to finish. It ensures the program does not continue or destroy the `std::thread` object while the worker is still running.

---

# 22. The Most Important Parts to Memorize

You do **not** need to memorize every line of C++.

Understand these relationships:

```text
System info   -> uname()
Memory        -> /proc/meminfo
Disk          -> statvfs()
Processes     -> /proc/<PID>/status
Create child  -> fork()
Child status  -> waitpid()
Signals       -> kill()
File info     -> stat/lstat()
Permissions   -> chmod()
Interfaces    -> getifaddrs()
Ping          -> fork() + exec() + waitpid()
DNS           -> getaddrinfo()
Threads       -> std::thread + join()
```

If you can explain **what each API gives us, why we use it, and what error we check**, you understand the project.

---

# 23. Five-Minute Explanation of the Entire Project

If someone asks you to explain the project from beginning to end, you can say:

> The project is a terminal-based Linux System Manager written in C++17 using OOP. I divided the program into five classes: SystemInfo, ProcessManager, FileManager, NetworkManager and ThreadDemo. `main.cpp` only handles the menu and calls the correct class.
>
> SystemInfo uses `uname()` for OS and kernel information, reads `/proc/meminfo` for RAM, and uses `statvfs()` for disk statistics. ProcessManager reads `/proc` to list processes and `/proc/PID/status` to get PID information. It also demonstrates `fork()`, child exit status through `waitpid()`, and signals through `kill()`.
>
> FileManager uses `stat`/`lstat` to inspect files and `chmod()` to modify permissions. NetworkManager uses `getifaddrs()` for local interfaces, `fork()` plus `execlp()` to run ping without `system()`, and `getaddrinfo()` for DNS lookup. ThreadDemo starts three `std::thread` objects, makes them work concurrently, protects terminal output with a mutex, and joins them before returning.
>
> The program checks Linux API return values and validates user input so ordinary errors do not crash the application.

That summary covers almost everything your instructor is likely to ask at the start of a discussion.
