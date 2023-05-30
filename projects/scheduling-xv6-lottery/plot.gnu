#!/usr/local/bin/gnuplot
set terminal png size 640,480
set output 'plot.png'
set xlabel "time"
set ylabel "time slices"
plot 'plot.txt' using 1:2 with lines title "10 tickets", \
     '' using 1:3 with lines title "20 tickets", \
     '' using 1:4 with lines title "30 tickets"
