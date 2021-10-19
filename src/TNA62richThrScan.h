#include <TH1F.h>
#include <string>

#include "TNA62richFrontend.h" // MAXCH
#include "TNA62richMap.h" // map

#define NTH 4096
#define NCHIP (MAXCH/8) // NINO chip settings are common to 8 channels


class TNA62richThrScan {

  private:
    double fcnt[MAXCH][NTH]; // matrix to store thr scan data
    double fDuration;
    std::string fName;  // ped or dark
    TNA62richMap * fMap;

    TH1F * fh[MAXCH]; // count histogram
    double fped[MAXCH];  // pedestal of each channel
    int fpedchip[NCHIP]; // chip pedestal
    int fthrchip[NCHIP]; // chip threshold

  public:
    TNA62richThrScan();
   ~TNA62richThrScan();

   int ReadFile(const char * filename);
   int Dump();
   int Plot();
   int Plot2D();
   int PlotSuperimpose(int logy);
   int Fill();
   int WriteTxt();
   int WriteRoot();

   int ChipStat();
   int ChipStatRead();

   void SetName(const char * name); // it can be "ped" or "dark"
   void SetDaqDuration(int value_in_seconds);

   std::string GetDescriptionString(int echannel);

  private:
    int Reset();
    int CreateHisto();
    int DumpHisto(int ch);
};
