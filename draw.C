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

  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_1.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_10.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_100.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_101.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_102.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_103.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_104.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_105.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_106.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_107.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_108.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_109.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_11.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_110.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_111.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_112.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_113.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_114.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_115.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_116.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_117.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_118.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_119.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_12.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_120.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_121.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_122.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_123.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_124.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_125.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_126.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_127.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_128.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_129.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_13.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_130.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_131.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_132.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_133.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_134.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_135.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_136.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_137.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_138.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_139.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_14.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_140.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_141.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_142.root");
  chain->AddFile("/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_143.root");
  
  cout << chain->GetEntries() << endl;

  Double_t pulse[10], ped[10];

  chain->SetBranchAddress("pulse", &pulse);
  chain->SetBranchAddress("ped", &ped);

  vector<Double_t> ucut;
  vector<Double_t> lcut;

  const int nbin=11;

  for (int i=0; i<nbin; i++) {
    lcut.push_back(25+i*25);
    ucut.push_back(50+i*25);
  }
  
  Double_t avgPulse[nbin][10], avgPulse2[nbin][10], adcUnc[nbin][10], totUnc[nbin][10]; 
  for (UInt_t i=0; i<nbin; i++) {
    for (UInt_t j=0; j<10; j++) {avgPulse[i][j]=0; avgPulse2[i][j]=0; adcUnc[i][j]=0; totUnc[i][j]=0;}
  }

  Int_t nPulses[nbin]={0};

  TFile *outf = new TFile("test_hist.root", "recreate");
  TH1D *hQ = new TH1D("hq", "hq",50, 0, 400);

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
  //for (UInt_t i=0; i<1000; i++) {
    chain->GetEntry(i);

    Double_t sumQ=0;
    for (UInt_t j=0; j<10; j++) { sumQ+=pulse[j];}

    hQ->Fill(sumQ);

    if (sumQ<25 || sumQ>300) continue;
    Int_t ibin= floor( sumQ/25-1 );
    //cout << sumQ << ", " << ibin << endl;

    nPulses[ibin]++;

    Double_t tmpUnc[10];
    Double_t uncNorm=0;

    for (UInt_t j=0; j<10; j++) { 
      tmpUnc[j]=sqrt(sigma(pulse[j])*sigma(pulse[j])+1);
      uncNorm+=tmpUnc[j];
    }

    for (UInt_t j=0; j<10; j++) { 
      tmpUnc[j]/=uncNorm;

      avgPulse[ibin][j]+=pulse[j]/sumQ;
      avgPulse2[ibin][j]+=pulse[j]*pulse[j]/sumQ;
      adcUnc[ibin][j]+=tmpUnc[j]*tmpUnc[j];
    }

  }

  //hQ->Draw("hist");

  //TH1D *h = new TH1D("h1", "h1", 10, -0.5, 9.5);
  vector<TGraphErrors*> grs;
  for (UInt_t i=0; i<nbin; i++) {
    char gname[50];
    sprintf(gname,"graph_%i",i);
    grs.push_back(new TGraphErrors(0));
    grs[i]->SetName(gname);

    cout << lcut[i] << " < Q_10 < " << ucut[i] << ": " << nPulses[i] << endl;

    for (UInt_t j=0; j<10; j++) {
      avgPulse[i][j]/=nPulses[i];
      avgPulse2[i][j]/=nPulses[i];
      adcUnc[i][j]=sqrt(adcUnc[i][j])/nPulses[i];
      totUnc[i][j]=sqrt(adcUnc[i][j] + avgPulse2[i][j] - avgPulse[i][j]*avgPulse[i][j])/sqrt(nPulses[i]);

      //cout << "avgPulse[" << j << "] = " << avgPulse[j] << " +/- " << totUnc[j] << endl;

      grs[i]->SetPoint(j, j, avgPulse[i][j]);
      grs[i]->SetPointError(j, 0, totUnc[i][j]);
    }
    
    grs[i]->Write();
  }

  outf->Write();
  outf->Close();


}
