#!/bin/bash

script_path="$(cd "$(dirname "$0")" && pwd)/welcome.sh"
line="source \"$script_path\""

if grep -Fxq "$line" "$HOME/.bashrc" 2>/dev/null; then
    echo "The welcome screen is already installed."
else
    echo >> "$HOME/.bashrc"
    echo "# ITI welcome screen" >> "$HOME/.bashrc"
    echo "$line" >> "$HOME/.bashrc"
    echo "The welcome screen was added to ~/.bashrc"
fi

echo "Open a new terminal to test it."
