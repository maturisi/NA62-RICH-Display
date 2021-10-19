#!/bin/bash

OUTFILE=data_efficiency.txt

SALE="./Saleve_eff.txt"
JURA="./Jura_eff.txt"

awk ' {$1+=0}1'   $JURA > disk1.tmp
awk ' {$1+=976}1' $SALE > disk2.tmp
cat disk1.tmp disk2.tmp > all.tmp

awk ' {print $1" "$4}' all.tmp  > $OUTFILE
rm *.tmp
exit
#eof
