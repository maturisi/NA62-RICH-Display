CONTENT:
generate a table with PMT Serial Number and Super Cell ID 
PMT serial number is the unique identifier used by manufacturer
Super Cell ID is an identifier used in the detector construction and operation 

AUTHOR:
matteo.turisini@gmail.com
INFN Florence
2019 October 7th


STRUCTURE:
folder 'out' contains the map
folder 'in' contains hand written files with serial number of the PMT
folder 'log' is the PMTs logbook and contains PMT notes e.g. replacements, missing labels
all other .txt files are intermediate to obtain the map

USAGE:
a bash script 'generate.sh' in folder 'work' is used to generate the map. 


PROCEDURE

1. edit manually the files in folder 'in' containing the PMT serial number for each PMT mounting ID
2. ./generate.sh 
3. check the map.txt (1952 lines with no duplicates)
4. ./process.sh  (since db is not online this must run on daq@pclhcp02.fi.infn.it)
