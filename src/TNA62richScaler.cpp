#include "TNA62richScaler.h"
#include "TNA62richGeo.h"
#include "TNA62richMap.h"
#include "TNA62richFrontend.h" // GetSector,GetFeboard...

#include <string> // std::string
#include <string.h> // strcmp


#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TError.h>
#include <TLatex.h>
//------------------------------------------------------------------------//
TNA62richScaler::TNA62richScaler() {
//------------------------------------------------------------------------//

//  printf("Hello from TNA62richScaler\n");
  gErrorIgnoreLevel = kError;
  ResetCounts();

  fNumberOfBurst = 1;
  fBurstDuration = 5;
  fMaxFreq       = 150;  // default value can be changed, see SetMaxFreq
  fHotCount  = 1e6;
}

//------------------------------------------------------------------------//
TNA62richScaler::~TNA62richScaler() {
//------------------------------------------------------------------------//
  //printf("Goodbye from TNA62richScaler\n");
}

//------------------------------------------------------------------------//
void TNA62richScaler::ResetCounts(){
//------------------------------------------------------------------------//
  for(int i=0;i<MAXCH;i++) fCount[i]=0;
}

//------------------------------------------------------------------------//
void TNA62richScaler::SetMaxFreq(double value){
//------------------------------------------------------------------------//
  fMaxFreq = value;
  printf("Max Freq (hot pixel) = %.1lf [Hz]\n",fMaxFreq);
  return;
}

//------------------------------------------------------------------------//
void TNA62richScaler::SetDaqParams(int numberOfBurst, double burstDuration){
//------------------------------------------------------------------------//
  fNumberOfBurst = numberOfBurst;
  fBurstDuration = burstDuration;
  printf("Number of Bursts = %d [#]\n",fNumberOfBurst);
  printf("Burst Duration = %.1lf [s] \n",fBurstDuration);
  return;
}


//------------------------------------------------------------------------//
void TNA62richScaler::CalculateHotThreshold(){
//------------------------------------------------------------------------//
  double duration = fBurstDuration*fNumberOfBurst;
  fHotCount = fMaxFreq * duration;
  return;
}


//------------------------------------------------------------------------//
int TNA62richScaler::Dump(){
//------------------------------------------------------------------------//
  for(int i=0;i<MAXCH;i++){
    if(IsConnected(i)){
      printf("CH %4d COUNTS %.0lf\n",i,fCount[i]);
    }
  }
  return 0;
}


//------------------------------------------------------------------------//
int TNA62richScaler::ReadFile(const char * filename) {
//------------------------------------------------------------------------//

  ResetCounts();

  printf("%s %s\n",__FUNCTION__,filename);

  FILE * fin = fopen(filename,"r");
  if(!fin){printf("Error: file %s not found",filename); return -1;}
  int channel;
  int count;
  while(fscanf(fin,"%d %d\n",&channel,&count)!=EOF){
    if(channel<0||channel>(MAXCH-1)){
      printf("%s Error: %d %d\n",__FUNCTION__,channel,count);
    }else{
      fCount[channel] = count;
    }
  }
  fclose(fin);
  return 0;
}


//------------------------------------------------------------------------//
int TNA62richScaler::Analise() {
//------------------------------------------------------------------------//

  this->CalculateHotThreshold();
  double lDuration = fBurstDuration*fNumberOfBurst;

  std::string deadName = "dead.txt";
  std::string hotName = "hot.txt";
  FILE * deadfile = fopen(deadName.c_str(),"w");
  FILE * hotfile = fopen(hotName.c_str(),"w");
  if(!deadfile)printf("Error: problem opening file %s\n",deadName.c_str());
  if(!hotfile)printf("Error: problem opening file %s\n",hotName.c_str());

  TH1F* h = new TH1F("h","",fMaxFreq/2,0,fMaxFreq);
  TH1F* hok = new TH1F("hok","",fMaxFreq/2,0,fMaxFreq); // dead and hot removed

  h->SetTitle("");
  h->GetXaxis()->SetTitle("rate [s^{-1} ]");
  h->GetYaxis()->SetTitle("number of PMs");

  hok->SetTitle("");
  hok->GetXaxis()->SetTitle("rate [s^{-1} ]");
  hok->GetYaxis()->SetTitle("number of PMs");

  int nHot=0;
  int nDead=0;
  int nOk=0;
  for(int i=0;i<MAXCH;i++){
    if(!IsConnected(i))continue;

    h->Fill(fCount[i]/lDuration);

    if(fCount[i]<=0.0){
      fprintf(deadfile,"ELEID %4d ",i);
      fprintf(deadfile,"SECTOR %4d ",GetSector(i));
      fprintf(deadfile,"BOARD %4d ",GetFeBoard(i));
      fprintf(deadfile,"CHANNEL %4d ",GetFeChannel(i));
      fprintf(deadfile,"COUNTS %.0lf ",fCount[i]);
      fprintf(deadfile,"\n");
      nDead++;
    }
    else if(fCount[i]>fHotCount){
      fprintf(hotfile,"ELEID %4d ",i);
      fprintf(hotfile,"SECTOR %4d ",GetSector(i));
      fprintf(hotfile,"BOARD %4d ",GetFeBoard(i));
      fprintf(hotfile,"CHANNEL %4d ",GetFeChannel(i));
      fprintf(hotfile,"COUNTS %.0lf ",fCount[i]);
      fprintf(hotfile,"\n");
      nHot++;
    }
    else{
      hok->Fill(fCount[i]/lDuration);
      nOk++;
    }
  }
  fclose(deadfile);
  fclose(hotfile);
  double entries = h->GetEntries();
  double mean = h->GetMean();
  double rms = h->GetRMS();
  if(entries!=0){
    printf("Statistics: \n");
    printf("Average Rate      %6.0lf [cps]\n",mean);
    printf("Rate spread (RMS) %6.0lf [cps]\n",rms);
    printf("%4d good (%4.1lf%%) \n",nOk,100*nOk/entries);
    printf("%4d  hot (%4.1lf%%) in %s (hot threshols is %.1lf [Hz]\n",nHot,100*nHot/entries,hotName.c_str(),fMaxFreq);
    printf("%4d dead (%4.1lf%%) in %s\n",nDead,100*nDead/entries,deadName.c_str());
  }
  gStyle->SetOptStat(0);
  const char * outPdf = "distr_scalerRate.pdf";
  TCanvas * cStat = new TCanvas("cStat");
  cStat->cd()->SetLeftMargin(0.13);
  cStat->cd()->SetBottomMargin(0.13);
  cStat->cd()->SetLogy(1);
  cStat->Print(Form("%s[",outPdf));

  h->SetMinimum(0.1);
  h->SetMaximum(1000);

  h->Draw();
  //cStat->Print(Form("%s",outPdf));
  cStat->cd()->SetLogy(0);
  h->SetMinimum(0);
  h->SetMaximum(1000);
 // cStat->Print(Form("%s",outPdf));


  hok->SetMinimum(0);
  hok->SetMaximum(220);

 // hok->SetFillColorAlpha(kBlue, 0.35);
  //hok->SetFillStyle(3354);
  hok->GetXaxis()->SetLabelSize(0.05);
  hok->GetXaxis()->SetTitleSize(0.05);
  hok->GetYaxis()->SetLabelSize(0.05);
  hok->GetYaxis()->SetTitleSize(0.05);
  hok->Draw();
  cStat->Print(Form("%s",outPdf));
  cStat->Print(Form("%s]",outPdf));
  printf("Histo in %s\n",outPdf);
  delete cStat;
  delete h;
  return 0; 
}


//------------------------------------------------------------------------//
int TNA62richScaler::Draw() {
//------------------------------------------------------------------------//
 
  gStyle->SetOptStat(0);
  
  double rmargin = 0.13; //right margin to make Z axis readble
  double lmargin = 0.13; //left margin to make Y axis readble

  char outPdfA[] ="ele_scaler.pdf";  // eleview
  char outPdfB[] ="geo_scaler.pdf";  // geoview 2 disks
  char outPdfC[] ="geo_scaler_jura.pdf";  // geoview 1 disk JURA
  char outPdfD[] ="geo_scaler_saleve.pdf";  // geoview 1 disk SALEVE
 
 
  TCanvas * c = NULL;     //  ele display
  TCanvas * canv = NULL;  // geo display 2 disks
  TCanvas * cj = NULL;  // geo display 1 disk jura
  TCanvas * cs = NULL;  // geo display 1 disk saleve
  
  double val;  // value
  
  int feb; // frontend board
  int fec; // frontend channel
  
  // scaler
  double min = 1;
  double max = fMaxFreq; // Hz
 
  TH2F * h = new TH2F("h","",32,0,32,64,0,64);
  //h->SetMinimum(min);
  //h->SetMaximum(max); 
  h->GetXaxis()->SetTitle("Channel ID [#]"); 
  h->GetYaxis()->SetTitle("Front End Board ID[#]");  
  // h->GetZaxis()->SetTitle("Count Rate in 10 Burst [Hz]");  
  h->SetTitle(Form("Data Monitor RICH NA62 "));   
  h->GetZaxis()->SetTitleOffset(1.8);
  h->GetXaxis()->CenterTitle();  
 


  TNA62richGeo * g = new TNA62richGeo(2);  
  TNA62richGeo * jura = new TNA62richGeo(1,0);  //JURA 
  TNA62richGeo * sale = new TNA62richGeo(1,1);  //JURA   

  g->SetMinimum(min);
  g->SetMaximum(max);

  jura->SetMinimum(min);
  jura->SetMaximum(max);

  sale->SetMinimum(min);
  sale->SetMaximum(max);


  double lDuration = fBurstDuration*fNumberOfBurst;
 
  for(int echann=0;echann<MAXCH;echann++){
    if(!IsConnected(echann))continue;
    feb = GetFeBoardAbsolute(echann);
    fec = GetFeChannel(echann); 
    val = fCount[echann]/lDuration;
    h->Fill(fec+0.5,feb+0.5,val);
    g->Fill(echann,val);
    jura->Fill(echann,val);
    sale->Fill(echann,val); 
   // printf("CH %4d BOARD %4d CHANNEL %4d VALUE %.0lf\n",echann,feb,fec,val);
  }
 
   // ELECTRONIC VIEW
  c = new TCanvas("c","RICH Monitor",400,500);
  c->cd()->SetLeftMargin(lmargin);
  c->cd()->SetRightMargin(rmargin);  
 // c->cd()->SetLogz(1);
  h->Draw("COLZ");  
  c->Print(Form("%s",outPdfA));  
  
  // DOUBLE DISK VIEW
  double w = 1000;  // to manage proportion of the canvas height and width
  canv = new TCanvas("canv",""); 
  canv->SetCanvasSize(w,w/2);
 // canv->cd()->SetLogz(1);
  g->Draw();
  canv->Print(Form("%s",outPdfB));
 // g->Dump();

  // SINGLE JURA  
  cj = new TCanvas("cj",""); 
  cj->SetCanvasSize(w,w);
  cj->cd()->SetLeftMargin(lmargin);
  cj->cd()->SetRightMargin(rmargin);  
  jura->Draw();
  cj->Print(Form("%s",outPdfC));

  // SINGLE SALEVE 
  cs = new TCanvas("cs",""); 
  cs->SetCanvasSize(w,w);
  cs->cd()->SetLeftMargin(lmargin);
  cs->cd()->SetRightMargin(rmargin);  
  sale->Draw();
  cs->Print(Form("%s",outPdfD));

  delete jura;
  delete sale;
  delete g; 
  delete h;
  delete c;
  delete canv;
  delete cj;
  delete cs;
  printf("Display ready in %s\n",outPdfA);
  printf("Display ready in %s\n",outPdfB);
  printf("Display ready in %s\n",outPdfC);
  printf("Display ready in %s\n",outPdfD);

  return 0; 
}
