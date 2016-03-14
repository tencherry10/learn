#!/bin/bash

gcc main.c -o main && ./main
gcc main.c lib_f.c -o main && ./main
gcc main.c ovrd_f.c -o main && ./main
gcc main.c lib_f.c ovrd_f.c -o main && ./main

rm -f ./main
