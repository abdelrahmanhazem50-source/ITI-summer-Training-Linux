#!/bin/bash

report_file="detective_report.txt"
current_user=$(whoami)
login_shell=$(getent passwd "$current_user" | cut -d: -f7)
current_shell=$(ps -p $$ -o comm=)
environment_count=$(env | wc -l)

cat > "$report_file" <<REPORT
Linux Detective Report
======================

1. Current Shell
Command: ps -p \$\$ -o comm=
Output: $current_shell
Explanation: Shows the program running as the current shell process.

2. Shell PID
Command: echo \$\$
Output: $$
Explanation: \$\$ stores the process ID of the current shell.

3. Parent PID
Command: echo \$PPID
Output: $PPID
Explanation: PPID stores the process ID of the shell's parent process.

4. Current User
Command: whoami
Output: $current_user
Explanation: whoami prints the current username.

5. Current Working Directory
Command: pwd
Output: $(pwd)
Explanation: pwd prints the directory currently being used.

6. Home Directory
Command: echo \$HOME
Output: $HOME
Explanation: HOME stores the path of the current user's home directory.

7. Hostname
Command: hostname
Output: $(hostname)
Explanation: hostname prints the computer name.

8. Login Shell
Command: getent passwd \$USER | cut -d: -f7
Output: $login_shell
Explanation: The seventh field of the user account record contains the login shell.

9. PATH
Command: echo \$PATH
Output: $PATH
Explanation: PATH lists the directories searched for executable commands.

10. Number of Environment Variables
Command: env | wc -l
Output: $environment_count
Explanation: env lists exported variables and wc -l counts the lines.
REPORT

echo "Report created: $report_file"
