#!/bin/bash

line='export COMPANY=ITI'

if grep -Fxq "$line" "$HOME/.bashrc" 2>/dev/null; then
    echo "COMPANY is already configured in ~/.bashrc"
else
    echo >> "$HOME/.bashrc"
    echo "# ITI persistent environment variable" >> "$HOME/.bashrc"
    echo "$line" >> "$HOME/.bashrc"
    echo "COMPANY=ITI was added to ~/.bashrc"
fi

echo "Run: source ~/.bashrc"
