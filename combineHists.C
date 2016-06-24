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

void combineHists(//TString infile="blah2.root",
		  TString infile="/afs/cern.ch/work/j/jlawhorn/HighPtJet80/HCALTree_all.root",
		  //TString infile="/afs/cern.ch/work/j/jlawhorn/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/HCALTree_all.root",
		  //TString outfile="HCALTree_mc_all.root") {
		  TString outfile="HCALTree_test.root") {
  
  float binsize=10;
  float binstart=20;

  TChain *chain = new TChain("Averages");
  chain->AddFile(infile);

  //chain->GetEntry(0);
  const int nbin=58;

  Int_t nPulses[nbin];
  Double_t avgPulse[nbin][10], avgPulse2[nbin][10];//, adcUnc[nbin][10];
  //Double_t ucut[nbin], lcut[nbin];

  chain->SetBranchAddress("nPulses",   &nPulses   );
  //chain->SetBranchAddress("ucut",      &ucut      );
  //chain->SetBranchAddress("lcut",      &lcut      );
  chain->SetBranchAddress("avgPulse",  &avgPulse  );
  chain->SetBranchAddress("avgPulse2", &avgPulse2 );
  //chain->SetBranchAddress("adcUnc",    &adcUnc    );
  
  Int_t sumNPulses[nbin];  
  Double_t sumAvgPulse[nbin][10], sumAvgPulse2[nbin][10];//, sumAdcUnc[nbin][10];//, sumTotUnc[nbin][10]; 

  for (UInt_t i=0; i<nbin; i++) {
    sumNPulses[i]=0;
    for (UInt_t j=0; j<10; j++) {sumAvgPulse[i][j]=0; sumAvgPulse2[i][j]=0; /*sumAdcUnc[i][j]=0; totUnc[i][j]=0;*/}
  }

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
    chain->GetEntry(i);
    //cout << "i " << i << endl;
    //cout << sumAvgPulse[0][0] << endl;
    
    for (UInt_t j=0; j<nbin; j++) {
      //cout << sumAvgPulse[0][0] << endl;
      sumNPulses[j]+=nPulses[j];
      //cout << "j " << j << endl;
      //cout << sumAvgPulse[0][0] << endl;
      //if (i==0 && j==0) sumAvgPulse[0][0]=0;
      for (UInt_t k=0; k<10; k++) { 
      //cout << j << ", " << k << ", " << avgPulse[j][k] << ", " <<  avgPulse2[j][k] << ", " << adcUnc[j][k] << ", " << sumAvgPulse[j][k] << ", " << sumAvgPulse2[i][j] << ", " << sumAdcUnc[i][j] << endl;
	//cout << "k " << k << endl;
	//cout << j << ", " << k << ", " << avgPulse[j][k] << ", " << sumAvgPulse[j][k] << ", " << sumAvgPulse[0][0] << endl;
	//if (k==0 && j==0) cout << j << ", " << k << ", " << sumAvgPulse[0][0] << endl;
	sumAvgPulse[j][k]+=avgPulse[j][k];
	//if (k==0 && j==0) cout << j << ", " << k << ", " << sumAvgPulse[0][0] << endl;
	//if (k==0)cout << j << ", " << k << ", " << sumAvgPulse[0][0] << endl;
	sumAvgPulse2[j][k]+=avgPulse2[j][k];
	//cout << sumAvgPulse2[j][k] << endl;
	//sumAdcUnc[j][k]+=adcUnc[j][k];
      }
      
    }
  }
  
  //cout << sumAvgPulse[0][0] << endl;

  TFile *out = new TFile("thegraphs2.root", "recreate");
  vector<TGraphErrors*> gr;

  for (UInt_t i=0; i<nbin; i++) {
    if (i<2) cout << endl;
    if (i<2) cout << " iiiiiiiiii = " << i << endl;
    //cout << lcut[i] << ", " << ucut[i] << endl;
    char gname[50];
    sprintf(gname, "q_%.0f_%.0f", 20.+10*i, 30.+10*i);
    gr.push_back(new TGraphErrors(10));
    gr[i]->SetName(gname);

    double y0=sumAvgPulse[i][0]/sumNPulses[i];
    for (UInt_t j=0; j<10; j++) {
      double x, y, dy;
      x=j;
      //y=(sumAvgPulse[i][j]/sumNPulses[i]-y0)/(1.0-10*y0);
      y=(sumAvgPulse[i][j]/sumNPulses[i]);
      //cout << sumAvgPulse2[i][j] << ", " << sumAvgPulse[i][j]*sumAvgPulse[i][j]/sumNPulses[i] << endl;
      dy=TMath::Max(sumAvgPulse2[i][j]-sumAvgPulse[i][j]*sumAvgPulse[i][j]/sumNPulses[i], 0.0);
      dy=sqrt(dy/sumNPulses[i]);
      //if (i<2)cout << j << ", " << y << ", " << nPulses[i] << endl;
      if (i<2)cout << j << ", " << x << ", " << y << ", " << dy << endl;

      gr[i]->SetPoint(j,x,y);
      gr[i]->SetPointError(j,0,dy);
    }

    gr[i]->Write();
  
  }

  out->Write();
  out->Close();

}
