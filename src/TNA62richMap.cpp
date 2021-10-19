#include "TNA62richMap.h"
#include "TNA62richFrontend.h"


#include <stdio.h> // printf, fopen
#include <string.h> // strcpy   C



//------------------------------------------------------------------------//
TNA62richMap::TNA62richMap() {
//------------------------------------------------------------------------//

  for(int i=0;i<MAXPIX;i++) this->ResetPixel(i);

  ReadTranslationTable();
}


//------------------------------------------------------------------------//
TNA62richMap::~TNA62richMap() {
//------------------------------------------------------------------------//

}


//------------------------------------------------------------------------//
void TNA62richMap::ResetPixel(int i){
//------------------------------------------------------------------------//

  strcpy(fPixel[i].serial,"AA1234");
  fPixel[i].abs        = -1;
  fPixel[i].supercell  = -1;
  fPixel[i].pmt        = -1;
  fPixel[i].sequential = -1;
  fPixel[i].readout    = -1;
  fPixel[i].trigger    = -1;
}

//------------------------------------------------------------------------//
void TNA62richMap::DumpPixel(int i){
//------------------------------------------------------------------------//

  if(i<1 || i>MAXPIX){
    printf("Error: pixel id out of range [1..%d]\n",MAXPIX);
    return;
  }
  int fe = fPixel[i].readout;
  printf("Information on Pixel %d\n",fPixel[i].abs);
  printf("PMT serial number %s\n",fPixel[i].serial);
  printf("SUPERCELL %d (%d) \n",fPixel[i].supercell,fPixel[i].pmt);
  printf("SEQUENTIAL %d \n",fPixel[i].sequential);
  printf("TRIGGER %d \n",fPixel[i].trigger);
  printf("FRONTEND %d: \n",fe);
  printf("\tSECTOR %d ",GetSector(fe));
  printf("BOARD %d ",GetFeBoard(fe));
  printf("ASIC %d ",GetAsic(fe));
  printf("CHANNEL %d ",GetAsicChannel(fe));
  printf("\n");
  printf("\tABSOLUTE BOARD %d\n",GetFeBoardAbsolute(fe));
  printf("\tABSOLUTE ASIC %d\n",GetAsicAbsolute(fe));
  return;
}



//------------------------------------------------------------------------//
int TNA62richMap::ReadTranslationTable(){
//------------------------------------------------------------------------//

  int p=0;

  //char inTxt[] =  "./translation_table.txt";
  char inTxt[] =  "../map/translation_table.txt";

  FILE * fin = fopen(inTxt,"r");
  if(!fin){printf("Error: file %s not found\n",inTxt);return -1;}

  unsigned int val;
  char str[6];

  for(int i=0;i<MAXPIX;i++){
    fscanf(fin,"%d",&val); fPixel[i].abs = val;
    fscanf(fin,"%s",str);  strcpy(fPixel[i].serial,str);
    fscanf(fin,"%d",&val); fPixel[i].supercell = val;
    fscanf(fin,"%d",&val); fPixel[i].pmt = val;
    fscanf(fin,"%d",&val); fPixel[i].sequential = val;
    fscanf(fin,"%d",&val); fPixel[i].readout = val;
    fscanf(fin,"%d",&val); fPixel[i].trigger = val;
    fscanf(fin,"%d",&val); // last column is discarded for the moment

    if(fPixel[i].sequential==1499){ // 2020, January 28 Matteo Turisini
      if(p)printf("********************************************************\n");
      if(p)printf("FIX in translation table between SEQID and ELEID (map0) \n");
      if(p)printf("SEQID 1499  now is ELEID 2030 (before was ELEID 1798)\n");
      if(p)printf("but the map was not updated. Thus it is updated here:\n");
      if(p)this->DumpPixel(i);
      fPixel[i].readout=2030;
      if(p)this->DumpPixel(i);
      if(p)printf("********************************************************\n");
    }
  }
  fclose(fin);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richMap::GetPixelID(int echannel){
//------------------------------------------------------------------------//
  return this->Get("abs",echannel);
}

//------------------------------------------------------------------------//
int TNA62richMap::GetSuperCellID(int echannel){
//------------------------------------------------------------------------//
  return this->Get("spc",echannel);
}


//------------------------------------------------------------------------//
int TNA62richMap::GetZerotoSevenID(int echannel){
//------------------------------------------------------------------------//
  return this->Get("pmt",echannel);
}

//------------------------------------------------------------------------//
int TNA62richMap::GetSequentialID(int echannel){
//------------------------------------------------------------------------//
  return this->Get("seq",echannel);
}

//------------------------------------------------------------------------//
int TNA62richMap::GetTriggerID(int echannel){
//------------------------------------------------------------------------//
  return this->Get("trg",echannel);
}

//------------------------------------------------------------------------//
int TNA62richMap::Get(const char * varname,int echannel){
//------------------------------------------------------------------------//

 for(int i=0;i<MAXPIX;i++){
   if(fPixel[i].readout==echannel){
      if(strcmp(varname,"abs")==0) return (int)fPixel[i].abs;
      if(strcmp(varname,"spc")==0) return (int)fPixel[i].supercell;
      if(strcmp(varname,"pmt")==0) return (int)fPixel[i].pmt;
      if(strcmp(varname,"seq")==0) return (int)fPixel[i].sequential;
      if(strcmp(varname,"trg")==0) return (int)fPixel[i].trigger;
   }
 }
 printf("Warning: echannel %d not found\n",echannel);
 return -2;
 }

//------------------------------------------------------------------------//
int TNA62richMap::GetEchannelFromPixel(int pixelAbsoluteID)
//------------------------------------------------------------------------//
{
  int k = pixelAbsoluteID;
  if(k<1 || k>MAXPIX){
    printf("Error: pixel id out of range [1..%d]\n",MAXPIX);
    return -1;
  }

  for(int i=0;i<MAXPIX;i++){
    if(fPixel[i].abs==k){
      return fPixel[i].readout;
    }
  }

  printf("Warning: pixel %d not found\n",k);
  return -2;
}

//------------------------------------------------------------------------//
int TNA62richMap::GetEchannelFromSeqID(int seqID){
//------------------------------------------------------------------------//

  for(int i=0;i<MAXPIX;i++){
    if(fPixel[i].sequential==seqID){
      return fPixel[i].readout;
    }
  }
  printf("Warning: SEQID %d not found\n",seqID);
  return -2;
}

 //------------------------------------------------------------------------//
std::string TNA62richMap::GetSerialNumber(int echannel){
//------------------------------------------------------------------------//

 for(int i=0;i<MAXPIX;i++){
   if(fPixel[i].readout==echannel){
     return fPixel[i].serial;
   }
 }
 printf("Sorry, PMT serial number not found for ELEID %d\n",echannel);
 return "unknown";
}

