#!/bin/bash
INFILE="pedestal.txt"
OUTFILE1="ped_mean.txt"
OUTFILE2="ped_rms.txt"


awk ' {print $1" "$2}'    $INFILE > $OUTFILE1
awk ' {print $1" "$3}'    $INFILE > $OUTFILE2

exit
#eof
