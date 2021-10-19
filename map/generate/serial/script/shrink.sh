#!/bin/bash
IN=../out/map.txt


TMP=tmp.txt
# remove redunant information and calculate the ABD CELL ID
awk ' {abs = 1 + $10 * 61 + $12; printf "%d %s %d  %s%d\n",$2,$8,abs,$13,$14}' $IN > $TMP


OUT=../out/serial_table.txt

  sed 's/L0/7/g' $TMP \
| sed 's/L1/4/g' \
| sed 's/L2/2/g' \
| sed 's/L3/1/g' \
| sed 's/R0/6/g' \
| sed 's/R1/5/g' \
| sed 's/R2/3/g' \
| sed 's/R3/0/g' \
 > $OUT



tail $IN
#tail $TMP
tail $OUT

rm $TMP

# human readble versione
OUT2=../out/serial_table_human.txt
awk '{printf " ABS %4d SERIAL %s ABS_CELL %4d PMT %4d\n", $1,$2,$3,$4}' $OUT > $OUT2
