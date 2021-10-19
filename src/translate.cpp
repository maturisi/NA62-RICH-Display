#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */

#include "TNA62richMap.h"

int main(int argc, char *argv[]) {

  if(argc!=2){
    printf("Usage: %s <eleID> \n",argv[0]);
    printf("       eleID - front end channel [0..2047]\n");
    printf("*****************************************************\n");
    printf("%s dump info about an individual frontend channel\n",argv[0]);
    printf("*****************************************************\n");
    printf("Example: %s  484\n",argv[0]);
    printf("*****************************************************\n");
    return -1;
  }
  int feChannel = atoi(argv[1]);

  if(!IsConnected(feChannel)){
    printf("Channel %d not connected\n",feChannel);
    return -1;
  }

  TNA62richMap map;
  int pix = map.GetPixelID(feChannel);
  map.DumpPixel(pix-1); // pixel ID starts from 1 and we want the array index that's why the -1
  return 0;
}
