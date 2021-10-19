FOLDER="../in/"
OUTFOLDER="../out"

# TRANSLATION TABLE between sequential ID and electronic channel
# it accounts for 4 sectors of 488 PMTs (1952 pixels) and 244 trigger channels
# gran total is 2196 channels =488 x 4 pixels + 244 trigger cells
echo 'TDAQ tables:'
IN=$FOLDER/seqID_roID.txt
OUT1=$OUTFOLDER/channels.txt
OUT2=$OUTFOLDER/channels_elesorted.txt
OUT3=$OUTFOLDER/channels_vector.txt
OUT4=$OUTFOLDER/channels_human.txt

TMP=tmp.txt
awk -F '_'  '{ printf "%s %s\n", $2,$4  }' $IN | awk '{ printf "%4d %4d\n",$1,$2}'  > $TMP
echo '   0  487' >> $TMP # SeqID 0 must be added by hand
sort -n -k 1 $TMP > $OUT1
echo $OUT1

sort -n -k 2 $TMP > $OUT2
echo $OUT2
rm $TMP

# prepare a vector for easy C import
# read second column and reshape as a vector
awk '{print $2}' $OUT1 > $TMP
paste -sd, $TMP > $OUT3
echo $OUT3
rm $TMP

# prepare human readble version of OUT
awk '{ printf "SEQ  %4d READOUT  %4d\n",$1,$2}' $OUT1 > $OUT4
echo $OUT4



# NEXT TABLE is about 1952 pixels
# OR_ID Column is meaningless (always zero) thus it is suppressed
IN=$FOLDER/mapPositionToSeqID_onlyPM_EmptyLinesRemoved.txt
#IN=$FOLDER/mapPositionToSeqID_onlyPM.txt
OUT5=$OUTFOLDER/readout.txt
OUT6=$OUTFOLDER/readout_human.txt

awk -F 'Disk' '{$1=$1}1' OFS='\nDisk' $IN > 0
sed 's/DiskID://g' 0 > 1
sed 's/UpDwID://g' 1 | sed 's/OrID://g' | sed 's/SCID://g' | sed 's/PMID://g' | sed 's/SeqID://g' > 2
grep -v "^$" 2 > $TMP
awk '{printf "%4d %4d %4d %4d %4d\n", $1,$2,$4,$5,$6}' $TMP > $OUT5
echo $OUT5
rm ?

# human readble version
awk '{ printf "FLANGE %4d PORTION %4d CELL %4d PMT %4d SEQ %4d\n",$1,$2,$3,$4,$5}' $OUT5 > $OUT6
echo $OUT6




# NEXT TABLE is about 244 trigger channels
IN=$FOLDER/mapPositionToSeqID_onlySC_EmptyLinesRemoved.txt
#IN=$FOLDER/mapPositionToSeqID_onlySC.txt
OUT7=$OUTFOLDER/trigger.txt
OUT8=$OUTFOLDER/trigger_human.txt
awk -F 'Disk' '{$1=$1}1' OFS='\nDisk' $IN > 0
sed 's/DiskID://g' 0 > 1
sed 's/UpDwID://g' 1 | sed 's/SCID://g' | sed 's/SeqID://g' > 2
grep -v "^$" 2 > $OUT7
rm ?
echo $OUT7
awk '{ printf "FLANGE %4d PORTION %4d CELL %4d TRIG %4d\n",$1,$2,$3,$4}' $OUT7 > $OUT8
echo $OUT8

