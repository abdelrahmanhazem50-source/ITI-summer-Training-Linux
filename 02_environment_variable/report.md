# Environment Variable Report

The persistent variable is:

```bash
export COMPANY=ITI
```

It is added to `~/.bashrc` because Ubuntu terminal windows normally start an interactive Bash shell, and Bash reads this file when the terminal opens.

Verification:

```bash
source ~/.bashrc
echo $COMPANY
bash -c 'echo $COMPANY'
```

After opening a new terminal:

```bash
echo $COMPANY
```

The expected output is `ITI` in the current shell, child shell, and new terminal.
