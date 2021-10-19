#ifndef NA62_RICH_GEO_H
#define NA62_RICH_GEO_H

#include <vector>
#include <TH2Poly.h>
#include "TNA62richFrontend.h" // MAXCH

class TNA62richGeo {

  private:
    double fr;
    double fk;
    TH2Poly * fh2p;
    double fxabs;
    double fyabs;
    std::vector<double> fxvec;
    std::vector<double> fyvec;

    int fnDisk; // how many disk you want in the display? 1 or 2
    int fDisk; // 0 Jura   (sectors 0 and 1, it means echannels    0-1023)
               // 1 Saleve (sectors 2 and 3, it means echannels 1024-2047)
               // this option is meaningful only in case of disk number 1

    // Differences between 1 disk or 2 disks
    //  - Number of bin is different
    //  - Axis limits and Canvas Right Margin
    //  - if disk = 1 another parameter is needed: which disk!

  public:
    TNA62richGeo(int nDisk=2,int whichDisk=0); // whichDisk 0 JURA or 1 SALEVE
    // examples:
    // TNA62richGeo()      2 disks
    // TNA62richGeo(2)     2 disks
    // TNA62richGeo(1)     1 disk JURA
    // TNA62richGeo(1,0)   1 disk JURA
    // TNA62richGeo(1,1)   1 disk SALEVE

   ~TNA62richGeo();

    void Fill(int echannel, double val); // electronic channel
    void Draw(const char * opt="COLZ,L");
    void SetMinimum(double val);
    void SetMaximum(double val);
    void SetTitle(const char * title= "title");
    void Dump();
    void Clear(); // clear content of all bins

  private:
    void  GenUpSector();
    void  GenDownSector();
    void  GenSuperCellPattern(double xoffset,double yoffset,int n,double xshift, double yshift);
    void  GenSuperCell(double xoffset, double yoffset);
    void  GenPolygon(double xc, double yc);
};

#endif
