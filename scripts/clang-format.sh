#!/usr/bin/bash

# Requires:
#  - clang-format-15

# Check code format:
# ./scripts/clang-format

# Fix code format:
# ./scripts/clang-format -f

set -e

FIX_MODE=0

while getopts f opt
do
    case "${opt}" in
        f) FIX_MODE=1 ;;
        *) break ;;
    esac
done

SOURCES=$(find . -regex '.+\.[hc]pp' -o -name "version.hpp.in")

if [[ $FIX_MODE -eq 0 ]]; then
    if ! clang-format-15 --dry-run --Werror $SOURCES; then
        echo "Error: source code is not formatted! Run '$0 -f'"
        exit 1
    fi
else
   clang-format-15 -i $SOURCES
fi
