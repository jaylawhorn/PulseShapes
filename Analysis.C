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
    double rawtslew=13.307784-1.556668*log(dataPulse[4]);
    tslew=(rawtslew<0)?(0):((rawtslew>10)?(10):(rawtslew));
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

void Analysis(Int_t i=0) {

  char gname[50];
  char pname[50];

  sprintf(gname, "graph_%i",i);
  sprintf(pname, "bin_%i.png",i);

  TFile *infile = new TFile("test_hist.root");
  TGraphErrors *g = (TGraphErrors*) infile->Get(gname);

  ROOT::Math::Minimizer *min = ROOT::Math::Factory::CreateMinimizer("Minuit2","");
  min->SetMaxFunctionCalls(100000000);
  min->SetTolerance(100.0);
  min->SetPrintLevel(0);

  PulseSimple pulse = PulseSimple();
  pulse.setDataPulse(*g);

  double step[1] = {0.01};
  double variable[1] = {2.51};

  ROOT::Math::Functor f(pulse,1);
  
  min->SetFunction(f);
  min->SetVariable(0, "thpd", variable[0], step[0]);

  min->Minimize();

  cout << *min->X() << ", " << min->MinValue() << endl;

  char fval[100];
  sprintf(fval, "t_{hpd}=%.3f", *min->X());

  //cout << pulse(&t) << endl;

  TGraph *gfit = new TGraph(0);

  for (UInt_t i=0; i<10; i++) {
    double tmpX=i, tmpT=*min->X();
    gfit->SetPoint(i,i, pulse.evalPulse(&tmpX, &tmpT));
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  TLegend *l = new TLegend(0.75, 0.75, 0.9, 0.9);
  l->SetShadowColor(0); l->SetFillColor(0);

  TPaveText *pt = new TPaveText(0.15,0.8,0.3,0.89,"NDC");
  pt->SetShadowColor(0); pt->SetFillColor(0); pt->SetLineColor(0);
  pt->AddText(fval);

  g->SetMarkerColor(kBlack);
  g->SetMarkerStyle(20);
  //g->SetMarkerSize(20);
  g->SetLineColor(kBlack);

  l->AddEntry(g, "Data", "p");

  gfit->SetMarkerColor(kRed);
  gfit->SetMarkerStyle(20);
  gfit->SetLineColor(kRed);

  l->AddEntry(gfit, "Fit", "p");

  g->GetYaxis()->SetRangeUser(0,1);
  g->GetYaxis()->SetTitle("Normalized Pulse Shape");
  g->GetXaxis()->SetTitle("Time Slice");
  g->SetTitle("");
  g->Draw("ape");
  gfit->Draw("p same");

  l->Draw();
  pt->Draw();

  c1->SaveAs(pname);

  //PrintPulse(*min->X());

};

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
