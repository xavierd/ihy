#!/bin/bash

rm -f test.ihy
make
time ./ihyconvert -c test.wav test.ihy
s1=`/bin/ls -l test.ihy | cut -f 5 -d ' '`
s2=`/bin/ls -l test.wav | cut -f 5 -d ' '`
r=$((100-(100*$s1/$s2)))
echo "Compression : $r %"
./ihyconvert -r test.ihy