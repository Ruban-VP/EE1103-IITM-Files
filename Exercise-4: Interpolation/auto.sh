#!/bin/bash

awk '{print $1"\t"$2}' assignment_2.txt > newdata.txt
#The file path of the datafile must be free of any spaces
# All the other files created when this bash script is run will be created in the present working diretory.
# So we don't need to specify their paths in the ./a.out command or the gnuplot command.

echo "" > errorplot.txt
gcc EE19B138_interp.c         #The interpolation code is compiled
for i in {1..10}
do
	var=$(( 2 ** i))
	./a.out $var newdata.txt >> errorplot.txt   
	#The executable file is ran for different downsampling constants (in this case they are the powers of 2 between 2 and 1024)
done

#For better visualisation, only the first 500 datapoints are used for plotting 

# Original dataset
awk '{if(NR<502) print $1"\t"$2}' newdata.txt > original.txt
# Interpolation with g=2
awk '{if(NR<502 && (NR-2)%2==0) print $1"\t"$2}' newdata.txt > interpg2.txt
# Interpolation with g=16
awk '{if(NR<502 && (NR-2)%16==0) print $1"\t"$2}' newdata.txt > interpg16.txt
# Interpolation with g=128
awk '{if(NR<502 && (NR-2)%128==0) print $1"\t"$2}' newdata.txt > interpg128.txt

#The following commands are used to call the gnuplot commands within the bash script. The 'Error plot' and 'Original vs interpolation' plots for some downsampling
#constants are plotted using these commands
gnuplot -persist <<-EOFMarker
set title 'Error vs G'
set xlabel 'Downsampling constant(G)'
set ylabel 'Mean squared error(E)'
plot 'errorplot.txt' with lines title 'Error plot' linestyle 1
EOFMarker

for FILE in "interpg2.txt" "interpg16.txt" "interpg128.txt"
do
	gnuplot -persist <<-EOFMarker
	set title 'Original vs Interpolation'
	set xlabel 'Time(Input)'
	set ylabel 'Output'
	plot 'original.txt' with lines title 'Original' linestyle 1, \
	'$FILE' with lines title 'Interpolation' linestyle 2
	EOFMarker
done

