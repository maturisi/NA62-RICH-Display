#!/bin/bash

echo 'Import partial maps in current directory'
cp ./generate/serial/out/map.txt          pmt.txt
cp ./generate/serial/out/serial_table.txt serial_table.txt
cp ./generate/channel/out/channels.txt    channels.txt
cp ./generate/channel/out/readout.txt     readout.txt
cp ./generate/channel/out/trigger.txt     trigger.txt

#cp ./generate/serial/out/serial_table_human.txt serial_table_human.txt
#cp ./generate/channel/out/channels_human.txt  channels_human.txt
#cp ./generate/channel/out/readout_human.txt   readout_human.txt
#cp ./generate/channel/out/trigger_human.txt   trigger_human.txt


echo 'Calculate Absolute Cell ID  [1..244]'
awk '{ abs_cell = 1 + $1 * 122 + $2 * 61 + $3 ; print abs_cell,$4,$5 }' readout.txt > readout_absolute_cell.txt
awk '{ abs_cell = 1 + $1 * 122 + $2 * 61 + $3 ; print abs_cell,$4 }' trigger.txt > trigger_absolute_cell.txt
#awk '{ abs_cell = 1 + $1 * 122 + $2 * 61 + $3 ; printf "ABSCELL %4d PMT %4d SEQ %4d\n",abs_cell,$4,$5}' readout.txt > readout_absolute_cell_human.txt
#awk '{ abs_cell = 1 + $1 * 122 + $2 * 61 + $3 ; printf "ABSCELL %4d TRIG %4d\n", abs_cell,$4 }' trigger.txt > trigger_absolute_cell_human.txt


echo 'Adding SeqID'
#i.e. for each entry in serial_table add SeqID (by matching ABS_CELL and PMT in readout.txt) > table1'

TABLE1="table1.txt"
while read -r pixel serial abs_cell pmt; do
  printf "%4d %s %4d %4d " $pixel $serial $abs_cell $pmt
  awk -v x="$abs_cell" -v y="$pmt" '$1==x && $2==y {printf "%4d\n",$3}' readout_absolute_cell.txt
done < serial_table.txt > $TABLE1

# human version of table1.txt
TABLE1H="table1_human.txt"
awk '{printf "PIXEL %4d SERIAL %s CELL %4d PMT %4d SEQ %4d\n",$1,$2,$3,$4,$5}' $TABLE1 > $TABLE1H


echo 'Adding EleID'
# 4. for each entry in table1 add EleID (by matching SeqID in channel.txt) > table 2
TABLE2="table2.txt"
while read -r pixel serial cell pmt seq; do
  printf "%4d %s %4d %4d %4d " $pixel $serial $cell $pmt $seq
  awk -v x="$seq" '$1==x {printf "%4d\n",$2}' channels.txt
done < $TABLE1 > $TABLE2

# human version of table2.txt
TABLE2H="table2_human.txt"
awk '{printf "PIXEL %4d SERIAL %s CELL %4d PMT %4d SEQ %4d ELE %4d\n",$1,$2,$3,$4,$5,$6}' $TABLE2 > $TABLE2H



echo 'Adding TrigID'
# 5. for each entry in trigger.txt add TrigID to table 2 (by matching ABS_CELL in trigger.txt) > table 3
TABLE3="table3.txt"
rm -f $TABLE3
while read -r cell trigch; do
    awk -v x="$cell" -v trig="$trigch" '$3==x { printf "%4d %s %4d %4d %4d %4d %4d\n", $1,$2,$3,$4,$5,$6,trig}' $TABLE2 >> $TABLE3
done < trigger_absolute_cell.txt

# human version of table3.txt
TABLE3H="table3_human.txt"
awk '{printf "PIXEL %4d SERIAL %s CELL %4d PMT %4d SEQ %4d ELE %4d TRIG %4d\n",$1,$2,$3,$4,$5,$6,$7}' $TABLE3 > $TABLE3H


echo 'Adding TrigEleID'
# 6. for each entry in table3 add TrigEleID (by matching SeqId in channel.txt) > table 4
TABLE4="table4.txt"
while read -r pixel serial cell pmt seq ele trig; do
  printf "%4d %s %4d %4d %4d %4d %4d " $pixel $serial $cell $pmt $seq $ele $trig
  awk -v x="$trig" '$1==x {printf "%4d\n",$2}' channels.txt
done < $TABLE3 > $TABLE4

sort -n $TABLE4


# human version of table4.txt
TABLE4H="table4_human.txt"
awk '{printf "PIXEL %4d SERIAL %s CELL %4d PMT %4d SEQ %4d ELE %4d TRIG %4d TRIGELE %4d\n",$1,$2,$3,$4,$5,$6,$7,$8}' $TABLE4 > $TABLE4H


FINALNAME=translation_table

cp $TABLE4  $FINALNAME.map
cp $TABLE4H $FINALNAME.human.map

rm *.txt

mv $FINALNAME.map $FINALNAME.txt
mv $FINALNAME.human.map translation_table_human.txt


echo 'RICH map ready in ' $FINALNAME.txt
echo 'Done'


exit
#EOF


