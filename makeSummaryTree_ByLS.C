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

#include "tree.h"

#endif

void makeSummaryTree_ByLS(TString outfile="sortedByPhase.root") {


  TChain *chain = new TChain("hcalTupleTree/tree");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias10.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias1.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias2.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias3.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias4.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias5.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias6.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias7.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias8.root");
  chain->AddFile("root://eoscms//store/user/jlawhorn/HEP17_PHASE_SCAN/ZeroBias9.root");

  vector<vector<double> > *QIE11DigiFC=0;
  Int_t           laserType=0;
  vector<int>     *QIE11DigiCapIDError=0;
  vector<int>     *QIE11DigiDepth=0;
  vector<int>     *QIE11DigiFlags=0;
  vector<int>     *QIE11DigiIEta=0;
  vector<int>     *QIE11DigiIPhi=0;
  vector<int>     *QIE11DigiLinkError=0;
  vector<int>     *QIE11DigiRawID=0;
  vector<int>     *QIE11DigiSubdet=0;
  vector<vector<int> > *QIE11DigiADC=0;
  vector<vector<int> > *QIE11DigiCapID=0;
  vector<vector<int> > *QIE11DigiSOI=0;
  vector<vector<int> > *QIE11DigiTDC=0;
  UInt_t          bx=0;
  UInt_t          event=0;
  UInt_t          ls=0;
  UInt_t          orbit=0;
  UInt_t          run=0;

  chain->SetBranchAddress("QIE11DigiFC", &QIE11DigiFC);
  chain->SetBranchAddress("laserType", &laserType);
  chain->SetBranchAddress("QIE11DigiCapIDError", &QIE11DigiCapIDError);
  chain->SetBranchAddress("QIE11DigiDepth", &QIE11DigiDepth);
  chain->SetBranchAddress("QIE11DigiFlags", &QIE11DigiFlags);
  chain->SetBranchAddress("QIE11DigiIEta", &QIE11DigiIEta);
  chain->SetBranchAddress("QIE11DigiIPhi", &QIE11DigiIPhi);
  chain->SetBranchAddress("QIE11DigiLinkError", &QIE11DigiLinkError);
  chain->SetBranchAddress("QIE11DigiRawID", &QIE11DigiRawID);
  chain->SetBranchAddress("QIE11DigiSubdet", &QIE11DigiSubdet);
  chain->SetBranchAddress("QIE11DigiADC", &QIE11DigiADC);
  chain->SetBranchAddress("QIE11DigiCapID", &QIE11DigiCapID);
  chain->SetBranchAddress("QIE11DigiSOI", &QIE11DigiSOI);
  chain->SetBranchAddress("QIE11DigiTDC", &QIE11DigiTDC);
  chain->SetBranchAddress("bx", &bx);
  chain->SetBranchAddress("event", &event);
  chain->SetBranchAddress("ls", &ls);
  chain->SetBranchAddress("orbit", &orbit);
  chain->SetBranchAddress("run", &run);

  TFile *outf = new TFile(outfile,"recreate");

  vector<TProfile *> vec;
  vector<TH1D *> vecTDC;
  vector<TH1D *> vecM0T;
  TH2D *hTDCvM0T = new TH2D("hTDCvM0T","hTDCvM0T",150,4,7,120,-50,70);

  vector<uint> maxLS_36;
  vector<uint> minLS_36;
  vector<uint> maxLS_37;
  vector<uint> minLS_37;
  vector<uint> maxLS_39;
  vector<uint> minLS_39;
  vector<uint> maxLS_40;
  vector<uint> minLS_40;

  minLS_36.push_back(0);   maxLS_36.push_back(66);
  minLS_36.push_back(67);  maxLS_36.push_back(79);
  minLS_36.push_back(80);  maxLS_36.push_back(92);
  minLS_36.push_back(93);  maxLS_36.push_back(105);
  minLS_36.push_back(106); maxLS_36.push_back(1000);

  int n36=minLS_36.size();

  minLS_37.push_back(0);   maxLS_37.push_back(51);
  minLS_37.push_back(52);  maxLS_37.push_back(64);
  minLS_37.push_back(65);  maxLS_37.push_back(76);
  minLS_37.push_back(77);  maxLS_37.push_back(89);
  minLS_37.push_back(90);  maxLS_37.push_back(102);
  minLS_37.push_back(103); maxLS_37.push_back(115);
  minLS_37.push_back(116); maxLS_37.push_back(128);
  minLS_37.push_back(129); maxLS_37.push_back(141);
  minLS_37.push_back(142); maxLS_37.push_back(154);
  minLS_37.push_back(154); maxLS_37.push_back(167);
  minLS_37.push_back(168); maxLS_37.push_back(180);
  minLS_37.push_back(181); maxLS_37.push_back(193);
  minLS_37.push_back(194); maxLS_37.push_back(206);
  minLS_37.push_back(207); maxLS_37.push_back(219);
  minLS_37.push_back(220); maxLS_37.push_back(232);
  minLS_37.push_back(233); maxLS_37.push_back(245);
  minLS_37.push_back(246); maxLS_37.push_back(1000);

  int n37=minLS_37.size();

  minLS_39.push_back(4);  maxLS_39.push_back(16);
  minLS_39.push_back(17); maxLS_39.push_back(29);
  minLS_39.push_back(30); maxLS_39.push_back(42);
  minLS_39.push_back(43); maxLS_39.push_back(55);
  minLS_39.push_back(56); maxLS_39.push_back(68);
  minLS_39.push_back(69); maxLS_39.push_back(1000);

  int n39=minLS_39.size();

  minLS_40.push_back(0); maxLS_40.push_back(12);
  minLS_40.push_back(111); maxLS_40.push_back(118);
  minLS_40.push_back(119); maxLS_40.push_back(131);
  minLS_40.push_back(132); maxLS_40.push_back(144);
  minLS_40.push_back(145); maxLS_40.push_back(157);
  minLS_40.push_back(158); maxLS_40.push_back(170);
  minLS_40.push_back(171); maxLS_40.push_back(183);
  minLS_40.push_back(184); maxLS_40.push_back(196);
  minLS_40.push_back(197); maxLS_40.push_back(209);
  minLS_40.push_back(210); maxLS_40.push_back(222);
  minLS_40.push_back(223); maxLS_40.push_back(235);
  minLS_40.push_back(236); maxLS_40.push_back(248);
  minLS_40.push_back(249); maxLS_40.push_back(261);
  minLS_40.push_back(262); maxLS_40.push_back(274);
  minLS_40.push_back(275); maxLS_40.push_back(287);
  minLS_40.push_back(288); maxLS_40.push_back(300);
  minLS_40.push_back(301); maxLS_40.push_back(1000);

  int n40=minLS_40.size();

  for (int i=0; i<n36+n37+n39+n40; i++) {
    char pname[10];
    sprintf(pname,"vec_%i",i);
    vec.push_back(new TProfile(pname,pname,10,-0.5,9.5,"s"));
    sprintf(pname,"vecTDC_%i",i);
    vecTDC.push_back(new TH1D(pname,pname,120,-50,70));
    sprintf(pname,"vecM0T_%i",i);
    vecM0T.push_back(new TH1D(pname,pname,150,4,7));
  }

  for (UInt_t i=0; i<chain->GetEntries(); i++) {
    chain->GetEntry(i);

    if (i%1000000==0) cout << 100*i/float(chain->GetEntries()) << endl;

    if (QIE11DigiIEta->size()==0) continue;

    for (uint i=0; i<QIE11DigiFC->size(); i++) {

      double sumFC=0;
      double m0T=0;
      for (uint j=0; j<QIE11DigiFC->at(i).size(); j++) {
	sumFC+=QIE11DigiFC->at(i).at(j);
	m0T+=j*QIE11DigiFC->at(i).at(j);
      }

      m0T/=sumFC;

      if (sumFC<10000) continue;

      //cout << run << ", " << ls << endl;
      int val=0;
      if (run==294736) {
	for (int j=0; j<n36; j++) {
	  if (ls>=minLS_36.at(j) && ls<maxLS_36.at(j)) {
	    if (QIE11DigiTDC->at(i).at(4)!=62)
	      val=QIE11DigiTDC->at(i).at(4); 
	    else
	      val=QIE11DigiTDC->at(i).at(3)-50;
	    for (int x=0; x<10; x++) {
	      vec.at(j)->Fill(x,QIE11DigiFC->at(i).at(x)/sumFC);
	    }
	    vecTDC.at(j)->Fill(val);
	    vecM0T.at(j)->Fill(m0T);
	    hTDCvM0T->Fill(m0T, val);
	  }
	}
      }
      else if (run==294737) {
	for (int j=0; j<n37; j++) {
	  if (ls>=minLS_37.at(j) && ls<maxLS_37.at(j)) {
	    if (QIE11DigiTDC->at(i).at(4)!=62)
	      val=QIE11DigiTDC->at(i).at(4);
	    else
	      val=QIE11DigiTDC->at(i).at(3)-50;
	    for (int x=0; x<10; x++) {
	      vec.at(n36+j)->Fill(x,QIE11DigiFC->at(i).at(x)/sumFC);
	    }
	    vecTDC.at(n36+j)->Fill(val);
	    vecM0T.at(n36+j)->Fill(m0T);
	    hTDCvM0T->Fill(m0T, val);
	  }
	}
      }
      else if (run==294739) {
	for (int j=0; j<n39; j++) {
	  if (ls>=minLS_39.at(j) && ls<maxLS_39.at(j)) {
	    if (QIE11DigiTDC->at(i).at(4)!=62)
              val=QIE11DigiTDC->at(i).at(4);
	    else
              val=QIE11DigiTDC->at(i).at(3)-50;

	    for (int x=0; x<10; x++) {
	      vec.at(n36+n37+j)->Fill(x,QIE11DigiFC->at(i).at(x)/sumFC);
	    }
	    vecTDC.at(n36+n37+j)->Fill(val);
	    vecM0T.at(n36+n37+j)->Fill(m0T);
	    hTDCvM0T->Fill(m0T,val);
	  }
	}
      }
      else if (run==294740) {
	for (int j=0; j<n40; j++) {
	  if (ls>=minLS_40.at(j) && ls<maxLS_40.at(j)) {
	    if (QIE11DigiTDC->at(i).at(4)!=62)
	      val=QIE11DigiTDC->at(i).at(4);
	    else
	      val=QIE11DigiTDC->at(i).at(3)-50;
	    for (int x=0; x<10; x++) {
	      vec.at(n36+n37+n39+j)->Fill(x,QIE11DigiFC->at(i).at(x)/sumFC);
	    }
	    vecTDC.at(n36+n37+n39+j)->Fill(val);
	    vecM0T.at(n36+n37+n39+j)->Fill(m0T);
	    hTDCvM0T->Fill(m0T,val);
	  }
	}
      }

    }

  }


  outf->Write();
  outf->Close();

}
