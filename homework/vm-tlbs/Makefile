tlb: tlb.c
	clang -Wall -o tlb tlb.c

plot.png: data.txt
	gnuplot -e "set terminal png size 640,480; set output 'plot.png'; set logscale x 2; plot 'data.txt' with lines"

data.txt:
	./tlb.sh > data.txt
