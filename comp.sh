#!/bin/bash

gcc mandel.c image_utils.c main_openmp.c -o main_omp -lpng -fopenmp
gcc mandel.c image_utils.c main.c -o main -lpng -fopenmp
