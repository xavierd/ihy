#!/usr/bin/perl

# Generate postscript and png plot with GNUplot
# (C) 2005 www.captain.at

# set custom font path
$ENV{GDFONTPATH} = "/usr/share/fonts/TTF/";

# GNUPLOT PNG
open (GNUPLOT, "|gnuplot");
print GNUPLOT <<EOPLOT;
set term png small xFFFFFF font "VeraMono" 8 size 1000,1000
set output "data.png"
set size 1 ,1
set nokey
set style data impulse
set xlabel "frequency [Hz]" font "VeraMono,10"
set xrange [0:22000]
set yrange [0:200]
set title "FFT" font "VeraMono,10"
set grid xtics ytics
set xtics 3000
plot "data.dat" using 1:2 w impulse 1
EOPLOT
close(GNUPLOT);

#set yrange [0:400000000]
#set yrange [0:500000000]
