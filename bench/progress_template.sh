#!/bin/bash
n=10000
for i in $(seq 0 $n)
do
    if [ 0 -eq $(expr $i % $(expr 10000 / 100)) ]
    then
        echo $(expr $(expr 100 \* $i) / 10000)/100%
    fi
done