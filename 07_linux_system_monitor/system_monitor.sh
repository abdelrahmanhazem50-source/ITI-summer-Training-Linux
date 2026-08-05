#!/bin/bash

show_date() {
    current_date=$(date)
    echo "$current_date"
}

show_uptime() {
    uptime_information=$(uptime)
    echo "$uptime_information"
}

show_logged_users() {
    logged_users=$(who)
    echo "$logged_users"
}

show_current_directory() {
    current_directory=$(pwd)
    echo "$current_directory"
}

show_disk_usage() {
    disk_usage=$(df -h)
    echo "$disk_usage"
}

show_memory_usage() {
    memory_usage=$(free -h)
    echo "$memory_usage"
}

list_files() {
    files=$(ls -la)
    echo "$files"
}

while true
do
    echo "====================="
    echo "Linux Toolkit"
    echo "====================="
    echo "1) Show Date"
    echo "2) Show Uptime"
    echo "3) Show Logged Users"
    echo "4) Show Current Directory"
    echo "5) Show Disk Usage"
    echo "6) Show Memory Usage"
    echo "7) List Files"
    echo "8) Exit"

    read -r -p "Enter your choice: " choice
    echo

    case "$choice" in
        1) show_date ;;
        2) show_uptime ;;
        3) show_logged_users ;;
        4) show_current_directory ;;
        5) show_disk_usage ;;
        6) show_memory_usage ;;
        7) list_files ;;
        8)
            echo "Exiting..."
            break
            ;;
        *) echo "Invalid choice." ;;
    esac

    echo
    read -r -p "Press Enter to return to the menu..." unused
    echo
done
