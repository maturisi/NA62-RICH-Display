#ifndef NA62_RICH_MAP_H
#define NA62_RICH_MAP_H

#include "TNA62richFrontend.h" // MAXPIX

#include <string> // std::string  C++

// define a line on the map
typedef struct pixel
{
  char serial[6]; // serial number from hamamatsu
  int  abs;       // absolute pixel ID
  int  supercell;  // supercell identifier
  int  pmt;        // identifier inside the supercell
  int  sequential; // sequential identifier
  int  readout;    // electronic channel identifier
  int  trigger;    // trigger identifier
} t_pixel;



class TNA62richMap {

  private:
    t_pixel fPixel[MAXPIX];

  public:
    TNA62richMap();
    ~TNA62richMap();

    int         ReadTranslationTable();
    void        ResetPixel          (int i);
    void        DumpPixel           (int i);

    int         GetPixelID          (int echannel);
    int         GetSuperCellID      (int echannel);
    int         GetZerotoSevenID    (int echannel);
    int         GetSequentialID     (int echannel);
    int         GetTriggerID        (int echannel);
    std::string GetSerialNumber     (int echannel);

    int         GetEchannelFromPixel(int pixel);
    int         GetEchannelFromSeqID(int seqid);

  private:
    int Get(const char * str,int echannel);
};
#endif
