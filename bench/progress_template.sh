#!/bin/bash
n=10000
for i in $(seq 0 $n)
do
    if [ 0 -eq $(expr $i % $(expr $n / 100)) ]
    then
        echo $(expr $(expr 100 \* $i) / $n)/100%
    fi
done