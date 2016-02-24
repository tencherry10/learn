#!/bin/bash 

find files/ -type d -exec ./summarize.sh {} `pwd`/summary \;
