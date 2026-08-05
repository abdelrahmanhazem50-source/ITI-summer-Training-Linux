print_content() {
    ls -la
}

go_to() {
    cd "$1"
}

search() {
    grep --color=always -n "$1" "$2"
}

show_file() {
    nl -ba "$1"
}

remove() {
    rm -i "$1"
}

make_dir() {
    mkdir -p "$1"
}

create_file() {
    touch "$1"
}
