#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <TProfile.h>
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

void makeSummaryTree(TString infile="root://eoscms//store/group/dpg_hcal/comm_hcal/RecoAlgos/Summer16Method2Update/HcalNtuple/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/Samples/160713_130517/0000/HCALTree_99.root",
	    TString outfile="blah2.root") {


  TChain *chain = new TChain("ExportTree/HcalTree");
  chain->AddFile(infile);
  
  Int_t           PulseCount;
  Double_t        Charge[5184][10];
  Double_t        Pedestal[5184][10];
  Int_t           IEta[5184];
  Int_t           IPhi[5184];
  Int_t           Depth[5184];

  chain->SetBranchAddress("PulseCount", &PulseCount);
  chain->SetBranchAddress("Charge", Charge);
  chain->SetBranchAddress("Pedestal", Pedestal);
  chain->SetBranchAddress("IEta", IEta);
  chain->SetBranchAddress("IPhi", IPhi);
  chain->SetBranchAddress("Depth", Depth);

  TFile *outf = new TFile(outfile, "recreate");
  TTree *outt = new TTree("Values", "Values");

  double nTS3, nTS4, nTS5, nTS6;
  double TS3, TS4, TS5, TS6;
  double sumQ;
  double ped;
  
  outt->Branch("nTS3",  &nTS3,  "nTS3/D");
  outt->Branch("nTS4",  &nTS4,  "nTS4/D");
  outt->Branch("nTS5",  &nTS5,  "nTS5/D");
  outt->Branch("nTS6",  &nTS6,  "nTS6/D");

  outt->Branch("TS3",  &TS3,  "TS3/D");
  outt->Branch("TS4",  &TS4,  "TS4/D");
  outt->Branch("TS5",  &TS5,  "TS5/D");
  outt->Branch("TS6",  &TS6,  "TS6/D");

  outt->Branch("sumQ",  &sumQ,  "sumQ/D");
  outt->Branch("ped",   &ped,   "ped/D");

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
  //for (UInt_t i=0; i<10; i++) {
    chain->GetEntry(i);

    for (int j = 0; j < (int)PulseCount; j++) {
      if (abs(IEta[j])>27 || ( abs(IEta[j])>14 && abs(IEta[j])<19)) continue;
      if(Charge[j][0]==Charge[j][4] && Charge[j][4]==Charge[j][5]) continue;

      sumQ=0; ped=0;

      TS3=0; TS4=0; TS5=0; TS6=0;
      nTS3=0; nTS4=0; nTS5=0; nTS6=0;

      for (UInt_t k=0; k<3; k++) { ped+=Charge[j][k]+Pedestal[j][k];}
      for (UInt_t k=3; k<7; k++) { sumQ+=Charge[j][k]+Pedestal[j][k];}
      
      ped/=3;
      sumQ-=4*ped;

      TS3 = Charge[j][3]+Pedestal[j][3]-ped;
      TS4 = Charge[j][4]+Pedestal[j][4]-ped;
      TS5 = Charge[j][5]+Pedestal[j][5]-ped;
      TS6 = Charge[j][6]+Pedestal[j][6]-ped;
      
      if (sumQ<5 || TS4<5 || ped<0 || TS3<0 || TS5<0 || TS6<0) continue;

      nTS3 = TS3/sumQ;
      nTS4 = TS4/sumQ;

      if (nTS4>1) cout << TS3 << ", " << TS4 << ", " << TS5 << ", " << TS6 << " = " << sumQ << ", " << ped << endl;

      nTS5 = TS5/sumQ;
      nTS6 = TS6/sumQ;

      outt->Fill();

    }
  }


  outf->Write();
  outf->Close();

}
