#include "TNA62richGeo.h"
#include "TNA62richMap.h"

#include "TNA62richFrontend.h" // to be able to distinguish between connected and spares channels

#include <stdio.h>

#include <TMath.h>
#include <TCanvas.h>
#include <TLine.h>

//------------------------------------------------------------------------//
TNA62richGeo::TNA62richGeo(int nDisk, int whichDisk) {
//------------------------------------------------------------------------//

   switch(nDisk){
     case 1: fnDisk = 1; break;
     case 2: fnDisk = 2; break;
     default: printf("Warning N disk can be 1 or 2, you have %d\n",nDisk);
   }
 // printf("Creating data display with %d disk\n",fnDisk);

  if(fnDisk==1){
    if(whichDisk!=0 && whichDisk!=1){
      printf("Warning disk can be 0 (Jura) or 1 (Saleve), you have %d\n",whichDisk);
      whichDisk = 0; // default choice is JURA
    }
    fDisk = whichDisk;
  }

  fh2p = new TH2Poly();
  fr = 9;     // radius in mm of Winstone cones (pmt radius is 8 mm)
  fk = 2 * fr * TMath::Sin(TMath::Pi()/3.);

  // create the first disk (translated to have same coordinates as F.Bucci)
  fxabs=-30*fr;
  fyabs=3.0*fk;
  this->GenUpSector();
  this->GenDownSector();

  if(fnDisk==2){
    // create a second disk
    fxabs -=650.0;
    fyabs +=0.0;
    this->GenUpSector();
    this->GenDownSector();
  }
}


//------------------------------------------------------------------------//
TNA62richGeo::~TNA62richGeo() {
//------------------------------------------------------------------------//

  if(fh2p) delete fh2p;
}


//------------------------------------------------------------------------//
void TNA62richGeo::Clear() {
//------------------------------------------------------------------------//

//  fh2p->ClearBinContents();
//  fh2p->Reset("ICESM");
  fh2p->Reset("M");

}





// -------------------------------------- //
void  TNA62richGeo::GenDownSector(){
// -------------------------------------- //

  // shift factors are universal
  double xshift = 5*fr;
  double yshift = -1;

  // each supercell row needs the coordinates where to start
  double xoffset = fxabs; // coordinates of the first bin
  double yoffset = fyabs;  // coordinates of the first bin

  // and the number n of cells in the row
  int n=11;

  // from cell 0 to 11
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 12 to 23
  xoffset = fxabs -1*fr;
  yoffset = fyabs -3*fk;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 24 to 34
  xoffset = fxabs - 2*fr;
  yoffset = fyabs - 6*fk;
  n = 10;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 35 to 45
  xoffset = fxabs - 3*fr;
  yoffset = fyabs - 9*fk;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 46 to 54
  xoffset = fxabs + 1*fr;
  yoffset = fyabs - 13*fk;
  n = 8;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 55 to 60
  xoffset = fxabs + 5*fr;
  yoffset = fyabs -17*fk;
  n = 5;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);
}


// -------------------------------------- //
void  TNA62richGeo::GenUpSector(){
// -------------------------------------- //

  // shift factors are universal
  double xshift = 5*fr;
  double yshift = -1;

  // each supercell row needs the coordinates where to start
  double xoffset = fxabs + 1*fr; // coordinates of the first bin
  double yoffset = fyabs + 3*fk;  // coordinates of the first bin

  // and the number n of cells in the row
  int n=11;

  // from cell 0 to 11
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);


  // from cell 12 to 23
  xoffset = fxabs + 2*fr;
  yoffset = fyabs + 6*fk;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 24 to 34
  xoffset = fxabs + 8*fr;
  yoffset = fyabs + 8*fk;
  n = 10;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

 // from cell 35 to 45
  xoffset = fxabs + 9*fr;
  yoffset = fyabs + 11*fk;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

 // from cell 46 to 54
  xoffset = fxabs +15*fr;
  yoffset = fyabs +13*fk;
  n = 8;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);

  // from cell 55 to 60
  xoffset = fxabs +26*fr;
  yoffset = fyabs +14*fk;
  n = 5;
  GenSuperCellPattern(xoffset,yoffset,n,xshift,yshift);
}


// ----------------------------------------------------------------------------------------------------//
void TNA62richGeo::GenSuperCellPattern(double xoffset,double yoffset,int n,double xshift, double yshift){
// ----------------------------------------------------------------------------------------------------//

  // first super cell in the pattern
  GenSuperCell(xoffset,yoffset);

  // other n cells each translated by xshift,yshift
  for(int i=0;i<n;i++){
    xoffset += xshift;
    yoffset += yshift*fk;
    GenSuperCell(xoffset,yoffset);
  }
}


// -------------------------------------- //
void  TNA62richGeo::GenSuperCell(double xoffset, double yoffset){
// -------------------------------------- //

  double xc[8],yc[8];    //  coordinates of the center of the 8 PMTs

  xc[0] = xoffset;
  xc[1] = xc[0] + 2*fr;
  xc[2] = xc[0] + 4*fr;
  xc[3] = xc[0] + 1*fr;
  xc[4] = xc[0] + 3*fr;
  xc[5] = xc[0] + 0*fr;
  xc[6] = xc[0] + 2*fr;
  xc[7] = xc[0] + 4*fr;

  yc[0] = yoffset;
  yc[1] = yc[0];
  yc[2] = yc[0];
  yc[3] = yc[0] + fk;
  yc[4] = yc[0] + fk;
  yc[5] = yc[0] + 2*fk;
  yc[6] = yc[0] + 2*fk;
  yc[7] = yc[0] + 2*fk;

  for(int i=0;i<8;i++){
    GenPolygon(xc[i],yc[i]);
    fxvec.push_back(xc[i]);
    fyvec.push_back(yc[i]);
  }
}

// -----------------------------------------------//
void TNA62richGeo::GenPolygon(double xc, double yc){
// -----------------------------------------------//

  const int n=24;
  double x[n],y[n];
  for(int i=0;i<n;i++) x[i] = xc + fr * TMath::Cos(2*TMath::Pi()*i/n);
  for(int i=0;i<n;i++) y[i] = yc + fr * TMath::Sin(2*TMath::Pi()*i/n);
  fh2p->AddBin(n, x, y);
}

// ----------------------//
void TNA62richGeo::Dump(){
// ---------------------//

 /*
  printf("List of PMT coordinates\n");
  for(int i=0;i<fxvec.size();i++){
    printf("%4d",i+1);
    printf("%4d %4d",i/8,i%8);
    printf("%9.3lf %9.3lf",fxvec[i],fyvec[i]);
    printf("\n");
  }
  */

  int seqID;
  double x,y;
  int disk,sector,feb,fec;

  TNA62richMap map;

  for(int echannel=0;echannel<MAXCH;echannel++){
    if(!IsConnected(echannel)) continue;
    seqID = map.GetSequentialID(echannel);
    x = -1;
    y = -1;

    if(seqID>=0 && seqID<(int)fxvec.size()){
      x = fxvec[seqID];
      y = fyvec[seqID];
    }
    disk = GetDisk(echannel);
    sector = GetSector(echannel);
    feb = GetFeBoard(echannel);
    fec = GetFeChannel(echannel);
    printf("DISK %d ",disk);
    printf("SECTOR %2d ",sector);
    printf("FEB %3d ",feb);
    printf("CHANNEL %3d ",fec);
    printf("ELEID %4d ",echannel);
    printf("SEQID %4d ",seqID);
    printf("X %8.3lf ",x);
    printf("Y %8.3lf ",y);
    printf("\n");
  }
}


// -------------------------------------- //
void TNA62richGeo::Fill(int echannel, double val){
// -------------------------------------- //

  if(fnDisk==1 && fDisk==0 && echannel > 1023) return;  // avoid bin double fill
  if(fnDisk==1 && fDisk==1 && echannel < 1024) return;  // avoid bin double fill

  int p = 0;
  TNA62richMap map;
  int seqID = map.GetSequentialID(echannel);

  if(fnDisk==1 && fDisk==1) seqID -= 976; // fill Saleve data on Jura disk 0

  double x = fxvec[seqID];
  double y = fyvec[seqID];

  if(p){
    printf("SEQ %4d ",seqID);
    printf("ELE %d ",echannel);
    printf("X %8.3lf Y %8.3lf ",x,y);
    printf("VAL %8.3lf\n",val);
  }
  fh2p->Fill(x,y,val);
}

// -------------------------------------- //
void TNA62richGeo::SetMinimum(double val){
// -------------------------------------- //

  fh2p->SetMinimum(val);

}

// -------------------------------------- //
void TNA62richGeo::SetMaximum(double val){
// -------------------------------------- //

  fh2p->SetMaximum(val);

}



// -------------------------------------- //
void TNA62richGeo::SetTitle(const char * title){
// -------------------------------------- //

  fh2p->SetTitle(title);
}

// -------------------------------------- //
void TNA62richGeo::Draw(const char * opt){
// -------------------------------------- //

   // In case of an external canvas
  fh2p->GetYaxis()->SetLimits(-40*fr,40*fr);

  fh2p->GetXaxis()->SetTitle("X [mm]");
  fh2p->GetYaxis()->SetTitle("Y [mm]");

  //fh2p->GetXaxis()->CenterTitle();
  //fh2p->GetYaxis()->CenterTitle();

  fh2p->GetXaxis()->SetLabelSize(0.04);
  fh2p->GetXaxis()->SetTitleSize(0.04);
  fh2p->GetYaxis()->SetLabelSize(0.04);
  fh2p->GetYaxis()->SetTitleSize(0.04);
  fh2p->GetZaxis()->SetLabelSize(0.04);
  fh2p->GetZaxis()->SetTitleSize(0.04);

  int xlim = 120;
  int xcanv = 2000;
  if(fnDisk==1){
    xlim = 40;
    xcanv = 1000;
  }
  fh2p->GetXaxis()->SetLimits(-1*xlim*fr, +40*fr);
  //fh2p->Draw("COLZ,L");

  fh2p->SetMarkerSize(0.35);
  fh2p->Draw(opt);

  int internalCanvas = 0;  // for test only
  if(internalCanvas){
    std::string outPdf = "geoInternalCanvas.pdf";
    TCanvas * c = new TCanvas("cGeo","NA62_RICH_monitor", xcanv,1000);
    c->cd()->SetRightMargin(0.18);
    c->Print(Form("%s[",outPdf.c_str()));
    // page 1
    fh2p->Draw("COLZ");
    c->Print(Form("%s",outPdf.c_str()));

    // page 2
    fh2p->SetMarkerSize(0.25);
    fh2p->Draw("TEXT");
    c->Print(Form("%s",outPdf.c_str()));

    // page 3
    fh2p->Draw("COLZ2");
    c->Print(Form("%s",outPdf.c_str()));

    // page 4
    fh2p->Draw("COLZ0");
    c->Print(Form("%s",outPdf.c_str()));

    // page 5
    fh2p->Draw("COLZ1");
    c->Print(Form("%s",outPdf.c_str()));

    // page 6    <-- this is good looking to me
    fh2p->Draw("COLZ,L");
    c->Print(Form("%s",outPdf.c_str()));

    // page 7    <-- this is good looking to Mauro Piccini (used in 2019 paper)
    fh2p->Draw("COLZ,P");
    c->Print(Form("%s",outPdf.c_str()));

    // page 8    <-- option F has no effect
    fh2p->Draw("COLZ,F");
    c->Print(Form("%s",outPdf.c_str()));

    c->Print(Form("%s]",outPdf.c_str()));
    delete c;
    printf("Internal Canvas ready in %s\n",outPdf.c_str());

  }
}

