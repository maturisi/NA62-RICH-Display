#include "TNA62richGeo.h"
#include "TNA62richMap.h"
#include "TNA62richFrontend.h" // GetSector,GetFeboard...

#include <stdio.h>      /* printf */
#include <string> // std::string

#include <TCanvas.h>
#include <TStyle.h>
#include <TError.h>

// known issue:
// Refresh the histogram with new value not worked for me
// It seems that the Fill operations are executed but ignored
// In TNA62richGeo::Clear I tried Reset and/or ClearBinConent without success
// so histogram are created and deleted for each variable to be plotted

//--------------------------------------
int main(int argc, char *argv[]) {
//--------------------------------------
  gErrorIgnoreLevel = kError; //kWarning
  gStyle->SetOptStat(0);
  std::string outPdf = "mapping.pdf";
  std::string title = "pippo";

  TCanvas      * c  = new TCanvas("c","mapping");
  double w = 1000;  // to manage proportion of the canvas height and width
  c->SetCanvasSize(w,w/2);
 // canv->cd()->SetLogz(1);
  c->cd()->SetLeftMargin(0.10);  // forse si misura in percentuale 0.08 = 8%?
  c->cd()->SetRightMargin(0.11);
  c->cd()->SetBottomMargin(0.09);
  c->Print(Form("%s[",outPdf.c_str()));

  int val;
  TNA62richGeo * g;

  TNA62richGeo * g2 = NULL;

  TNA62richMap map;

  const char totHisto = 14;
  for(int i=1;i<=totHisto;i++){
    g = new TNA62richGeo(2);
    for(int echan=0;echan<MAXCH;echan++){ // loop on electronic channel
      if(!IsConnected(echan))continue;

      switch(i){
        case  1: val = GetDisk(echan);           title="Disk"; break;
        case  2: val = GetSector(echan);         title="Sector"; break;
        case  3: val = GetFeBoard(echan);        title="FE board"; break;
        case  4: val = GetFeBoardAbsolute(echan);title="FE board absolute"; break;
        case  5: val = GetFeChannel(echan);      title="FE channel"; break;
        case  6: val = GetAsic(echan);           title="ASIC"; break;
        case  7: val = GetAsicAbsolute(echan);   title="ASIC absolute"; break;
        case  8: val = GetAsicChannel(echan);    title="ASIC channel"; break;
        case  9: val = echan;                    title="Electronic channel"; break;
        case 10: val = map.GetPixelID(echan);      title="Pixel"; break;
        case 11: val = map.GetSuperCellID(echan);     title="SuperCell"; break;
        case 12: val = map.GetZerotoSevenID(echan);title="ZeroToSevenID"; break;
        case 13: val = map.GetSequentialID(echan); title="SequentialID"; break;
        case 14: val = map.GetTriggerID(echan);    title="TriggerID"; break;
        default:val = 0;
      }
      g->Fill(echan,val);
    }

    g->SetTitle(title.c_str());
    g->Draw("COLZ,L");

    if(i==11){
      TNA62richGeo * g2 = new TNA62richGeo(2);
      for(int echan=0;echan<MAXCH;echan++){ // loop on electronic channel
        if(!IsConnected(echan))continue;
        int val = map.GetSuperCellID(echan);
        val=val%4;
        g2->Fill(echan,val);
      }
      g2->SetTitle("SupercellID");
      g2->Draw();
    }
    g->Draw("TEXT,SAME");


    c->Print(Form("%s",outPdf.c_str()));
    delete g;
    if(g2)delete g2;
  }
  c->Print(Form("%s]",outPdf.c_str()));
  delete c;

  return 0;
}

