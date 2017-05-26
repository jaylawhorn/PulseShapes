{

  //-------------------------------------------
  //
  //generate array of numbers
  //
  //-------------------------------------------

  TFile *f = new TFile("sortedByTDC.root","read");
  
  vector<TProfile*> v_p3;
  vector<TProfile*> v_p4;
  for (int i=0; i<50; i++) {
    char pname[10];
    sprintf(pname,"p3_%i",i);
    v_p3.push_back((TProfile*)f->Get(pname));
    sprintf(pname,"p4_%i",i);
    v_p4.push_back((TProfile*)f->Get(pname));

  } 

  TH1D *g4 = new TH1D("g4","TS4",200,0,100);
  TH1D *g5 = new TH1D("g5","TS5",200,0,100);
  TH1D *g6 = new TH1D("g6","TS6",200,0,100);
  TH1D *g7 = new TH1D("g7","TS7",200,0,100);

  TH1D *d4 = new TH1D("d4","TS4",200,0,100);
  TH1D *d5 = new TH1D("d5","TS5",200,0,100);
  TH1D *d6 = new TH1D("d6","TS6",200,0,100);
  TH1D *d7 = new TH1D("d7","TS7",200,0,100);
  
  for (int i=0; i<50; i++) {
    if (49-i==44) continue;

    g4->SetBinContent(i+1,     v_p4.at(49-i)->GetBinContent(4+1));
    if (i+1==13) cout << v_p4.at(49-i)->GetBinContent(4+1) << endl;
    g5->SetBinContent(i+1+50,  v_p4.at(49-i)->GetBinContent(5+1));
    g6->SetBinContent(i+1+100, v_p4.at(49-i)->GetBinContent(6+1));
    g7->SetBinContent(i+1+150, v_p4.at(49-i)->GetBinContent(7+1));

  }

  for (int i=0; i<50; i++) {

    g4->SetBinContent(i+1+50, v_p3.at(49-i)->GetBinContent(4+1));
    g5->SetBinContent(i+1+100,v_p3.at(49-i)->GetBinContent(5+1));
    g6->SetBinContent(i+1+150,v_p3.at(49-i)->GetBinContent(6+1));
    g7->SetBinContent(i+1+200,v_p4.at(49-i)->GetBinContent(7+1));

  }

  double tempX=0;

  for (int i=1; i<200; i++) {
    tempX=0;
    if (g4->GetBinContent(i+1)!=0 && g4->GetBinContent(i)==0) {
      cout << "4, " << i << ", " << g4->GetBinContent(i+1) << endl;
      //d4->SetBinContent(i+1,0);
    }
    else if (g4->GetBinContent(i+1)==0 && g4->GetBinContent(i)!=0)
      cout << "4, " << i << endl;
    //d4->SetBinContent(i+1,0);
    ///else {
    if (i>50) tempX=d4->GetBinContent(i+1-50);
    //cout << tempX << endl;
    //d4->SetBinContent(i+1, g4->GetBinContent(i+1) - g4->GetBinContent(i)+tempX);
    d4->SetBinContent(i+1, g4->GetBinContent(i+1));
      //}

    tempX=0;
    if (g5->GetBinContent(i+1)!=0 && g5->GetBinContent(i)==0) {
      cout << "5, " << i << endl;
      //d5->SetBinContent(i+1,0);
    }
    else if (g5->GetBinContent(i+1)==0 && g5->GetBinContent(i)!=0)
      cout << "5, " << i << endl;
    //d5->SetBinContent(i+1,0);
    //else {
    if(i>50) tempX=d5->GetBinContent(i+1-50);
    //cout << tempX << endl;
    //d5->SetBinContent(i+1, g5->GetBinContent(i+1) - g5->GetBinContent(i)+tempX);
    d5->SetBinContent(i+1, g5->GetBinContent(i+1));
    //}

    tempX=0;
    if (g6->GetBinContent(i+1)!=0 && g6->GetBinContent(i)==0) { 
      cout << "6, " << i << endl;
      //d6->SetBinContent(i+1,0);
    }
    else if (g6->GetBinContent(i+1)==0 && g6->GetBinContent(i)!=0)
      cout << "6, " << i << endl;
    //d6->SetBinContent(i+1,0);
    //else {
    //if(i>50) tempX=d6->GetBinContent(i+1-50);
    //cout << tempX << endl;
    //d6->SetBinContent(i+1, g6->GetBinContent(i+1) - g6->GetBinContent(i)+tempX);
    d6->SetBinContent(i+1, g6->GetBinContent(i+1));
    //}

    tempX=0;
    if (g7->GetBinContent(i+1)!=0 && g7->GetBinContent(i)==0) {
      cout << "7, " << i << endl;
      //d7->SetBinContent(i+1,0);
    }
    else if (g7->GetBinContent(i+1)==0 && g7->GetBinContent(i)!=0)
      cout << "7, " << i << endl;
    //d7->SetBinContent(i+1,0);
    //else {
    if(i>50) tempX=d7->GetBinContent(i+1-50);
    //cout << tempX << endl;
    //d7->SetBinContent(i+1, g7->GetBinContent(i+1) - g7->GetBinContent(i)+tempX);
    d7->SetBinContent(i+1, g7->GetBinContent(i+1));
    //}
  }

  //d4->GetYaxis()->SetRangeUser(-0.1,0.5);

  d4->SetLineWidth(2);
  d5->SetLineWidth(2);
  d6->SetLineWidth(2);
  d7->SetLineWidth(2);

  d4->SetLineColor(kRed);
  d4->Draw("hist");
  d5->SetLineColor(kGreen);
  d5->Draw("histsame");
  d6->SetLineColor(kBlue);
  d6->Draw("histsame");
  d7->SetLineColor(kViolet);
  d7->Draw("histsame");

  /*
  g5->SetLineColor(kGreen);  
  g5->Draw("hist"); 
  g4->SetLineColor(kRed);
  g4->Draw("same");
  g6->SetLineColor(kBlue);
  g6->Draw("histsame");

  g7->SetLineColor(kViolet);
  g7->Draw("histsame");
  */

}
