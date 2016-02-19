#!/bin/bash

mkdir -p build
gcc -Wall -Werror -Wno-unused-variable -fpic foo.c -shared -o ./build/libfoo.so && strip -s ./build/libfoo.so
gcc -Wall -Werror -Wno-unused-variable -ldl main.c -o ./build/main && ./tmp/main ./build/libfoo.so
