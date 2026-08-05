#!/bin/bash

commands_file="$(cd "$(dirname "$0")" && pwd)/custom_commands.sh"
line="source \"$commands_file\""

if grep -Fxq "$line" "$HOME/.bashrc" 2>/dev/null; then
    echo "The custom commands are already installed."
else
    echo >> "$HOME/.bashrc"
    echo "# ITI custom terminal commands" >> "$HOME/.bashrc"
    echo "$line" >> "$HOME/.bashrc"
    echo "The custom commands were added to ~/.bashrc"
fi

echo "Run: source ~/.bashrc"
