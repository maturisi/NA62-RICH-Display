#!/bin/bash
# merge in a single file the files from the 4 TEL62 boards
# filename from the TEL62 boards are stored in the SECTORx variables
# please assign SECTORx the path to the data that have to be merged

OUTFILE=data_scaler.txt

SECTOR1="./190521_stability4/rich_j1_hv900_thr1500_iter99_2019_05_22_18_45_09_cnt.txt"
SECTOR2="./190521_stability4/rich_j2_hv900_thr1500_iter99_2019_05_22_18_45_56_cnt.txt"
SECTOR3="./190521_stability4/rich_s1_hv900_thr1500_iter99_2019_05_22_18_53_56_cnt.txt"
SECTOR4="./190521_stability4/rich_s2_hv900_thr1500_iter99_2019_05_22_20_20_25_cnt.txt"

awk ' {$1+=0}1'    $SECTOR1 > sector1.tmp
awk ' {$1+=512}1'  $SECTOR2 > sector2.tmp
awk ' {$1+=1024}1' $SECTOR3 > sector3.tmp
awk ' {$1+=1536}1' $SECTOR4 > sector4.tmp

cat sector?.tmp | sort -n  > $OUTFILE

lines=`wc -l $OUTFILE | awk '{print $1}'`;
if test $lines -eq 2048; then
  echo File $OUTFILE ready
else
  echo Warning: unexpected line number, expected 2048 you have $lines
fi

rm -f sector?.tmp
exit
#eof
