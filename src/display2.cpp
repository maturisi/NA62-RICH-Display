
#include "TNA62richPixel.h"

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <string>       /*std::string */

/*
 whatever including path
gain, klumi, efficiency, pedestal,...
2 columns x 1952 rows, formats distinguish between what's in the first column:
pixelID = format1,
sedID = format2.

format 3 is for 2048 row files and eleid as channel identifier
*/
int main(int argc, char * argv[]) {

  if(argc!=4){
    printf("Usage: %s <what> <filename> <format>\n",argv[0]);
    printf("      what      - gain | klumi | efficiency\n");
    printf("      filename - input file name\n");
    printf("      format   - 1 pixelID | 2 seqID \n");
    printf("*****************************************************\n");
    printf("%s plots generic data about the NA62-RICH detector\n",argv[0]);
    printf("Input file is 2 column x 1952 rows plain text file\n");
    printf("The first column can be SEQID or PIXID \n");
    printf("The second column is any PMT realated variable\n");
    printf("At the moment only gain, klumi and efficiency are implemented\n");
    printf("*****************************************************\n");
    printf("Example1: %s gain       ../data/datasheet/gain.txt 1\n",argv[0]);
    printf("Example2: %s klumi      ../data/datasheet/lumi_k.txt 1\n",argv[0]);
    printf("Example3: %s efficiency ../data/efficiency/data_efficiency.txt 2\n",argv[0]);
    printf("Example4: %s pedmean    ../data/pedestal/ped_mean.txt 3\n",argv[0]);
    printf("Example5: %s pedrms     ../data/pedestal/ped_rms.txt 3\n",argv[0]);
    printf("*****************************************************\n");
    return -1;
  }

  TNA62richPixel * display = new TNA62richPixel();
  display->SetFormat(atoi(argv[3]));
  display->SetVarName(argv[1]);
  int ret = display->ReadData(argv[2]);
  if(ret<0){ printf("File %s not found, exit\n",argv[2]);return -1;}
  //display->Dump();
  display->Analise(); // 1D e txt output
  display->Draw();  // 2D
  return 0;

}

