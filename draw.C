#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <iostream>
#include <iomanip>

#endif

double sigma(double ifC) {
  if(ifC < 75) return (0.577 + 0.0686*ifC)/3.;
  return (2.75  + 0.0373*ifC + 3e-6*ifC*ifC)/3.;
}

void draw() {

  TChain *chain = new TChain("Events");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_3.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_30.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_31.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_32.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_33.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_34.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_35.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_36.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_37.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_38.root");
  //chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_39.root");

  cout << chain->GetEntries() << endl;

  Double_t pulse[10], ped[10];

  chain->SetBranchAddress("pulse", &pulse);
  chain->SetBranchAddress("ped", &ped);

  Double_t avgPulse[10], avgUnc[10]; 
  for (Int_t i=0; i<10; i++) {avgPulse[i]=0; avgUnc[i]=0;}

  Int_t nPulses=0;

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
    chain->GetEntry(i);

    Double_t sumQ=0;
    for (UInt_t j=0; j<10; j++) { sumQ+=(pulse[j]-ped[j]); }
    //if (sumQ<100 || sumQ>120) continue;
    if (sumQ<60 || sumQ>80) continue;
    //if (sumQ<200 || sumQ>250) continue;

    nPulses++;

    Double_t tmpUnc[10];
    Double_t uncNorm=0;

    for (UInt_t j=0; j<10; j++) { 
      tmpUnc[j]=sqrt(sigma(pulse[j]-ped[j])*sigma(pulse[j]-ped[j])+1);
      uncNorm+=tmpUnc[j];
    }

    for (UInt_t j=0; j<10; j++) { 
      tmpUnc[j]/=uncNorm;

      avgPulse[j]+=(pulse[j]-ped[j])/sumQ;
      avgUnc[j]+=tmpUnc[j]*tmpUnc[j];
    }

  }

  TFile *outf = new TFile("test_hist.root", "recreate");
  //TH1D *h = new TH1D("h1", "h1", 10, -0.5, 9.5);
  TGraphErrors *g = new TGraphErrors(0);

  cout << nPulses << endl;

  for (UInt_t j=0; j<10; j++) {
    avgPulse[j]/=nPulses;
    avgUnc[j]=sqrt(avgUnc[j])/nPulses;

    cout << "avgPulse[" << j << "] = " << avgPulse[j] << " +/- " << avgUnc[j] << endl;

    //h->SetBinContent(j+1, avgPulse[j]);
    g->SetPoint(j, j, avgPulse[j]);
    g->SetPointError(j, 0, avgUnc[j]);
  }

  g->SetName("gr"); g->Write();
  //cout << h->Integral() << endl;
  
  outf->Write();
  outf->Close();


  //vector<TH1D*> h;
  //vector<Double_t> ucut;
  //vector<Double_t> lcut;
  
  //for (int i=0; i<10; i++) {
  //char hname[50], cut[200];
  
  //sprintf(hname, "h%i", i);
  //h.push_back(new TH1D(hname, hname, 10,-0.5,9.5));
  //
  //lcut.push_back(25+i*25);
  //ucut.push_back(50+i*25);
  
  //sprintf(cut,"pulse*(m2Charge>%.1f && m2Charge<%.1f)",lcut[i],ucut[i]);
  //t->Draw(TString("TS>>")+TString(hname), cut);
  //h[i]->Scale(1.0/h[i]->Integral());

  //outf->Write();
  //outf->Close();

}
