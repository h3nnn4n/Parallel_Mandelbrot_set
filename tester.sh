#!/bin/bash

/usr/bin/time -f '%e' -a -o time_main ./main

for j in main_threads main_omp
do
    for i in `seq 1 16`
    do
        for k in `seq 1 2`
        do
            /usr/bin/time -f '%e' -a -o time_${i}_${j} ./$j $i
        done
    done
done