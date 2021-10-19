#!/bin/bash
# RICH photodetection array is composed by 1952 PMT devices
# organized in two flanges by 976 PMT, flanges are called JURA and SALEVE
# each flange is organized in two sector UP and DOWN

# This script produces a map to uniquely identify the PMT in the array
# for each PMT the map specifyies:
# ABS - an absolute identifier, from 1 to 1952
# ID  - a local identifier from 1 to 976 for each flange  JURA and SALEVE
# SERIAL - the unique identifier by the manufacturer (Hamamatsu)
# SECTOR - an identifier of the sector (1=JURA UP, 2=JURA DOWN, 3 SALEVE UP, 4 SALEVE DOWN)
# CELL - which identify the ASIC that readout the PMT
# NAME - to identify the PMT insiede a CELL, name is [Left/Right][0..3]
# Please note that PMT name corresponds to HV channels

# INFN, Florence
# matteo.turisini@gmail.com
# 2019 October 7th

F0='abs.txt' # file with progressive number from 1 to 1972
rm -f $F0
for((i=1;i<=1952;i++)); do printf "ABS %04d\n" "$i" >> $F0; done
#---------------------------------------------------------------------

F1='id.txt' # file with progressive number from 1 to 976
rm -f $F1
for((i=1;i<=976;i++)); do printf "ID %02d\n" "$i" >> $F1; done
#-----------------------------------------------------

F20='flange0.txt'
rm -f $F20
for((i=1;i<=976;i++)); do printf "FLANGE 0\n" >> $F20; done

F21='flange1.txt'
rm -f $F21
for((i=1;i<=976;i++)); do printf "FLANGE 1\n" >> $F21; done



#--------------------------------------------------------
#F4='serial_fake.txt' # file with fake PMT serial number
# please note that the serial.txt must be edited by hand by human
#rm -f $F4
#for((i=1;i<=976;i++)); do printf "SERIAL HBXXXX\n" >> $F4; done

#F13='id_edit_serial.txt'
#paste $F1 $F4 > $F13

#F14='serial_jura.txt'
#F15='serial_saleve.txt'
#cp $F13 $F14
#cp $F13 $F15

#------------------------
F5='nameSALEVE.txt'
rm -f $F5

F6='nameJURA.txt'
rm -f $F6


# prepare supercell list as they were mounted
F7='sector0.txt' # JUP
echo 00 > $F7
for((i=1;i<=60;i++)); do printf "%02d\n" "$i" >> $F7; done

F8='sector1.txt' # JDOWN
cp $F7 $F8


F2='sector2.txt' # SUP
echo 00 > $F2
for((i=60;i>=1;i--)); do printf "%02d\n" "$i" >> $F2; done

F3='sector3.txt' # SDOWN
echo 37 > $F3
for((i=36;i>=0;i--)); do printf "%02d\n" "$i" >> $F3; done
for((i=60;i>=38;i--)); do printf "%02d\n" "$i" >> $F3; done


# generate names for SALEVE
while read sc; do
  for((i=3;i>=0;i--)); do printf "SECTOR 2 CELL %02s R %d\n" "$sc" "$i" >> $F5; done
  for((i=3;i>=0;i--)); do printf "SECTOR 2 CELL %02s L %d\n" "$sc" "$i" >> $F5; done

#  for((i=3;i>=0;i--)); do printf "S%02sR%d\n" "$sc" "$i" >> $F5; done
#  for((i=3;i>=0;i--)); do printf "S%02sL%d\n" "$sc" "$i" >> $F5; done

done < $F2

while read sc; do
  for((i=3;i>=0;i--)); do printf "SECTOR 3 CELL %02s R %d\n" "$sc" "$i" >> $F5; done
  for((i=3;i>=0;i--)); do printf "SECTOR 3 CELL %02s L %d\n" "$sc" "$i" >> $F5; done

#  for((i=3;i>=0;i--)); do printf "S%02sR%d\n" "$sc" "$i" >> $F5; done
#  for((i=3;i>=0;i--)); do printf "S%02sL%d\n" "$sc" "$i" >> $F5; done
done < $F3


# generate names for JURA # PLEASE NOTE OPPOSITE LEF-RIGH AND SEQUENTIAL 0 TO 3
while read sc; do
  for((i=0;i<=3;i++)); do printf "SECTOR 0 CELL %02s L %d\n" "$sc" "$i" >> $F6; done
  for((i=0;i<=3;i++)); do printf "SECTOR 0 CELL %02s R %d\n" "$sc" "$i" >> $F6; done

#  for((i=0;i<=3;i++)); do printf "J%02sL%d\n" "$sc" "$i" >> $F6; done
#  for((i=0;i<=3;i++)); do printf "J%02sR%d\n" "$sc" "$i" >> $F6; done
done < $F7

while read sc; do
  for((i=0;i<=3;i++)); do printf "SECTOR 1 CELL %02s L %d\n" "$sc" "$i" >> $F6; done
  for((i=0;i<=3;i++)); do printf "SECTOR 1 CELL %02s R %d\n" "$sc" "$i" >> $F6; done

#  for((i=0;i<=3;i++)); do printf "J%02sL%d\n" "$sc" "$i" >> $F6; done
#  for((i=0;i<=3;i++)); do printf "J%02sR%d\n" "$sc" "$i" >> $F6; done
done < $F8

#rm -f sector?.txt

F9='mapJURA.txt'
F10='mapSALEVE.txt'
F16='../in/serial_jura.txt'
F17='../in/serial_saleve.txt'
F18='serial_withoutID_jura.txt'
F19='serial_withoutID_saleve.txt'


awk '{printf $3" "$4 "\n"}' $F16 > $F18
awk '{printf $3" "$4 "\n"}' $F17 > $F19


paste $F20 $F1 $F18 $F6 > $F9
paste $F21 $F1 $F19 $F5 > $F10

F11='mapTOTAL.tmp'
cat $F9 $F10 > $F11


F12='map.txt'
paste $F0 $F11 > map.txt

column -t map.txt > map1.txt

cp map1.txt ../out/map.txt
echo 'Map Created in ../out/'$F12
