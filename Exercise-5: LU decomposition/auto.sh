#!/bin/bash

echo "" > errorplot.txt 
echo "" > profiledata.txt
echo "" > profiledata2.txt
awk '{$1 = $1; print $1"\t"$2}' assignment_2.txt > original.txt
awk '{if(NR<1001){$1 = $1; print $1"\t"$2} }' assignment_2.txt > newdata.txt
#The file path of the datafile must be free of any spaces (In this case, the file was in the same working directory. So the full file path isn't needed.)
#Hence, when you run this bash script, please provide a space-free file path (or) just create a copy of the dataset in the PWD.
# All the other files created when this bash script is run, will be created in the present working diretory.
# So we don't need to specify their paths in the ./a.out command or the gnuplot command.
# Also, only the first 1000 rows from the original file is extracted for space constraints.

gcc LUD_Cubspline_Errorvsg.c -lm -o errorvsg #The "Error vs G" printing C code is compiled
gcc LUD_Cubspline_Inoutinterpol.c -lm -o inoutintpol #The "Input, Output and Interpolated output" printing C code is compiled

for i in {1..8}
do
	var=$(( 2 ** i))
	./errorvsg $var newdata.txt >> errorplot.txt   
	#The executable file is ran for different downsampling constants (in this case they are the powers of 2 between 2 and 256)
done 

./inoutintpol 2 newdata.txt > interpg2.txt
#Interolation with g=2
./inoutintpol 16 newdata.txt > interpg16.txt
#Interpolation with g=16
./inoutintpol 256 newdata.txt > interpg256.txt
#Interpolation with g=256

#The following commands are used to call the gnuplot commands within the bash script. The 'Error plot' and 'Original vs interpolation' plots for some downsampling
#constants are plotted using these commands
gnuplot -persist <<-EOFMarker
set title 'Error vs G'
set xlabel 'Downsampling constant(G)'
set ylabel 'Mean squared error(E)'
plot 'errorplot.txt' with lines title 'Error plot' linestyle 1
EOFMarker

for FILE in "interpg2.txt" "interpg16.txt" "interpg256.txt"
do
	gnuplot -persist <<-EOFMarker
	set title 'Original vs Interpolation'
	set xlabel 'Time(Input)'
	set ylabel 'Output'
	plot '$FILE' using 1:2 with lines title 'Original' linestyle 1, \
	'$FILE' using 1:3 with lines title 'Interpolation' linestyle 2
	EOFMarker
done

#The following commands are used to obtain the profile data.

#profiledata.txt  contains profiling data about the Cubic spline code.
#profiledata2.txt contains profiling data about the Cubic spline code.

gcc -Wall -std=c99 -pg LUD_Cubspline_Errorvsg.c -lm -o errorprofiling

./errorprofiling 90 original.txt 
gprof errorprofiling gmon.out >> profiledata.txt

./errorprofiling 200 original.txt 
gprof errorprofiling gmon.out >> profiledata.txt

./errorprofiling 2000 original.txt 
gprof errorprofiling gmon.out >> profiledata.txt

gcc -Wall -std=c99 -pg EE19B138_interp.c -lm -o errorprofiling

./errorprofiling 90 original.txt 
gprof errorprofiling gmon.out >> profiledata2.txt

./errorprofiling 200 original.txt 
gprof errorprofiling gmon.out >> profiledata2.txt

./errorprofiling 2000 original.txt 
gprof errorprofiling gmon.out >> profiledata2.txt

