#!/usr/bin/bash

# Requires:
#  - typos (https://github.com/crate-ci/typos)

# Check typos:
# ./scripts/typos

# Fix typos:
# ./scripts/typos -f

FIX_MODE=0

while getopts f opt
do
    case "${opt}" in
        f) FIX_MODE=1 ;;
        *) break ;;
    esac
done

if [[ $FIX_MODE -eq 0 ]]; then
    if ! typos; then
        echo "Error: source code has a some typos! Run '$0 -f'"
        exit 1
    fi
else
    typos -w
fi
