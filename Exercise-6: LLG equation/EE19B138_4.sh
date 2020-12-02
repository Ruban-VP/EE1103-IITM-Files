#!/bin/bash

#Usage: Open your linux terminal and enter the command: bash EE19B138_4.sh
#Note: All the files will be created in the PWD. 

echo "" > plot2.txt
echo "" > plot3.txt
echo "" > plot4.txt        #Files created and initialised to store the data to be plotted 

gcc EE19B138_4.c -lm -o plot  #Executable of the .c file is created

#Note:
#In all the places, the stepsize is in the order of 1e-17 or 1e-18 because the derivative of Phi is in the order of 1e16. So choosing a stepsize 
# in the order of 1e-12 or 1e-13 leads to an increase of 1000 in Phi for every iteration which contradicts the meaning of the SMALL increment.
#Theoretically, stepsize must be infinitesimally small and all calculations are done with that limit operating. But since, it is not possible in a computer,
# I chose it as 1e-18 so the Phi increment would be in the order of 0.01 radians which is comparable to the range in which Phi is. 

./plot 0.5e-17 0.05 1 > plot1.txt    #Data generated for part 1 of the assignment

for i in {1..1000}
do
	var="${i}e-18"
	./plot $var 0.05 2 >> plot2.txt   #Data generated for part 2 of the assignment
done

for i in {10..300}
do
	b="${i}e-3"
	./plot 1e-18 $b 3 >> plot3.txt    #Data generated for part 3 of the assignmment
done

./plot 1e-18 0.01 4 > plot41.txt
./plot 1e-18 0.05 4 > plot42.txt
./plot 1e-18 0.1 4 > plot43.txt
./plot 1e-18 0.3 4 > plot44.txt       #Data generated for part 4 of the assignment
 
#The rest of the code below plots all the above generated data using the "gnuplot" software. This bash script basically automates the data generation
#and plotting processes.

gnuplot -persist <<-EOFMarker
set title 'Iteration number vs Error'
set xlabel 'Iteration number'
set ylabel 'Error'
set term wxt 0
plot 'plot1.txt' using 1:2 with lines title 'Mx error curve' 
set term wxt 1
plot 'plot1.txt' using 1:3 with lines title 'My error curve' 
set term wxt 2
plot 'plot1.txt' using 1:4 with lines title 'Mz error curve'
set term wxt 3
set title 'Iteration number vs error - All in one curve'
set xlabel 'Iteration number'
set ylabel 'Error'
plot 'plot1.txt' using 1:2 with lines title 'Mx error curve' linestyle 3, \
'plot1.txt' using 1:3 with lines title 'My error curve' linestyle 2, \
'plot1.txt' using 1:4 with lines title 'Mz error curve' linestyle 1
EOFMarker

awk '{$1 = $1/1e-18; $2 = $2*180/3.14; print $1"\t"$2}' plot2.txt > plot22.txt

gnuplot -persist <<-EOFMarker
set title 'Step size vs RMS error'
set xlabel 'Step size (in 1e-18 s)'
set ylabel 'RMS error (in degrees)'
plot 'plot22.txt' using 1:2 with lines title 'Stepsize vs RMS error'
EOFMarker

awk '{$2 = $2/1e-15; print $1"\t"$2}' plot3.txt > plot33.txt

gnuplot -persist <<-EOFMarker
set title 'Alpha vs Switching time'
set xlabel 'Alpha'
set ylabel 'Switching time (in Femtoseconds)'
plot 'plot33.txt' using 1:2 with lines title 'Alpha vs Switching time'
EOFMarker

for FILE in "plot41.txt" "plot42.txt" "plot43.txt" "plot44.txt"
do
	gnuplot -persist <<-EOFMarker
	set title 'Magnetisation vector precession plot'
	set xlabel 'Mx'
	set ylabel 'My'
	set zlabel 'Mz'
	splot '$FILE' u 2:3:4 with lines title 'Precession plot' 
	EOFMarker
done

awk '{$1 = $1/1e-15; print$1"\t"$2"\t"$3"\t"$4}' plot42.txt > plot42_temp.txt

gnuplot -persist <<-EOFMarker
set title 'Time vs Magnetisation vector components'
set xlabel "Time (in Femtoseconds)"
set ylabel "Magnetisation component"
plot 'plot42_temp.txt' using 1:2 with lines title 'Time vs Mx' linestyle 3, \
'plot42_temp.txt' using 1:3 with lines title 'Time vs My' linestyle 2, \
'plot42_temp.txt' using 1:4 with lines title 'Time vs Mz' linestyle 1
EOFMarker