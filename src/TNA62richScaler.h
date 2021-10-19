#ifndef NA62_RICH_SCALER_H
#define NA62_RICH_SCALER_H

#include "TNA62richFrontend.h" // MAXCH

class TNA62richScaler {

  private:
    double fCount[MAXCH]; // scaler data
   
    int fNumberOfBurst;        // daq setting: number of burst
    double fBurstDuration;  // daq setting: duration of single burst
    double fMaxFreq; // ana setting: pixels with counting rate in //Hz higher than MaxFreq will be classified as hot
    double fHotCount;   

  public:
    TNA62richScaler();
   ~TNA62richScaler(); 
   
   int ReadFile(const char * filename); //(channel,counts) x 2048
   int Dump();
  
   void SetDaqParams(int numberOfBurst, double burstDuration);
   void SetMaxFreq(double value); 
  
   int Analise();  
   int Draw();
       
private:
   void ResetCounts();  
   void CalculateHotThreshold();
};
#endif
