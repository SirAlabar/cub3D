#!/bin/bash

# Function to run with valgrind
run_valgrind() {
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --verbose \
             --log-file=valgrind-out.txt \
             ./cub3d "$@"
}

# Function to run with Address Sanitizer
run_asan() {
    make clean
    make CFLAGS="-fsanitize=address -g"
    ./cub3d "$@"
}

# Main execution
case "$1" in
    "valgrind")
        shift
        run_valgrind "$@"
        ;;
    "asan")
        shift
        run_asan "$@"
        ;;
    *)
        exec "$@"
        ;;
esac
