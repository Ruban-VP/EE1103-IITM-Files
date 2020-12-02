#!/bin/bash

grep -v '#' data_4_corr.txt | awk '{print $1"\t"$2"\t"$3}' > newdata.txt
# The file is filtered of any field names containing columns

gcc correlation.c -lm  # The executable file of the C code is created

./a.out newdata.txt 1 > temp1.txt      # Fourier data of 1st signal
./a.out newdata.txt 2 > temp2.txt      # Fourier data of 2nd signal
./a.out newdata.txt 3 > temp3.txt      # Correlation data

# The common frequency of the discrete signals are found and printed
awk ' BEGIN{big=0; domfreq=0;} {if($2>=big){big=$2; domfreq=$1;}} END{printf "The dominant frequency of the sampled first signal is:" domfreq"\n";}' temp1.txt
awk ' BEGIN{big=0; domfreq=0;} {if($2>=big){big=$2; domfreq=$1;}} END{printf "The dominant frequency of the sampled second signal is:" domfreq"\n";}' temp2.txt

# The DFT plot of 1st signal is plotted
gnuplot -persist <<-EOFMarker
set title 'Discrete fourier transform of 1st signal'
set xlabel 'Frequency (in Hz)'
set ylabel 'DFT Magnitude'
plot 'temp1.txt' with lines title 'DFT magnitude vs Frequency'
EOFMarker

# The DFT plot of 2nd signal is plotted
gnuplot -persist <<-EOFMarker
set title 'Discrete fourier transform of 2nd signal'
set xlabel 'Frequency (in Hz)'
set ylabel 'DFT Magnitude'
plot 'temp2.txt' with lines title 'DFT magnitude vs Frequency'
EOFMarker

# The correlation plot is plotted
gnuplot -persist <<-EOFMarker
set title 'Correlation data plot'
set xlabel 'Delay parameter'
set ylabel 'Correlation value'
plot 'temp3.txt' with lines title 'Correlation vs Delay parameter'
EOFMarker

