#include "TNA62richThrScan.h"

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <string>       /*std::string */

// -------------------------------------- //
int main(int argc, char *argv[]) {
// -------------------------------------- //

  if(argc!=4){
    printf("Usage: %s <type> <filename> <duration>\n",argv[0]);
    printf("       type - ped or dark (for hv ON or OFF)");
    printf("       filename - input filename");
    printf("       duration - scaler integration time during daq");
    printf("*****************************************************\n");
    printf("%s generates scatter a scatter plot for each channel with the counting rate as a function of the thresholds and write it in a .root file\n",argv[0]);
    printf("Input file is 3 column x (2048 x N)rows plain text file where N indicated the number of threshold points\n");
    printf("The  first column is ELEID (as produced by mbcounter )\n");
    printf("The second column is the threshold\n");
    printf("The third column is the number of counts in the elapsed time\n");
    printf("To produce this kind of file by merging threshold scan data customize and run the  script in ../../data/thrscan/preprocess/ folder\n");
    printf("*****************************************************\n");
    printf("Example1: %s ped ../../data/thrscan/preprocess/200306_ALL/ped.txt  1 \n",argv[0]);
    printf("Example2: %s dark ../../data/thrscan/preprocess/200306_ALL/dark.txt 10 \n",argv[0]);
    printf("*****************************************************\n");
    return -1;
  }

  TNA62richThrScan * r = new TNA62richThrScan();
  r->SetName(argv[1]);
  int ret = r->ReadFile(argv[2]);
  if(ret!=0) return -1;
  r->SetDaqDuration(atoi(argv[3]));
  r->Fill();
  r->WriteRoot();
  if( strcmp(argv[1],"ped")==0){
    r->WriteTxt();
    r->ChipStat();
  }
  else if( strcmp(argv[1],"dark")==0){
   // to be implemented
  }
  r->Plot();  // individual channel log scale
  r->Plot2D();
  r->PlotSuperimpose(0); // ped.root and dark.root superimposed in linear scale
  r->PlotSuperimpose(1); // ped.root and dark.root superimposed in log scale
  delete r;
  return 0;
}
