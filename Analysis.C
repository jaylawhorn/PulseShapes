#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"

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
      ans+=thePulses_.hpdShape_(25*floor(x[0]+0.5)+j-92.5);
    }
    
    return ans;
  }

private:
  float ts1, ts2, ts3, tpre, wd1, wd2, wd3;
  double dataPulse[10];
  double dataErr[10];
};

void PrintPulse(float thpd);

void Analysis() {

  TFile *infile = new TFile("test_hist.root");
  TGraphErrors *g = (TGraphErrors*) infile->Get("gr");

  ROOT::Math::Minimizer *min = ROOT::Math::Factory::CreateMinimizer("Minuit2","");
  min->SetMaxFunctionCalls(10000000);
  min->SetTolerance(10.0);
  min->SetPrintLevel(0);

  PulseSimple pulse = PulseSimple();
  pulse.setDataPulse(*g);

  double step[1] = {0.1};
  double variable[1] = {4.0};

  ROOT::Math::Functor f(pulse,1);
  
  min->SetFunction(f);
  min->SetVariable(0, "thpd", variable[0], step[0]);

  min->Minimize();

  cout << *min->X() << ", " << min->MinValue() << endl;

  //cout << pulse(&t) << endl;

  TGraph *gfit = new TGraph(0);

  for (UInt_t i=0; i<10; i++) {
    double tmpX=i, tmpT=*min->X();
    gfit->SetPoint(i,i, pulse.evalPulse(&tmpX, &tmpT));
  }

  g->SetMarkerColor(kBlack);
  g->SetMarkerStyle(20);
  //g->SetMarkerSize(20);
  g->SetLineColor(kBlack);

  gfit->SetMarkerColor(kRed);
  gfit->SetMarkerStyle(20);
  gfit->SetLineColor(kRed);

  g->Draw("ape");
  gfit->Draw("p same");

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
