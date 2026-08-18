# Requirement Checklist

This file maps the graduation project requirements to the implementation so it is easy to review during the demo.

| Requirement | Implementation |
|---|---|
| C++17 or newer | `Makefile` uses `-std=c++17` |
| OOP | Five required classes are separated into `.h` and `.cpp` files |
| System information | `SystemInfo::showSystemInfo()` uses `uname()` |
| Memory information | `SystemInfo::showMemoryInfo()` reads `/proc/meminfo` |
| Disk information | `SystemInfo::showDiskInfo()` uses `statvfs()` |
| List processes | `ProcessManager::listProcesses()` scans numeric `/proc` folders |
| Process details | `ProcessManager::showProcessInfo()` reads `/proc/<PID>/status` |
| Process states | R, S, D, Z, T/t and I are translated to readable names |
| Child process | `fork()` creates a child and prints parent/child PIDs |
| Child exit status | Child exits with `42`; parent reads it using `waitpid()` |
| Signals | `kill()` sends SIGTERM, SIGSTOP, SIGCONT and SIGKILL |
| Safe signal testing | `signal_test.cpp` provides a dedicated process |
| File information | `lstat()` reads size, type, UID, GID and permissions |
| Change permissions | `chmod()` accepts octal permission values |
| Network interfaces | `getifaddrs()` + `inet_ntop()` list IPv4 interfaces |
| Ping | `fork()` + `execlp()` + `waitpid()` run `ping -c 3`; no `system()` |
| DNS lookup | `getaddrinfo()` + `getnameinfo()` show numeric addresses |
| Three threads | `ThreadDemo` creates three `std::thread` objects |
| Thread sleep | `std::this_thread::sleep_for()` |
| Thread join | All three threads are joined |
| Error handling | Invalid PID, file, permission, menu input and network errors are handled |
| Makefile | Supports `make` and `make clean` |
| README | Includes overview, features, APIs, build/run, testing, screenshots and problems |
| Architecture diagram | `ARCHITECTURE.md` and `architecture.png` |
| Screenshots | `screenshots/` folder |
| Test cases | `TEST_CASES.md` |
| Final presentation | `Linux_System_Manager_Presentation.pptx` |
