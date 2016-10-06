#!/bin/bash

gcc mandel.c image_utils.c main_openmp.c  -o main_omp_static  -lpng -fopenmp -D_SCHELL_=static
gcc mandel.c image_utils.c main_openmp.c  -o main_omp_dynamic -lpng -fopenmp -D_SCHELL_=dynamic
gcc mandel.c image_utils.c main_openmp.c  -o main_omp_guided  -lpng -fopenmp -D_SCHELL_=guided

gcc mandel.c image_utils.c main.c         -o main             -lpng

gcc mandel.c image_utils.c main_threads.c -o main_threads     -lpng -lpthread
