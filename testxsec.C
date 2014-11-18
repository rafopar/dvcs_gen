{
  gSystem->Load("libTGVKelly.so");
  gSystem->Load("libTGenDVCS.so");
  TGenDVCS *gEv=new TGenDVCS(5.7572,0,0,7846);
  TGraph *g=new TGraph();
  TGraph *gbh=new TGraph();
  for(Int_t i=0;i<1000;i++){
    gEv->SetXb(0.36);
    gEv->SetQ2(2.3);
    gEv->Settmin(-0.28);
    gEv->Settmax(-0.28000001);
    gEv->ComputeElectron();
    gEv->ComputeDVCS(); 
    g->SetPoint(i,gEv->GetPhi(),gEv->XSecSum());
    gbh->SetPoint(i,gEv->GetPhi(),gEv->XSecSum(1));
  }
  TMultiGraph *mg=new TMultiGraph();
  mg->Add(g,"*");
  mg->Add(gbh,"*");
  gbh->SetMarkerColor(kRed);
  mg->Draw("A");
  mg->SetMinimum(0);
}
