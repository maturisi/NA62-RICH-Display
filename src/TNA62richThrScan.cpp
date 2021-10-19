#include "TNA62richThrScan.h"
#include "TNA62richMap.h"


#include <stdio.h>


#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TError.h>
#include <TH2F.h>
#include <TLine.h>


#include <string>      /* std::string */
#include <sys/stat.h> /* stat */
// -------------------------------------- //
bool exists(const std::string& name) {
// -------------------------------------- //
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}



//------------------------------------------------------------------------//
TNA62richThrScan::TNA62richThrScan() {
//------------------------------------------------------------------------//

  this->Reset();
  fMap = new TNA62richMap(); // put before CreateHisto if you want fancy titles
  this->CreateHisto();
  gErrorIgnoreLevel=kWarning;
  fName="Unknown";
  fDuration = 0;


}


//------------------------------------------------------------------------//
TNA62richThrScan::~TNA62richThrScan(){
//------------------------------------------------------------------------//

  printf("Cleanup\n");
  for(int i=0;i<MAXCH;i++) if(fh[i]) delete fh[i];
  if(fMap) delete fMap;
}


//------------------------------------------------------------------------//
int TNA62richThrScan::Reset() {
//------------------------------------------------------------------------//

  printf("Reset TNA62richThrScan object\n");
  for(int i=0;i<MAXCH;i++){
    fped[i]=0.0;
    for(int j=0;j<NTH;j++){
      fcnt[i][j]=-1.;
    }
  }
  for(int i=0;i<NCHIP;i++){
    fpedchip[i]= 0;
    fthrchip[i]=1500;
  }
  return 0;
}


//------------------------------------------------------------------------//
void TNA62richThrScan::SetName(const char * name) {
//------------------------------------------------------------------------//

  if(strcmp(name,"ped")==0){
    fName="ped";
  }else if(strcmp(name,"dark")==0){
    fName="dark";
  }else{
    printf("Error: Unknown name %s (please use \"ped\" or \"dark\")\n",name);
  }
  return;
}


//------------------------------------------------------------------------//
void TNA62richThrScan::SetDaqDuration(int value_in_seconds) {
//------------------------------------------------------------------------//

  if(value_in_seconds<=0){
    printf("Error: Daq Duration must be a positive number, you have %d\n", value_in_seconds);
    return;
  }
  fDuration = value_in_seconds;
  return;
}


//------------------------------------------------------------------------//
std::string TNA62richThrScan::GetDescriptionString(int echannel){
//------------------------------------------------------------------------//

  int i       = echannel;
//  int pixid   = fMap->GetPixelID(i);
//  int seqid   = fMap->GetSequentialID(i);
//  int sc      = fMap->GetSuperCellID(i);
//  int pmt     = fMap->GetZerotoSevenID(i);
  int sector  = GetSector(i);
  int feboard = GetFeBoard(i);
  int nino    = GetAsic(i);
  int ninoc   = GetAsicChannel(i); // nino channel

  std::string serial = fMap->GetSerialNumber(i);
  std::string frontend;
  std::string translate;

  frontend= Form("Sector %d Board %2d Nino %d Ch %d",sector,feboard,nino,ninoc);
  //translate = Form("PIXid %d SEQid %d SCid %2d_%d",pixid,seqid,sc,pmt);
  //translate = Form("SEQid %d SCid",seqid,sc);

  translate = Form("ELE %d ",echannel);

 // translate = "";
  std::string result;
 // result = Form("%s %s SERIAL %s",frontend.c_str(),translate.c_str(),serial.c_str());

  result = Form("%s %s",frontend.c_str(),translate.c_str());
  return result;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::CreateHisto() {
//------------------------------------------------------------------------//

  double step = 5.0;
  double sup = 2500;
  double inf = -2500;
  int bin = (sup-inf)/step;

  for(int i=0;i<MAXCH;i++){
    fh[i] = new TH1F(Form("h%04d",i),"",bin,inf,sup);
    fh[i]->GetXaxis()->SetTitle("Threshold [mV]");
    fh[i]->GetYaxis()->SetTitle("Counting rate [cps]");

    fh[i]->SetMinimum(0.1);
    fh[i]->SetMaximum(1e8);
    fh[i]->GetXaxis()->SetRangeUser(500.,2500.);

    if(!IsConnected(i))continue;

    fh[i]->SetTitle((this->GetDescriptionString(i)).c_str());
  }
  return 0;
}


//------------------------------------------------------------------------//
int TNA62richThrScan::Dump() {
//------------------------------------------------------------------------//
  printf("Dump\n");
  for(int ch=0;ch<MAXCH;ch++)
    for(int thr=0;thr<NTH;thr++)
      printf("CH %4d THR %4d CNT %4.0lf\n",ch,thr,fcnt[ch][thr]);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::Fill() {
//------------------------------------------------------------------------//

  printf("Fill\n");
  if(fDuration<=0){
    printf("Error:Bad daq duration value (%lf) Assuming 1 second\n ",fDuration);
    fDuration = 1;
  }
  double val;
  for(int i=0;i<MAXCH;i++){
    for(int j=0;j<NTH;j++){
      val = fcnt[i][j];
      if(val>=0) fh[i]->Fill(j,val/fDuration);
    }
  }
  return 0;
}



//------------------------------------------------------------------------//
int TNA62richThrScan::WriteTxt() {
//------------------------------------------------------------------------//

  double mean;
  double rms;
  double rms_expected_val = 33; // from pedestal study on Dec 2019 data
  double rms_expected_err = 3; // from pedestal study on Dec 2019 data (gaussian fit on rms data of JUP)

  double lmin = rms_expected_val - 3 * rms_expected_err;
  double lmax = rms_expected_val + 3 * rms_expected_err;

  printf("Write Txt\n");
  const char * outNamePed  = "pedestal.txt" ;
  const char * outNameHot  = "pedestal_hot.txt" ;
  const char * outNameCold = "pedestal_cold.txt" ;

  FILE * outTxtPed  = fopen(outNamePed,"w");
  FILE * outTxtHot  = fopen(outNameHot,"w");
  FILE * outTxtCold = fopen(outNameCold,"w");

  for(int i=0;i<MAXCH;i++){
    mean    = fh[i]->GetMean();
    rms     = fh[i]->GetRMS();
    fprintf(outTxtPed,"%4d ",i);
    fprintf(outTxtPed,"%6.0lf ",mean);
    fprintf(outTxtPed,"%6.0lf ",rms);
    fprintf(outTxtPed,"\n");

    if(!IsConnected(i))continue;
    if(rms>(lmax)){
      fprintf(outTxtHot,"CH %4d ",i);
      fprintf(outTxtHot,"%s",GetDescriptionString(i).c_str());
      fprintf(outTxtHot,"MEAN %6.0lf ",mean);
      fprintf(outTxtHot,"RMS %6.0lf ",rms);
      fprintf(outTxtHot,"\n");
    }

    if(rms<(lmin)){
      fprintf(outTxtCold,"CH %4d ",i);
      fprintf(outTxtCold,"%s",GetDescriptionString(i).c_str());
      fprintf(outTxtCold,"MEAN %6.0lf ",mean);
      fprintf(outTxtCold,"RMS %6.0lf ",rms);
      fprintf(outTxtCold,"\n");
    }
  }
  fclose(outTxtPed);
  fclose(outTxtHot);
  fclose(outTxtCold);
  printf("Pedestal ALL  in %s\n",outNamePed);
  printf("Pedestal HOT  in %s\n",outNameHot);
  printf("Pedestal COLD in %s\n",outNameCold);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::WriteRoot() {
//------------------------------------------------------------------------//

  std::string outRoot = Form("%s.root",fName.c_str());
  printf("Write Histo on %s \n",outRoot.c_str());

  TFile * f = new TFile(outRoot.c_str(),"RECREATE");
  for(int i=0;i<MAXCH;i++){
    fh[i]->Write(Form("%s%04d",fName.c_str(),i));
  }
  f->Close();
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::Plot() {
//------------------------------------------------------------------------//
  printf("Plot\n");

  std::string outPdf= Form("%s.pdf",fName.c_str());
  TCanvas * c = new TCanvas();
  c->cd()->SetLogy(1);

  c->Print(Form("%s[",outPdf.c_str()));
  double mean;
  for(int i=0;i<MAXCH;i++){
    mean = fh[i]->GetMean();
    if(mean>0){
      fh[i]->Draw("L HIST");
      c->Print(Form("%s",outPdf.c_str()));
    }
  }
  c->Print(Form("%s]",outPdf.c_str()));
  printf("Histograms in %s\n",outPdf.c_str());
 // this->DumpHisto(32);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::Plot2D() {
//------------------------------------------------------------------------//

  printf("Plot2D\n");

  double xmin = 0;
  double xmax = 2048;
  int xbin = xmax;

  double ymin = 500;
  double ymax = 2200;
  int step = 5; // in mV, must be identical to the step used in daq
  double ybin = (ymax-ymin)/step;

  TH2F * h2 = new TH2F("h2","",xbin,xmin,xmax,ybin,ymin,ymax);

  double val;
  for(int i=0;i<MAXCH;i++){
    for(int j=0;j<NTH;j++){
      val = fcnt[i][j];
      if(val>=0) h2->Fill(i,j,val/fDuration);
    }
  }
  double zmin = 1;
  double zmax = 1e8;

  std::string outPdf= Form("%s2D.pdf",fName.c_str());
  gStyle->SetOptStat(0);
  gStyle->SetPadRightMargin(0.15);
  TCanvas *c = new TCanvas();
  c->cd()->SetLogz(1);

  if (fName.compare("dark") == 0){
   zmax = 450;
   c->cd()->SetLogz(0);
   printf("zmax = %.0lf and linear z axis\n",zmax);
  }

  h2->SetMaximum(zmax);
  h2->SetMinimum(zmin);
  h2->SetTitle(fName.c_str());
  h2->GetXaxis()->SetTitle(Form("Frontend Channel [0..%.0lf]",xmax));
  h2->GetYaxis()->SetTitle("Threshold [mV]");
  h2->GetZaxis()->SetTitle("Counting rate [cps]");
  h2->GetXaxis()->CenterTitle();
  //h->GetYaxis()->CenterTitle();
  //h->GetZaxis()->CenterTitle();
  h2->Draw("COLZ");

  TLine * li = new TLine(xmin,1500,xmax,1500);
  li->SetLineWidth(2);
  li->Draw();

  this->ChipStatRead();  // here the THR recipe is read from external file.

  TLine * line[NCHIP];
  for(int i=0;i<NCHIP;i++){
    line[i] = new TLine(i*8,fthrchip[i],i*8+7,fthrchip[i]);
    line[i]->SetLineWidth(2);
    line[i]->SetLineColor(2);
    if(xmax<=512 && i>63) continue;
    line[i]->Draw();
  }
  c->Modified();
  c->cd();

  c->Print(outPdf.c_str());
  printf("Monitor ready in %s\n",outPdf.c_str());

  for(int i=0;i<NCHIP;i++){
    if(line[i])delete line[i];
  }
  delete li;
  delete h2;
  delete c;
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::DumpHisto(int ch) {
//------------------------------------------------------------------------//

  if(ch<0 || ch > MAXCH){
    printf("Error: channel out of range [0..%d]\n",ch);
    return -1;
  }

  int bins = fh[ch]->GetNbinsX();
  for(int i=0;i<bins;i++){
   printf("CH %4d BIN %4d ",ch,i);
   printf("CENTER %6.1lf ",fh[ch]->GetBinCenter(i));
   printf("CONTENT %.1lf \n",fh[ch]->GetBinContent(i));
  }
  return 0;
}



//------------------------------------------------------------------------//
int TNA62richThrScan::ReadFile(const char * filename) {
//------------------------------------------------------------------------//

  printf("Read File %s\n",filename);

  FILE * fin = fopen(filename,"r");
  if(!fin){printf("Error: file %s not found\n",filename); return -1;}
  int ch;
  int cnt;
  int thr;
  while(fscanf(fin,"%d %d %d\n",&ch,&thr,&cnt)!=EOF){
   // printf("%d %d %d \n",ch,thr,cnt);
    if(ch>MAXCH){
      printf("Error: channel out of range [0..%d]\n",MAXCH);
    }else if (thr<0 || thr > NTH){
      printf("Error: threshold out of range [0...%d]\n",NTH);
    }else if(fcnt[ch][thr]>0){
      printf("Error: cannot fill two times the same bin\n");
    }else {
      fcnt[ch][thr] = cnt;
    }
  }
  fclose(fin);
  return 0;
}


//------------------------------------------------------------------------//
int TNA62richThrScan::ChipStat() {
//------------------------------------------------------------------------//

  int p=0;
  int n_sigma = 6; // to determine NINO threshold position

  printf("ChipStat\n");
  const char * inFile= "pedestal.txt" ;
  const char * outFile = "chip.txt" ;

  FILE * inTxt = fopen(inFile,"r");
  FILE * outTxt = fopen(outFile,"w");

  int i;
  double val[2];

  // chip parameters are average pedestal and average rms of the 8 channels
  // the model can be refined eventully
  double avg_ped=0.0;
  double avg_rms=0.0;
  int thr;
  while(fscanf(inTxt,"%d %lf %lf\n",&i,val,val+1)!=EOF){

   // printf("CH %d MEAN %6.1lf RMS %6.1lf\n",i,val[0],val[1]);

    int sector = GetSector(i);
    int fe = GetFeBoard(i);
    int asic = GetAsic(i);
    int ch_asic = GetAsicChannel(i);
    int abs_asic = GetAsicAbsolute(i);

  // if(fe!=0) continue;
    //printf("CH_ELE %4d ",i); // as it comes from daq
    if(p)printf("SECTOR %d ",sector);
    if(p)printf("FE %3d ",fe);
    if(p)printf("ASIC %2d ",asic);
    if(p)printf("CH %2d ",ch_asic);
    // printf("ABS_ASIC %3d ",abs_asic);
    if(p)printf("PED %6.0lf ",val[0]);
    if(p)printf("RMS %6.0lf ",val[1]);

    if(ch_asic==7){
        avg_ped+=val[0];
        avg_rms+=val[1];
        avg_ped=avg_ped/8.;
        avg_rms=avg_rms/8.;
        thr= int (avg_ped - n_sigma*avg_rms);  // threshold is defined here!
        //if(thr<=0) thr=1500; // default choice
        if(thr<1500) thr=1500; // default choice
        fprintf(outTxt,"%4d %8.1lf %8.1lf %8d\n",abs_asic,avg_ped,avg_rms,thr); // if you change this, please change the reader too! ChipStatRead()
        avg_ped=0.0;
        avg_rms=0.0;
    }else{
      avg_ped+=val[0];
      avg_rms+=val[1];
    }

    if(p)printf("\n");
  }
  printf("Chip Statistics ready in %s\n",outFile);
  fclose(inTxt);
  fclose(outTxt);
  return 0;
}

//------------------------------------------------------------------------//
int TNA62richThrScan::ChipStatRead() {
//------------------------------------------------------------------------//

  printf("ChipStatRead\n");
  const char * inFile = "chip.txt" ;
  FILE * inTxt = fopen(inFile,"r");


  int asic;
  double mean,rms;
  int thr;

  while(fscanf(inTxt,"%d %lf %lf %d\n",&asic,&mean,&rms,&thr)!=EOF){
    //printf("%4d %8.1lf %8.1lf %8d\n",asic,mean,rms,thr);
    fthrchip[asic]=thr;
  }
  fclose(inTxt);
  return 0;
}


//------------------------------------------------------------------------//
int TNA62richThrScan::PlotSuperimpose(int logy) {
//------------------------------------------------------------------------//

  int isYLog=logy; // 0 LINEAR, 1 LOG

  std::string filePed = "ped.root";
  std::string fileDark = "dark.root";

  if(!exists(filePed.c_str())) return -1;
  if(!exists(fileDark.c_str())) return -2;

  TFile * f1 = new TFile(filePed.c_str(),"READ");
  TFile * f2 = new TFile(fileDark.c_str(),"READ");

  TH1F * hped;
  TH1F * hdark;

  double xmin=300;
  double xmax=2200;
  double ymin;
  double ymax;

  std::string outPdf;

  if(isYLog){
    ymin=0.1;
    ymax=1E8;
    outPdf = "superimpose_log.pdf";
  }else{
    ymin=1;
    ymax=450;
    outPdf = "superimpose_lin.pdf";
  }
  double mean;
  int thr;

  TCanvas * c = new TCanvas();
  c->Divide(4,2); // one ASIC per page
  c->Print(Form("%s[",outPdf.c_str()));

  TLine * lineTHR;
  TLine * linePED;

  for(int i=0;i<MAXCH;i++){
    if(!IsConnected(i))continue;

    hped = NULL;
    hdark = NULL;
    hped = (TH1F*) f1->Get(Form("ped%04d",i));
    if(!hped) continue;

    hped->SetMaximum(ymax);
    hped->SetMinimum(ymin);
    hped->GetXaxis()->SetRangeUser(xmin,xmax);

    hped->SetFillColorAlpha(kBlue, 0.35);
    hped->SetFillStyle(3354);

    c->cd(1+GetAsicChannel(i))->SetLogy(isYLog);
    //c->cd()->SetLogy(isYLog);
    hped->Draw("HIST");

    hdark = (TH1F*) f2->Get(Form("dark%04d",i));
    hdark->SetLineColor(kRed);
    hdark->Draw("HIST SAME");

    // LINE Threshold
    thr = fthrchip[GetAsicAbsolute(i)];
    lineTHR = new TLine(thr,ymin,thr,ymax);
    lineTHR->SetLineWidth(2);
    lineTHR->SetLineColor(kMagenta);

    //LINE Pedestal position (Averaged on chip! see ChipStat)
    mean = hped->GetMean();
    linePED = new TLine(mean,ymin,mean,ymax);
    linePED->SetLineColor(kBlack);

    //lineTHR->Draw();
    //linePED->Draw();

   // c->Print(Form("%s",outPdf.c_str()));
    delete lineTHR;
    delete linePED;
    if(GetAsicChannel(i)==7){
      //printf("ASIC %d\n",GetAsicAbsolute(i));
      c->Print(Form("%s",outPdf.c_str()));
    }
  }
  c->Print(Form("%s]",outPdf.c_str()));
  printf("Histograms in %s\n",outPdf.c_str());
  return 0;
}
