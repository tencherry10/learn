#!/bin/bash

mkdir -p build
gcc -Wall -Werror -Wno-unused-variable -ldl main.c -o ./build/main && ./build/main
