# Linux System Manager

A terminal-based C++ project that reads and controls basic parts of a Linux system. The project is intentionally kept simple: each topic has its own class, and the program uses Linux APIs instead of calling shell commands for the main work.

## Features

### System
- Operating system, hostname, kernel version and machine architecture using `uname()`
- Total and available memory from `/proc/meminfo`
- Total, used and free disk space using `statvfs()`

### Processes
- Lists running processes from `/proc`
- Shows PID, PPID, name and process state
- Shows detailed process information including threads and memory
- Creates a child process with `fork()`
- Parent waits using `waitpid()` and reads exit status `42`
- Sends `SIGTERM`, `SIGSTOP`, `SIGCONT` and `SIGKILL` using `kill()`

### Files
- Reads size, type, owner UID, group GID and permissions using `stat()` / `lstat()`
- Changes permissions with `chmod()`

### Network
- Lists IPv4 interfaces with `getifaddrs()` and `inet_ntop()`
- Runs `ping -c 3` through `fork()` + `execlp()` + `waitpid()`
- Resolves hostnames with `getaddrinfo()` and `getnameinfo()`

### Threads
- Starts three `std::thread` objects
- Each thread performs a small timed job
- Uses `join()` so the main program waits for every thread to finish

## Project Structure

```text
LinuxSystemManager/
├── main.cpp
├── SystemInfo.h
├── SystemInfo.cpp
├── ProcessManager.h
├── ProcessManager.cpp
├── FileManager.h
├── FileManager.cpp
├── NetworkManager.h
├── NetworkManager.cpp
├── ThreadDemo.h
├── ThreadDemo.cpp
├── signal_test.cpp
├── Makefile
├── test_file.txt
├── ARCHITECTURE.md
├── architecture.png
├── TEST_CASES.md
├── screenshots/
└── README.md
```

## Architecture

`main.cpp` only handles the menu and calls class functions.

- `SystemInfo`: operating system, memory and disk
- `ProcessManager`: `/proc`, process details, `fork()`, `waitpid()` and signals
- `FileManager`: file metadata and permissions
- `NetworkManager`: interfaces, ping and DNS
- `ThreadDemo`: three-thread example

See `ARCHITECTURE.md` and `architecture.png` for the diagram.

## Important Linux APIs Used

### `uname()`
Reads basic information about the running Linux system and kernel.

### `/proc`
Linux exposes process information through this virtual filesystem. The program reads files such as `/proc/<PID>/status` instead of using `ps`.

### `fork()`
Creates a new child process. It returns `0` inside the child and the child's PID inside the parent.

### `waitpid()`
Makes the parent wait for a child process and allows the parent to read its exit status.

### `kill()`
Sends a Linux signal to a process.

### `stat()` / `lstat()`
Reads file metadata such as size, owner and permission bits.

### `chmod()`
Changes file permissions using octal values such as `755`, `644` and `700`.

### `statvfs()`
Reads filesystem size and free-space information.

### `getifaddrs()`
Reads network interface information.

### `getaddrinfo()` / `getnameinfo()`
Resolve hostnames to numeric IP addresses.

### `std::thread`
Creates threads inside the same process. Threads share the process memory, but each thread has its own stack and execution state.

### `join()`
Waits for a thread to finish before continuing. Without joining or detaching a joinable thread, the C++ program can terminate unexpectedly when the thread object is destroyed.

## Ubuntu Environment Used for the Submission

The screenshots and terminal examples are formatted to match the target Ubuntu machine:

```text
geddo@Ubuntu:~/LinuxSystemManager$
```

The terminal uses the normal Ubuntu/GNOME appearance with an aubergine background and standard Linux window controls. The username is only part of the documentation/screenshots; nothing in the C++ code is hardcoded to `geddo`, so the program still works normally on other Linux users and machines.

## Build

Open a terminal in the project folder:

```bash
make
```

This creates:

```text
./linux_system_manager
./signal_test
```

To clean compiled files:

```bash
make clean
```

## Run

```bash
./linux_system_manager
```

## Safe Signal Test

Do not experiment on random system processes.

Open a second terminal and run:

```bash
./signal_test
```

It prints its own PID. Use that PID with menu option `7`.

Suggested order:

1. Send `SIGSTOP`
2. Check the process state from option `5`
3. Send `SIGCONT`
4. Send `SIGTERM` when finished

`SIGKILL` can also be tested, but the process will stop immediately because SIGKILL cannot be handled by the program.

## File Permission Test

A safe file is included:

```text
test_file.txt
```

Try option `8`, then option `9` with values such as:

```text
644
600
755
```

You can also verify the result in another terminal with:

```bash
ls -l test_file.txt
```

## Testing Notes

The project was built with C++17 and `-Wall -Wextra -pedantic -pthread`. The main features were run on Linux after compilation.

Detailed manual test cases are in `TEST_CASES.md`.

## Screenshots

The `screenshots` folder contains example runs for:

- main menu and system information
- processes and child process
- file information and permissions
- network information and DNS
- multi-threading

## Problems and Solutions

### Reading process information
At first it is tempting to run `ps`, but the project requires Linux interfaces directly. The solution was to scan numeric folders inside `/proc` and read each `status` file.

### Ping without `system()`
The project does not allow `system("ping ...")`. The solution is to create a child with `fork()`, replace it with the `ping` program using `execlp()`, and wait in the parent with `waitpid()`.

### Process signal testing
Sending signals to unknown processes is risky. A small `signal_test` program is included so signal features can be demonstrated safely.

### Thread output mixing together
Three threads can print at the same time, which makes terminal output messy. A small `std::mutex` protects each group of output lines while the threads still run concurrently.

## Process States Used

- `R` - Running or runnable
- `S` - Interruptible sleep
- `D` - Uninterruptible sleep
- `Z` - Zombie
- `T` / `t` - Stopped or traced
- `I` - Idle kernel thread

## Process vs Thread

A process has its own virtual address space and system resources. A thread is a smaller execution unit inside a process. Threads in the same process share code, global variables, heap memory and open files, while each thread keeps its own stack, CPU registers and program counter.
