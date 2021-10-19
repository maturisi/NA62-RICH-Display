#ifndef NA62_RICH_PIXEL_H
#define NA62_RICH_PIXEL_H

#include "TNA62richFrontend.h" // MAXCH
#include <string> // std::string

class TNA62richPixel {

  private:
    double fVal[MAXCH]; // generic data about RICH pixels

    int fFormat;        // first column options: 0 unknown, 1 pixID, 2 seqID
    std::string fWhat;  // name of the variable gain, efficiency....

  public:
    TNA62richPixel();
   ~TNA62richPixel();

   int ReadData(const char * filename);
   int Dump();
   int Analise();
   int Draw();
   void SetFormat(int opt);
   void SetVarName(const char * what);

private:
   void ResetValues();

};

#endif
