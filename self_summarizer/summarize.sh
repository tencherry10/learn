#!/bin/bash

cd $1
updir=`echo $1 | sed 's/^[^\/]*\///'` 
mkdir -p $2/$updir
find . -maxdepth 1 -type f -exec ls -Ao --time-style=+%s -- {} + | \
  sed 's|./||' | \
  awk '{print $6 "\t" $5}' | \
  egrep -v "^[[:space:]]*$|^#" \
  | tee $2/$updir/summary.txt

