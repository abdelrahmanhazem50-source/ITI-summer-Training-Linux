# Linux System Manager - Test Cases

These tests use normal user files and a dedicated signal test process. They do not require killing random system processes.

| Test | Steps | Expected Result |
|---|---|---|
| System information | Run option `1` | OS, hostname, kernel and architecture are displayed |
| Memory information | Run option `2` | Total and available memory are displayed |
| Disk information | Run option `3` | Total, used and free space for `/` are displayed |
| List processes | Run option `4` | PID, PPID, name and process state are shown |
| Process details | Run option `5`, enter the manager PID or another valid PID | PID, PPID, name, state, threads and memory are shown |
| Invalid PID | Run option `5`, enter `abc` or a missing PID | Friendly error message is shown |
| Child process | Run option `6` | Parent and child PIDs are shown and parent prints exit status `42` |
| Signal test | Run `./signal_test` in another terminal, use its PID in option `7` | SIGSTOP, SIGCONT, SIGTERM or SIGKILL can be sent safely |
| File information | Run option `8`, enter `test_file.txt` | Size, type, UID, GID and permissions are shown |
| Change permission | Run option `9`, enter `test_file.txt`, then `600` | Permissions change successfully |
| Invalid permission | Run option `9`, enter `999` | Input is rejected |
| Network information | Run option `10` | IPv4 interfaces and UP/DOWN state are shown |
| Ping | Run option `11`, enter `127.0.0.1` | Three ping packets are executed through `fork()` + `exec()` |
| DNS lookup | Run option `12`, enter `localhost` or an online hostname | Numeric IPv4/IPv6 addresses are shown |
| Threads | Run option `13` | Three threads start, work, finish, then join |
| Invalid menu input | Enter `hello`, `99`, or blank input | Application shows an error and continues |
