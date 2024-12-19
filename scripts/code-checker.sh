#!/usr/bin/bash

# Requires:
#  - CodeChecker (https://pypi.org/project/codechecker/)

# Check code:
# ./scripts/code-checker.sh

set -e

rm -rf .code-checker
mkdir .code-checker

CodeChecker analyze ./build/compile_commands.json \
    --analyzers clangsa clang-tidy cppcheck \
    --analyzer-config 'clang-tidy:take-config-from-directory=true' \
    --output .code-checker/reports

CodeChecker parse .code-checker/reports \
    --export html \
    --output .code-checker/reports-html
