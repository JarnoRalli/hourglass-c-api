#!/bin/bash

# Applies clang-format to all the .hpp, .h, .cpp and .c files. Skips the ./build directory.
find . \( -iname '*.hpp' -o -iname '*.h' -o -iname '*.cpp' -o -iname '*.c' \) -not -path "*/build/*" | xargs clang-format --verbose --style=file -i
