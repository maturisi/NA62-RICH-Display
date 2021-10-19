2019 nOV 26
riordinato i vari pezzi di codice sparsi ed efetuato backup su lxplus.

Per completare resta:
-sistemare in ordine il codice sviluppato per scaler,database e mappe

-fare il merge delle translation tables e dedicare loro una cartella
/home/cctdaq/Scrivania/rich/dbPMT/map
/home/cctdaq/Scrivania/rich/suite0/maps
magari razionalizzando un poco

-riprovare tutto il codice modificando i path ai dati e scrivendo istruzioni
in particolare collocazione dati scaler e collocazione mappe di traslazione

- rivedere il codice del display 2D per un uso piu immediato

--------------


2019 October 
matteo.turisini@gmail.com
INFN Firenze


PC1: daq@pclhcb02 (daqcedar)
PC2: cctdaq@static-2-192 (cctdaq)


DIARY
07 # Create RICH PMTs translation table (from construction paper logbook)
08 # Extract characterization data from database (from delivery and acceptance tests)
09 # Validate translation table (crosschecking with database) 
10 # Select population 
21 # software finalized and editing how to and slides
other:
installed PC2 with debian 10 and ROOT 6.19 (http://tylern4.github.io/InstallRoot/)


NOTE
# final version of the code with MySQL and selection in C code, new executable ./getPMTinfo <serial>
# analysis with CLI bash and root
# everything else moved to preliminary


PROCEDURE

1. edit manually the files in folder 'in' containing the PMT serial number for each PMT mounting ID
2. ./generate.sh 
3. check the map.txt (1952 lines with no duplicates)
4. ./analise.sh  (this include ./process.sh > rich.txt and since db is not online this must run on daq@pclhcp02.fi.infn.it)
