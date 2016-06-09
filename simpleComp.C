#include "TROOT.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

#include "HcalPulseShapes.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

using namespace std;

//static const double tzero[3]= {23.960177, 13.307784, 9.109694};
//static const double slope[3] = {-3.178648,  -1.556668, -1.075824 };
//static const double tmax[3] = {16.00, 10.00, 6.25 };
//
//double delay(double fC, BiasSetting bias) {
//  double rawDelay=tzero[bias]+slope[bias]*log(fC);
//  return (rawDelay<0)?(0):((rawDelay>tmax[bias])?(tmax[bias]):(rawDelay));   
//}

class PulseSimple 
{
public:
  PulseSimple() {
    ts1  = 8.0;
    ts2  = 19.0;
    ts3  = 29.3; 
    tpre = 9.0; 
    //thpd = 4.0;
    wd1  = 2.0; 
    wd2  = 0.7; 
    wd3  = 0.32;    
  }

  void setDataPulse(TGraphErrors gr) {
    for (Int_t i=0; i<10; i++) { 
      double x,y;
      gr.GetPoint(i,x,y);
      dataPulse[i]=y;
      dataErr[i]=gr.GetErrorY(i);
    }
  }

  void setTimeSlew(double qavg) {
    // !!!! gonna have a bad time if you call this before setDataPulse, should fix!!!!!
    double rawtslew=13.307784-1.556668*log(dataPulse[4]*qavg);
    tslew=(rawtslew<0)?(0):((rawtslew>10)?(10):(rawtslew));
    cout << qavg << ", " << tslew << endl;
    //tslew=0;
  }

  double operator()(const double *t) {
    double chi2=0;
    for (Int_t i=0; i<10; i++) {
      double tmpX=i, tmpT=t[0];
      chi2+=( evalPulse(&tmpX, &tmpT) - dataPulse[i] )*( evalPulse(&tmpX, &tmpT) - dataPulse[i] )/( dataErr[i]*dataErr[i] );
    }
    return chi2;
  }

  double evalPulse(double *x, double *t) {
    double ans=0;
    
    double thpd=t[0];
    HcalPulseShapes thePulses_;
    
    thePulses_.computeHPDShape(ts1,ts2,ts3,thpd,tpre,wd1,wd2,wd3, thePulses_.hpdShape_);
    for (UInt_t j=0; j<25; j++) {
      ans+=thePulses_.hpdShape_(25*floor(x[0]+0.5)+j-92.5-tslew);
    }
    
    return ans;
  }

private:
  float ts1, ts2, ts3, tpre, wd1, wd2, wd3;
  float tslew;
  double dataPulse[10];
  double dataErr[10];
};


void PrintPulse(float thpd);
void DrawChi2();

void simpleComp() {
  
  char gname[50];

  TFile *infile = new TFile("thegraphs2.root");
  TGraphErrors *g;

  PulseSimple pulse = PulseSimple();

  vector<TGraph*> gdiff;
  vector<TGraph*> gdata;
  vector<TGraph*> gtemp;
  for (Int_t i=0; i<10; i++) { 
    gdiff.push_back(new TGraph(0)); 
  }
  
  for (UInt_t i=0; i<58; i++) {
    gdata.push_back(new TGraph(0));
    gtemp.push_back(new TGraph(0));
    
    sprintf(gname, "q_%i_%i",20+10*i,30+10*i);
    g = (TGraphErrors*) infile->Get(gname);
    
    pulse.setDataPulse(*g);
    pulse.setTimeSlew(25+10*i);
    
    double tmpT=4.0; // default in CMSSW
    for (UInt_t j=0; j<10; j++) {
      double tmpX=j;
      double x,y;
      g->GetPoint(j,x,y);
      //gdata[i]->SetPoint(j,j,y);
      //gtemp[i]->SetPoint(j,j,pulse.evalPulse(&tmpX, &tmpT));
      gdiff[j]->SetPoint(i,25+10*i,y-pulse.evalPulse(&tmpX, &tmpT));
      //gdiff[j]->SetPoint(i,25+10*i,pulse.evalPulse(&tmpX, &tmpT));
      //gdiff[j]->SetPoint(i,25+10*i,y);
    }
  }

  /* TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  char pname[50];
  char title[50];
  for (UInt_t i=0; i<10; i++) {
    sprintf(pname,"TS%i_diff_noslew.png",i);
    sprintf(title,"TS%i",i);
    
    gdiff[i]->SetMarkerStyle(20);
    gdiff[i]->GetYaxis()->SetTitle("Data-Template");
    gdiff[i]->GetXaxis()->SetTitle("Q");
    gdiff[i]->SetTitle(title);
    gdiff[i]->Draw("ap");
    
    //c1->SaveAs(pname);
    }*/

  //for (UInt_t i=0; i<58; i++) {
  //  sprintf(pname, "bin_%i.png", i);
  //  sprintf(title,"%i < Q_{10} < %i",20+10*i, 30+10*i);
  //
  //  gdata[i]->SetMarkerStyle(20);
  //  gdata[i]->SetLineWidth(3);
  //  gtemp[i]->SetMarkerStyle(20);
  //  gtemp[i]->SetMarkerColor(kRed);
  //  gtemp[i]->SetLineColor(kRed);
  //  gtemp[i]->SetLineWidth(3);
  //
  //  gdata[i]->GetYaxis()->SetTitle("Fractional Containment");
  //  gdata[i]->GetXaxis()->SetTitle("Time Slice");
  //  gdata[i]->SetTitle(title);
  //  gdata[i]->GetYaxis()->SetRangeUser(-0.01,1.1);
  //  gdata[i]->Draw("apl");
  //  gtemp[i]->Draw("same pl");
  //
  //  c1->SaveAs(pname);
  //
  //}

}

void DrawChi2() {

  UInt_t i=10;
  char gname[50];
  sprintf(gname, "graph_%i",i);

  TFile *infile = new TFile("test_hist.root");
  TGraphErrors *g = (TGraphErrors*) infile->Get(gname);

  PulseSimple pulse = PulseSimple();
  pulse.setDataPulse(*g);
  pulse.setTimeSlew(25*i+12.5);

  TGraph *gchi2 = new TGraph(100);

  for (UInt_t i=0; i<100; i++) {
    Double_t t=0.1*i;
    gchi2->SetPoint(i, t, pulse(&t));
    cout << t << ", " << pulse(&t) << endl;
  }

  gchi2->Draw("apl");


}

void PrintPulse(float thpd) {

  float ts1, ts2, ts3, tpre, wd1, wd2, wd3;
  ts1=8. ; ts2=19. ; ts3=29.3; tpre=9.0; wd1=2.0; wd2=0.7; wd3=0.32;
  HcalPulseShapes thePulses_;
  
  Double_t pulse[10];

  //for (Int_t k=0; k<1; k++) {

  //thpd=1.0+k*0.5;

  for (UInt_t i=0; i<10; i++) {pulse[i]=0;}
  thePulses_.computeHPDShape(ts1,ts2,ts3,thpd,tpre,wd1,wd2,wd3, thePulses_.hpdShape_);
  for (UInt_t i=0; i<10; i++) {
    for (UInt_t j=0; j<25; j++) {
      pulse[i]+=thePulses_.hpdShape_(25*i+j-92.5);
    }
  }
  
  for (UInt_t i=0; i<10; i++) {
    cout << "pulse[" << i << "]= " << pulse[i] << endl;
  }
  
}
