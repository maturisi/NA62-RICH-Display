

FOLDER="./original/"

# READ OUT CHANNEL TO SEQUENTIAL
IN=$FOLDER/seqID_roID.txt
OUT="map0.txt"
awk -F '_'  '{ printf "%s %s\n", $2,$4  }' $IN | awk '{ printf "%4d %4d\n",$1,$2}'  > $OUT
sort -k 2 $OUT > map0_EleSorted.txt


# SEQUENTIAL TO PMT AND SUPERCELL
IN=$FOLDER/mapPositionToSeqID_onlyPM_EmptyLinesRemoved.txt
#IN=$FOLDER/mapPositionToSeqID_onlyPM.txt
OUT="map1.txt"
awk -F 'Disk' '{$1=$1}1' OFS='\nDisk' $IN > 0
sed 's/DiskID://g' 0 > 1
sed 's/UpDwID://g' 1 | sed 's/OrID://g' | sed 's/SCID://g' | sed 's/PMID://g' | sed 's/SeqID://g' > 2
grep -v "^$" 2 > $OUT
rm ?


# prepare a vector for easy C import
# read second column and reshape as a vector
awk '{print $2}' map0.txt > 0
paste -sd, 0 > map1_Seq2Ro_Reshape.txt


# SUPERCELLS
IN=$FOLDER/mapPositionToSeqID_onlySC_EmptyLinesRemoved.txt
#IN=$FOLDER/mapPositionToSeqID_onlySC.txt
OUT="map2.txt"
awk -F 'Disk' '{$1=$1}1' OFS='\nDisk' $IN > 0
sed 's/DiskID://g' 0 > 1
sed 's/UpDwID://g' 1 | sed 's/SCID://g' | sed 's/SeqID://g' > 2
grep -v "^$" 2 > $OUT
rm ?
