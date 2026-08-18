# Linux System Manager - Architecture

The program is split into five small classes so that each Linux topic stays in its own file.

```text
                         main.cpp
                            |
            +---------------+---------------+
            |               |               |
            v               v               v
       SystemInfo      ProcessManager    FileManager
            |               |               |
            |               |               |
      uname()          /proc/<PID>        stat()
   /proc/meminfo         fork()           chmod()
      statvfs()         waitpid()
                         kill()
                            |
                  +---------+---------+
                  |                   |
                  v                   v
            NetworkManager        ThreadDemo
                  |                   |
             getifaddrs()         std::thread
             getaddrinfo()        sleep_for()
              fork/exec           join()
```

## Class Responsibilities

- `SystemInfo` reads operating system, memory and disk information.
- `ProcessManager` reads `/proc`, creates child processes, waits for them and sends signals.
- `FileManager` reads file metadata and changes permissions.
- `NetworkManager` lists IPv4 interfaces, runs `ping` with `fork()` + `exec()` and performs DNS lookup.
- `ThreadDemo` starts three threads and waits for them using `join()`.
- `main.cpp` only creates the objects, shows the menu and calls the correct class function.
