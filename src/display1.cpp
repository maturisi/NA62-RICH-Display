/* display scaler data*/
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <string>       /*std::string */

#include "TNA62richScaler.h"

int main(int argc, char *argv[]) {

  double duration= 5; // seconds
  int maxFreq = 150; // limit to separate normal from hot pixels
  int nBurst = 10;

  if(argc!=3){
    printf("Usage: %s <filename> <nBurst> \n",argv[0]);
    printf("       filename - input file name\n");
    printf("       nBurst   - scaler integration time expressed in number of burst \n");
    printf("*****************************************************\n");
    printf("%s plots scaler data about the NA62-RICH detector\n",argv[0]);
    printf("Input file is 2 column x 2048 rows plain text file\n");
    printf("The first column is ELEID\n");
    printf("The second column is the number of counts in the elapsed time\n");
    printf("This kind of file can be produced using mbcounter executable\n");
    printf("See merge.sh to join the four RICH sector together in a single file\n");
    printf("A 5 second burst duration is assumed for counting rates computation\n");
    printf("When a pixel fires more than %d cps it is considered a hot pixel\n",maxFreq);
    printf("Edit and recompile to change this hot pixel threshold\n");
    printf("*****************************************************\n");
    printf("Example: %s ../data/scaler/data_scaler.txt %d\n",argv[0],nBurst);
    printf("*****************************************************\n");
    return -1;
  }

  std::string inTxt = argv[1];;
  nBurst = atoi(argv[2]); // depends on DAQ settings


  TNA62richScaler scaler;
  int err = scaler.ReadFile(inTxt.c_str());
  if(err) return -1;

  scaler.SetMaxFreq(maxFreq);
  scaler.SetDaqParams(nBurst,duration);
  //scaler.Dump();
  scaler.Analise();
  scaler.Draw();
  return 0;
}
