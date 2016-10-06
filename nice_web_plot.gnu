#!/usr/bin/gnuplot
#
# Creates a version of a plot, which looks nice for inclusion on web pages
#
# AUTHOR: Hagen Wierstorf

reset

set terminal pngcairo size 800,600 enhanced font 'Verdana,9'
set output 'accel.png'

# define axis
# remove border on top and right and set color to gray
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set tics nomirror
# define grid
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12

# color definitions
set style line 1 lc rgb '#8b1a0e' pt 1 ps 1 lt 1 lw 2 # --- red
set style line 2 lc rgb '#5e9c36' pt 6 ps 1 lt 1 lw 2 # --- green
set style line 3 lc rgb '#65393d' pt 3 ps 1 lt 1 lw 2 # --- brown
set style line 4 lc rgb '#3db7c2' pt 4 ps 1 lt 1 lw 2 # --- blue
set style line 5 lc rgb '#f9c386' pt 5 ps 1 lt 1 lw 2 # --- blue
set style line 6 lc rgb '#98cdc5' pt 6 ps 1 lt 1 lw 2 # --- grey-cyan-thing

set key top left

set xlabel 'Threads'
set ylabel 'Aceleracao'

plot 'time_omp_static'  u 1:3 t 'openmp\_static'   w lp ls 1, \
     'time_omp_guided'  u 1:3 t 'openmp\_guided'   w lp ls 2, \
     'time_omp_dynamic' u 1:3 t 'openmp\_dynamic'  w lp ls 3, \
     'time_threads'     u 1:3 t 'threads'          w lp ls 4
