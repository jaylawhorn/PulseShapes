#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TMath.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>


#endif

void getLandauFrac(float fC, int offset, float &sum, float minCharge_[58], float maxCharge_[58], float pulseFrac_[58][10]) {

  int chargeBin = -1;
  for (int i=0; i<58; i++) {
    if (fC>minCharge_[i] && fC<maxCharge_[i]) chargeBin=i;
  }
  if (fC>maxCharge_[57]) chargeBin=57;

  if (chargeBin==-1) chargeBin=0;

  sum=pulseFrac_[chargeBin][offset+3];
  return;

}

void doNegEnergyCorr(TString infile="root://eoscms//store/group/dpg_hcal/comm_hcal/RecoAlgos/Summer16Method2Update/HcalNtuple/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/Samples/160713_130517/0000/HCALTree_99.root",
		     //TString inname="pulse_shape_M3_HPD.csv") {
  //TString infile="root://eoscms//store/group/dpg_hcal/comm_hcal/RecoAlgos/Summer16Method2Update/HcalNtuple/HighPtJet80/Samples/160522_083815/0000/HCALTree_97.root",
		     //TString inname="DATA.csv") {
		     //TString infile="root://eoscms//store/group/dpg_hcal/comm_hcal/RecoAlgos/Summer16Method2Update/HcalNtuple/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/Samples/160713_130517/0000/HCALTree_99.root",
		     TString inname="MC.csv") {

  float minCharge_[58];
  float maxCharge_[58];
  float pulseFrac_[58][10];
  float pulseFracDeriv_[58][10];
  float timeSlew_[58];

  std::ifstream ifs;
  ifs.open(inname.Data());
  assert(ifs.is_open());
  std::string line;

  int i = 0;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    
    std::string tmpStr;
    std::stringstream ss(line);
    ss >> tmpStr;
    minCharge_[i] = std::atoi(tmpStr.c_str());
    ss >> tmpStr;
    maxCharge_[i] = std::atoi(tmpStr.c_str());
    for (int k=0; k<10; k++) { ss >> tmpStr; pulseFrac_[i][k] = std::atof(tmpStr.c_str()); }
    for (int k=0; k<10; k++) { ss >> tmpStr; pulseFracDeriv_[i][k] = std::atof(tmpStr.c_str()); }

    timeSlew_[i] = pulseFrac_[i][5]/pulseFrac_[i][4];
    //cout << i << ", " << timeSlew_[i] << "; ";

    i++;
  }
  cout << endl;

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

  TH2D * hComp = new TH2D("hComp","",50, -10, 40, 50, -10, 40);

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
  //for (UInt_t i=0; i<10; i++) {
    chain->GetEntry(i);
    
    for (int j = 0; j < (int)PulseCount; j++) {
      if (abs(IEta[j])>27 || ( abs(IEta[j])>14 && abs(IEta[j])<19)) continue;
      if(Charge[j][0]==Charge[j][4] && Charge[j][4]==Charge[j][5]) continue;
      //if (Charge[j][4]+Charge[j][5]<20) continue;
      
      //Double_t sumQ=0, ped=0;
      //for (UInt_t k=0; k<3; k++) { ped+=Charge[j][k];}
      //for (UInt_t k=0; k<10; k++) { sumQ+=Charge[j][k];}

      float i3=0;
      getLandauFrac(Charge[j][3], 1, i3, minCharge_, maxCharge_, pulseFrac_);
      float n3=0;
      getLandauFrac(Charge[j][3], 2, n3, minCharge_, maxCharge_, pulseFrac_);
      float nn3=0;
      getLandauFrac(Charge[j][3], 3, nn3, minCharge_, maxCharge_, pulseFrac_);

      float i4=0;
      getLandauFrac(Charge[j][4], 1, i4, minCharge_, maxCharge_, pulseFrac_);
      float n4=0;
      getLandauFrac(Charge[j][4], 2, n4, minCharge_, maxCharge_, pulseFrac_);

      float i5=0;
      getLandauFrac(Charge[j][5], 1, i5, minCharge_, maxCharge_, pulseFrac_);
      float n5=0;
      getLandauFrac(Charge[j][5], 2, n5, minCharge_, maxCharge_, pulseFrac_);

      float ch3=Charge[j][3]/i3;
      float ch4=(i3*Charge[j][4]-n3*Charge[j][3])/(i3*i4);
      float ch5=(n3*n4*Charge[j][3]-i4*nn3*Charge[j][3]-i3*n4*Charge[j][4]+i3*i4*Charge[j][5])/(i3*i4*i5);

      if (ch3<-3) {
	ch3 = -3;
	ch4 = Charge[j][4]/i4;
	ch5 = (i4*Charge[j][5]-n4*Charge[j][4])/(i4*i5);
      }

      if (ch5<-3) {

	float i4_final;
	getLandauFrac(Charge[j][4], 1, i4_final, minCharge_, maxCharge_, pulseFrac_);
	float n4_final;
	getLandauFrac(Charge[j][4], 2, n4_final, minCharge_, maxCharge_, pulseFrac_);

	float newTS = (Charge[j][5] + 3*i5)/(Charge[j][4] - i3*ch3);
	int newBin=0;
	for (int k=0; k<58; k++) {
	  if (newTS < timeSlew_[k]) newBin=k;
	}
	float i4_new = pulseFrac_[newBin][4];
	float ch4_new = (Charge[j][4]-ch3*n3)/i4_new;

	ch5 = -3;
	ch4 = ch4_new;

	//hComp->Fill(Charge[j][4]+Charge[j][5], ch4_new);
      }

      hComp->Fill(Charge[j][4]+Charge[j][5], ch4);
      
    }
    
  }

  hComp->Draw("colz");

}
