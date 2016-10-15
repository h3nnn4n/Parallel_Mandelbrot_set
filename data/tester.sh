#!/bin/bash

/usr/bin/time -f '%e' -a -o time_main ./main

for j in main_threads main_omp_static main_omp_dynamic main_omp_guided
do
    echo $j
    for i in `seq 1 16`
    do
        echo $i
        for k in `seq 1 10`
        do
            /usr/bin/time -f '%e' -a -o time_${i}_${j} ./$j $i
        done
    done
done
