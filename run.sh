#!/bin/bash

dir=$(pwd)

#valgrind -q --track-fds=yes --leak-check=full --show-leak-kinds=all --trace-children=yes --show-reachable=yes --suppressions=./readline.supp ./minishell

# Use a comprehensive suppression file that includes readline and libc suppressions
SUPPRESSION_FILE="$dir/readline.supp"

# Run Valgrind with the updated suppression file
valgrind -q \
    --track-fds=yes \
    --leak-check=full \
    --show-leak-kinds=all \
    --trace-children=yes \
    --show-reachable=yes \
    --suppressions="$SUPPRESSION_FILE" \
    ./minishell
