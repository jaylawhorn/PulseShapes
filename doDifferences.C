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

  TH1D *g3 = new TH1D("g3","TS3",300,0,150);
  TH1D *g4 = new TH1D("g4","TS4",300,0,150);
  TH1D *g5 = new TH1D("g5","TS5",300,0,150);
  TH1D *g6 = new TH1D("g6","TS6",300,0,150);
  TH1D *g7 = new TH1D("g7","TS7",300,0,150);

  TH1D *d3 = new TH1D("d3","TS3",300,0,150);
  TH1D *d4 = new TH1D("d4","TS4",300,0,150);
  TH1D *d5 = new TH1D("d5","TS5",300,0,150);
  TH1D *d6 = new TH1D("d6","TS6",300,0,150);
  TH1D *d7 = new TH1D("d7","TS7",300,0,150);

  TH1D *inte = new TH1D("i3","TS3",300,0,150);

  
  for (int i=0; i<50; i++) {
    if (49-i==44) continue;

    g3->SetBinContent(i+1,     v_p4.at(49-i)->GetBinContent(3+1));
    g4->SetBinContent(i+1+50,  v_p4.at(49-i)->GetBinContent(4+1));
    g5->SetBinContent(i+1+100, v_p4.at(49-i)->GetBinContent(5+1));
    g6->SetBinContent(i+1+150, v_p4.at(49-i)->GetBinContent(6+1));
    g7->SetBinContent(i+1+200, v_p4.at(49-i)->GetBinContent(7+1));

  }

  for (int i=0; i<12; i++) {

    g3->SetBinContent(i+1+50, v_p3.at(49-i)->GetBinContent(3+1));
    g4->SetBinContent(i+1+100,v_p3.at(49-i)->GetBinContent(4+1));
    g5->SetBinContent(i+1+150,v_p3.at(49-i)->GetBinContent(5+1));
    g6->SetBinContent(i+1+200,v_p3.at(49-i)->GetBinContent(6+1));
    g7->SetBinContent(i+1+250,v_p4.at(49-i)->GetBinContent(7+1));

  }

  double tempX=0;
  double qq=0;
  for (int i=0; i<300; i++) {
    tempX=g3->GetBinContent(i+1)-g3->GetBinContent(i);
    //if (i>50) tempX+=d4->GetBinContent(i+1-50);
    d3->SetBinContent(i+1, tempX);

    //if (i==0) tempX=g4->GetBinContent(i+1)-g3->GetBinContent(i);
    tempX=g4->GetBinContent(i+1)-g4->GetBinContent(i);
    //if (i>50) tempX+=d4->GetBinContent(i+1-50);
    d4->SetBinContent(i+1, tempX);

    tempX=g5->GetBinContent(i+1)-g5->GetBinContent(i);
    //if(i>50) tempX+=d5->GetBinContent(i+1-50);
    d5->SetBinContent(i+1, tempX);

    tempX=g6->GetBinContent(i+1)-g6->GetBinContent(i);
    //if(i>50) tempX+=d6->GetBinContent(i+1-50);
    d6->SetBinContent(i+1, tempX);

    tempX=g7->GetBinContent(i+1)-g7->GetBinContent(i);
    //if(i>50) tempX+=d7->GetBinContent(i+1-50);
    d7->SetBinContent(i+1, tempX);
  }
  
  /*  float sum=0;
  for (int i=0; i<300; i++) {
    sum+=d3->GetBinContent(i+1);
    sum+=d4->GetBinContent(i+1);
    sum+=d5->GetBinContent(i+1);
    sum+=d6->GetBinContent(i+1);
    sum+=d7->GetBinContent(i+1);
    inte->SetBinContent(i+1, sum);

  }

  inte->Draw();*/

  //d4->GetYaxis()->SetRangeUser(-0.1,0.5);
  /*    
  d3->SetLineWidth(2);
  d4->SetLineWidth(2);
  d5->SetLineWidth(2);
  d6->SetLineWidth(2);
  d7->SetLineWidth(2);

  d3->Draw("hist");
  d4->SetLineColor(kRed);
  d4->Draw("histsame");
  d5->SetLineColor(kGreen);
  d5->Draw("histsame");
  d6->SetLineColor(kBlue);
  d6->Draw("histsame");
  d7->SetLineColor(kViolet);
  d7->Draw("histsame");
  */
  
  g5->SetLineColor(kGreen);  
  g5->Draw("hist"); 
  g4->SetLineColor(kRed);
  g4->Draw("same");
  g3->Draw("same"); 
  g6->SetLineColor(kBlue);
  g6->Draw("histsame");
  g7->SetLineColor(kViolet);
  g7->Draw("histsame");

}
