#!/usr/bin/bash

# Requires:
#  - meson (https://pypi.org/project/meson/)

# Check meson scripts format:
# ./scripts/meson-format

# Fix meson scripts format:
# ./scripts/meson-format -f

set -e

FIX_MODE=0

while getopts f opt
do
    case "${opt}" in
        f) FIX_MODE=1 ;;
        *) break ;;
    esac
done

MESON_FILES=$(find . -name "meson.build")

if [[ $FIX_MODE -eq 0 ]]; then
    if ! meson format -q -r; then
        echo "Error: meson build scripts are not formatted! Run '$0 -f'"
        exit 1
    fi
else
    meson format -i -r
fi
