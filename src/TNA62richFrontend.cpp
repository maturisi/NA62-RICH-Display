#include "TNA62richFrontend.h"

int GetDisk           (int ch){return ch/1024;}
int GetSector         (int ch){return ch/512;}
int GetFeBoard        (int ch){return (ch/32)%16;}
int GetFeBoardAbsolute(int ch){return ch/32;}
int GetFeChannel      (int ch){return ch%32;}


int GetAsic(int ch)  {return GetFeChannel(ch)/8;} // 0...3
int GetAsicAbsolute(int ch){return ch/8; }  // 0..255
int GetAsicChannel(int ch){return ch%8;}    //  0...7

//-------------------------------
bool IsConnected(int ch) {
//-------------------------------

  bool ret;

  // last 24 channels of each sector are assumed spares
  bool spareJ1 = (ch>= 488 && ch <= 511) ? true: false;
  bool spareJ2 = (ch>=1000 && ch <=1023) ? true: false;
  bool spareS1 = (ch>=1512 && ch <=1535) ? true: false;
  bool spareS2 = (ch>=2024 && ch <=2047) ? true: false;
  bool spare = spareJ1 || spareJ2 || spareS1 || spareS2;

  ret = !spare;

  // REMAP
  // Some channels during RICH detector maintenance have benn replaced with spares
  // The following changes reflect the current cabling
  // You have to edit this part everytime the cabling change
  // the arrow ---> "means replaced by"

  // Date: missing ask Francesca Bucci
  // SECTOR 3 BOARD 3 CHANNEL 0 (1632) ---> SECTOR 3 BOARD 15 CHANNEL 14 (2030)
  if(GetFeBoardAbsolute(ch)==51 && GetFeChannel(ch)==0) ret = false;
  if(GetFeBoardAbsolute(ch)==63 && GetFeChannel(ch)==14) ret = true;
  // SECTOR 3 BOARD 8 CHANNEL 6 (1798) ---> SECTOR 3 BOARD 15 CHANNEL 15 (2031)
  if(GetFeBoardAbsolute(ch)==56 && GetFeChannel(ch)==6) ret = false;
  if(GetFeBoardAbsolute(ch)==63 && GetFeChannel(ch)==15) ret = true;

  return ret;
}


