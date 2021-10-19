#include "TNA62richPixel.h"
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
TNA62richPixel::TNA62richPixel() {
//------------------------------------------------------------------------//

  //printf("Hello from TNA62richPixel\n");
  gErrorIgnoreLevel = kError; //kWarning
  ResetValues();

  fFormat=0;
  fWhat = "UNKNOWN";

}

//------------------------------------------------------------------------//
TNA62richPixel::~TNA62richPixel() {
//------------------------------------------------------------------------//
//  printf("Goodbye from TNA62richPixel\n");
}

//------------------------------------------------------------------------//
void TNA62richPixel::ResetValues(){
//------------------------------------------------------------------------//
  for(int i=0;i<MAXCH;i++) fVal[i] =0;
}

//------------------------------------------------------------------------//
void TNA62richPixel::SetFormat(int opt){
//------------------------------------------------------------------------//

 int p = 0;
 if(opt<0||opt>3){ printf("Error: bad file format option\n");}
 else{
   fFormat=opt;
  if(p)printf("Format set to %d (0 unknown, 1 pixID, 2 seqID,3 eleID)\n",fFormat);
 }
}

//------------------------------------------------------------------------//
void TNA62richPixel::SetVarName(const char * what){
//------------------------------------------------------------------------//

   int p = 0;

   fWhat=what;
   if(strcmp(fWhat.c_str(),"gain")==0){
     if(p)printf("Gain\n");
   }else if(strcmp(fWhat.c_str(),"klumi")==0){
     if(p)printf("Cathode Luminosity\n");
   }else if(strcmp(fWhat.c_str(),"efficiency")==0){
     if(p)printf("Efficiency\n");
   }else if(strcmp(fWhat.c_str(),"pedmean")==0){
     if(p)printf("Pedestal mean\n");
   }else if(strcmp(fWhat.c_str(),"pedrms")==0){
     if(p)printf("Pedestal rms\n");
   }
   else{
     printf("Erro: Unknown variable type (please specify gain,klumi or efficieny, or implment a new variable!)\n");
   }
}



//------------------------------------------------------------------------//
int TNA62richPixel::Dump(){
//------------------------------------------------------------------------//

printf("Input file format is %d (0 unknown, 1 pixID, 2 seqID,3 eleID)\n",fFormat);
printf("Name of the variable is %s\n",fWhat.c_str());

  for(int i=0;i<MAXCH;i++){
    if(IsConnected(i)){
      printf("CH %4d VAL %.3lf\n",i,fVal[i]);
    }
  }
  return 0;
}




//------------------------------------------------------------------------//
int TNA62richPixel::ReadData(const char * filename) {
//------------------------------------------------------------------------//

  //reads two column file
  int p=0;
  ResetValues();

  FILE * fin = fopen(filename,"r");
  if(!fin){
    printf("Error: file %s not found\n",filename);
    return -1;
  }

  double val, fakeval;
  int pixel;
  int echann;

  TNA62richMap map;

  ResetValues();

  while(fscanf(fin,"%d %lf\n",&pixel,&val)!=EOF){

    if(fFormat==1){
      echann = map.GetEchannelFromPixel(pixel);  // from database we have PIXID
    }else if(fFormat==2){
      echann = map.GetEchannelFromSeqID(pixel); // Mauro Piccini Uses SEQID
    }else if(fFormat==3){
      echann = pixel; // in case the first column is eleID
    }

    if(val<=0 && p){
      printf("------------------------------------\n");
      printf("ATTENTION! Missing information \n");
      fakeval =0.0;  // efficiency from Mauro
      // fakeval =1.0;  // fix gain
      //fakeval =  67.571; //fix klumi
      printf("PIXEL %4d, VAL %lf replaced with VAL %lf\n",pixel,val,fakeval);
      val=fakeval;
      printf("------------------------------------\n");
    }
    if(IsConnected(echann)){
      fVal[echann]=val;
    }
  }
  fclose(fin);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richPixel::Analise() {
//------------------------------------------------------------------------//

  std::string outPdf;
  std::string title;
  std::string titleX;
  std::string titleY;
  double minLin;
  double minLog;
  double maxLin;
  double maxLog;

  TH1F* h;
  int bin=100;
  double xlow=0;
  double xup=1;

  titleY = "number of PMs";

  if(strcmp(fWhat.c_str(),"gain")==0){
    title  = "";
    titleX = "gain";
    outPdf = "distr_gain.pdf";
    minLin = 0;
    minLog = 0.1;;
    maxLin = 150;
    maxLog = 1000;
    bin = 100;
    xlow=0*1e6;
    xup=3*1e6;

  }else if (strcmp(fWhat.c_str(),"klumi")==0){
    title = "";
    titleX = "luminous sensitivity [#muA/lm]";
    outPdf = "distr_klumi.pdf";
    minLin = 0;
    minLog = 0.1;;
    maxLin = 200;
    maxLog = 1000;
    bin = 100;
    xlow=0;
    xup=150;

  }else if (strcmp(fWhat.c_str(),"efficiency")==0){
    title = "";
    titleX = "";
    outPdf = "distr_efficiency.pdf";
    minLin = 0;
    minLog = 0.1;;
    maxLin = 200;
    maxLog = 1000;

    bin = 200;
    xlow=-1;
    xup=1;
  }else if (strcmp(fWhat.c_str(),"pedmean")==0){
    title = "";
    titleX = "";
    outPdf = "distr_pedmean.pdf";
    minLin = 0;
    minLog = 0.1;;
    maxLin = 200;
    maxLog = 1000;

    bin = 100;
    xlow=1700;
    xup=2000;
  }else if (strcmp(fWhat.c_str(),"pedrms")==0){
    title = "";
    titleX = "";
    outPdf = "distr_pedrms.pdf";
    minLin = 0;
    minLog = 0.1;;
    maxLin = 200;
    maxLog = 1000;

    bin = 100;
    xlow=0;
    xup=100;
  }
  else{
    printf("Error: unknown type of data\n");
  }

  h = new TH1F("h","",bin,xlow,xup);

  double min = 1e9;
  double max = -1e9;
  double curr;
  for(int i=0;i<MAXCH;i++){
    if(!IsConnected(i))continue;
    //if(fVal[i]<=0)continue;
    if(strcmp(fWhat.c_str(),"gain")==0){
       h->Fill(fVal[i]*1E6);
    }else{
     h->Fill(fVal[i]);
    }
    // update max e min
    curr = fVal[i];
    if(curr>max) max = curr;
    if(curr<min) min = curr;
  }
  double entries = h->GetEntries();
  double mean    = h->GetMean();
  double rms     = h->GetRMS();
  printf("Statistics on variable %s: \n",fWhat.c_str());
  printf("Total Channel: %6.0lf \n",entries);
  printf("Mean %6.3lf  RMS %6.3lf \n",mean,rms);
  printf("Min %6.3lf  Max %6.3lf \n",min,max);

  gStyle->SetOptStat(0);

  TCanvas * cStat = new TCanvas("cStat");
  cStat->Print(Form("%s[",outPdf.c_str()));
  cStat->cd()->SetLogy(1);
  cStat->cd()->SetLeftMargin(0.13);
  cStat->cd()->SetBottomMargin(0.13);

  h->SetMinimum(minLog);
  h->SetMaximum(maxLog);
  h->SetTitle(title.c_str());
  h->GetXaxis()->SetTitle(titleX.c_str());
  h->GetYaxis()->SetTitle(titleY.c_str());

  h->GetXaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);

  h->Draw();
  //h->SetFillColorAlpha(kBlue, 0.35);
  //h->SetFillStyle(3354);
  cStat->Print(Form("%s",outPdf.c_str()));

  // linear plot
  /*
  cStat->cd()->SetLogy(0);
  h->SetMinimum(minLin);
  h->SetMaximum(maxLin);
 // h->Draw();
 // cStat->Print(Form("%s",outPdf.c_str()));
  */
  cStat->Print(Form("%s]",outPdf.c_str()));
  printf("Histo in %s\n",outPdf.c_str());
  delete cStat;
  delete h;
  return 0;
}



//------------------------------------------------------------------------//
int TNA62richPixel::Draw() {
//------------------------------------------------------------------------//

  gStyle->SetOptStat(0);
  gStyle->SetStripDecimals(kFALSE); // number of digits after the comma Zaxis
  //gStyle->SetPalette(53);

  // apply on both single disk view
  double rmargin = 0.14; //right margin to make Z axis readble
  double lmargin = 0.16; //left margin to make Y axis readble
  double bmargin = 0.10; //left margin to make X axis readble

  std::string outPdfA = Form("ele_%s.pdf",fWhat.c_str());   // eleview
  std::string outPdfB = Form("geo_%s.pdf",fWhat.c_str()); // geoview 2 disks
  std::string outPdfC = Form("geo_%s_jura.pdf",fWhat.c_str());// geoview JURA
  std::string outPdfD = Form("geo_%s_saleve.pdf",fWhat.c_str());//geoView SALEVE

  TCanvas * c = NULL;     //  ele display
  TCanvas * canv = NULL;  // geo display 2 disks
  TCanvas * cj = NULL;  // geo display 1 disk jura
  TCanvas * cs = NULL;  // geo display 1 disk saleve

  TH2F * h=NULL;
  int feb; // frontend board
  int fec; // frontend channel

  double min = 0;
  double max = 1;

 // user defined palette (for efficiency)
/*
 unsigned int ncolr =4;
 unsigned int ncolrEndPoints = 40;
 double stop[ncolr];
 double red[ncolr];
 double green[ncolr];
 double blue[ncolr];

 int i;

i= 0; stop[i]=0; red[i]= 255; green[i]=0; blue[i]= 0;
i= 1; stop[i]=10; red[i]= 0; green[i]=255; blue[i]= 0;
i= 2; stop[i]=50; red[i]= 0; green[i]=  0; blue[i]=255;
i= 3; stop[i]=100; red[i]= 0; green[i]=  255; blue[i]=255;


for(i=0;i<ncolr;i++)red[i]=red[i]/255.;
for(i=0;i<ncolr;i++)green[i]=green[i]/255.;
for(i=0;i<ncolr;i++)blue[i]=blue[i]/255.;
for(i=0;i<ncolr;i++)blue[i]=stop[i]/100.;

TColor::CreateGradientColorTable(ncolr,stop,red,green,blue,ncolrEndPoints);

for(i=0;i<ncolr;i++)printf("%2d %6.3lf %4.0lf %4.0lf %4.0lf\n",i,stop[i],red[i]*255,green[i]*255,blue[i]*255);
*/


  if(strcmp(fWhat.c_str(),"gain")==0){
    min = 0.1;
    max = 2.5;
  }else if(strcmp(fWhat.c_str(),"klumi")==0){
    min = 40;
    max = 110;
  }else if(strcmp(fWhat.c_str(),"efficiency")==0){
    min = 0.000;
    max = 0.165;

  }else if(strcmp(fWhat.c_str(),"pedmean")==0){
    min = 1847-5*15;  // 1700
    max = 1837+5*15;   // 2000
  }else if(strcmp(fWhat.c_str(),"pedrms")==0){
    min = 32-5*5.5;
    max = 32+5*5.5;
  }

  h = new TH2F("h","",32,0,32,64,0,64);
  h->SetMinimum(min);
  h->SetMaximum(max);

  TNA62richGeo * g    = new TNA62richGeo(2);
  TNA62richGeo * jura = new TNA62richGeo(1,0);  //JURA
  TNA62richGeo * sale = new TNA62richGeo(1,1);  //SALEVE

  g->SetMinimum(min);
  g->SetMaximum(max);

  jura->SetMinimum(min);
  jura->SetMaximum(max);

  sale->SetMinimum(min);
  sale->SetMaximum(max);


  for(int echann=0;echann<MAXCH;echann++){
    if(!IsConnected(echann))continue;
    feb = GetFeBoardAbsolute(echann);
    fec = GetFeChannel(echann);

    g->Fill(echann,fVal[echann]);
    jura->Fill(echann,fVal[echann]);
    sale->Fill(echann,fVal[echann]);
    h->Fill(fec+0.5,feb+0.5,fVal[echann]);
  }

  // ELECTRONIC VIEW
  c = new TCanvas("c","RICH Monitor",400,500);
  c->Print(Form("%s[",outPdfA.c_str()));
 // c->cd()->SetLogz(1);
  c->cd()->SetLeftMargin(0.12);
  c->cd()->SetRightMargin(0.18);

  h->GetXaxis()->SetTitle("Channel ID [#]");
  h->GetYaxis()->SetTitle("Front End Board ID[#]");
  h->SetTitle(Form("Data Monitor RICH NA62 - %s",fWhat.c_str()));
  h->GetZaxis()->SetTitleOffset(1.8);
  h->GetXaxis()->CenterTitle();
  h->Draw("COLZ");
  c->Print(Form("%s",outPdfA.c_str()));
  h->SetMarkerSize(0.25);
  h->Draw("TEXT");
  c->Print(Form("%s",outPdfA.c_str()));
  c->Print(Form("%s]",outPdfA.c_str()));

  // DOUBLE DISK VIEW
  double w = 1000;  // to manage proportion of the canvas height and width
  canv = new TCanvas("canv","");
  canv->SetCanvasSize(w,w/2);
 // canv->cd()->SetLogz(1);
  canv->cd()->SetLeftMargin(0.10);  // forse si misura in percentuale 0.08 = 8%?
  canv->cd()->SetRightMargin(0.11);
  canv->cd()->SetBottomMargin(0.09);

  g->Draw();
  canv->Print(Form("%s",outPdfB.c_str()));
 // g->Dump();

  // SINGLE JURA
  cj = new TCanvas("cj","");
  cj->SetCanvasSize(w,0.85*w);
  cj->cd()->SetLeftMargin(lmargin);
  cj->cd()->SetRightMargin(rmargin);
  cj->cd()->SetBottomMargin(bmargin);
  jura->Draw();
  cj->Print(Form("%s",outPdfC.c_str()));

  // SINGLE SALEVE
  cs = new TCanvas("cs","");
  cs->SetCanvasSize(w,0.85*w);
  cs->cd()->SetLeftMargin(lmargin);
  cs->cd()->SetRightMargin(rmargin);
  cs->cd()->SetBottomMargin(bmargin);
  sale->Draw();
  cs->Print(Form("%s",outPdfD.c_str()));


  delete jura;
  delete sale;
  delete g;
  delete h;
  delete c;
  delete canv;
  delete cj;
  delete cs;
  printf("Display ready in %s\n",outPdfA.c_str());
  printf("Display ready in %s\n",outPdfB.c_str());
  printf("Display ready in %s\n",outPdfC.c_str());
  printf("Display ready in %s\n",outPdfD.c_str());
  return 0;
}
