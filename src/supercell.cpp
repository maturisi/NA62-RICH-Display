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
  std::string outPdf = "supercell.pdf";
  std::string title = "";

  TCanvas      * c  = new TCanvas("c","supercell");
  double w = 1000;  // to manage proportion of the canvas height and width
  c->SetCanvasSize(w,w/2);
 // canv->cd()->SetLogz(1);
  c->cd()->SetLeftMargin(0.10);  // forse si misura in percentuale 0.08 = 8%?
  c->cd()->SetRightMargin(0.11);
  c->cd()->SetBottomMargin(0.09);
  c->Print(Form("%s[",outPdf.c_str()));

  int val;
  TNA62richGeo * g  =NULL;
  TNA62richMap map;

  int supercell = 120;

  g = new TNA62richGeo(2);
  for(int echan=0;echan<MAXCH;echan++){ // loop on electronic channel
    if(!IsConnected(echan))continue;
    val = map.GetSuperCellID(echan);
    if(val==supercell) g->Fill(echan,val);
  }
  title= Form("SuperCell %d",supercell);
  g->SetTitle(title.c_str());
  g->Draw("COLZ,L");
  g->Draw("TEXT,SAME");
  c->Print(Form("%s",outPdf.c_str()));
  delete g;
  c->Print(Form("%s]",outPdf.c_str()));
  delete c;
  return 0;
}

