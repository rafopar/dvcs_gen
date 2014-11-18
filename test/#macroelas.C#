{
TCanvas *c=new TCanvas();
TH1F *h=new TH1F("h","Theta_e",100,20,24);
TGenElas *gEv=new TGenElas(5.75,0,8276,298635);
gEv->SetTargetParam(15.,0.0723);
gEv->SetGeometry(0.659291,2.36925,0.37885,6000.);
gEv->SetFermi();
Int_t nev=5, j=0;
Double_t cs=0., cs2=0.;
cout<<"Generating "<<nev<<" events..."<<endl;
cout<<"Total cross-section w/o rc or calo acceptance : "<<gEv->XSecTotal()<<" barn"<<endl;
Double_t lum=1e37;//cm^-2 s^-1

for(Int_t i=0;i<nev;i++){
  gEv->GenerateVertex();//Generates the vertex
  gEv->ExtBrem();//Make external pre-vertex radiative corrections
  gEv->GenKin();//Computes scattered proton kinematics
  gEv->IntRCBef();//Internal radiative corrections (before vertex)
  gEv->GenFermiIni();//To use if fFermi is set
  if(gEv->ComputeElectron()){
    gEv->IntRCAft();//Internal radiative corrections (after vertex)
    gEv->ComputeElas();//Compute the scattered electron 4-vector
    if(gEv->HitsSpectro(gEv->GetFinalProton())){//Check hor spectro accep.
      gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis
      if(gEv->HitsCalo(gEv->GetScatteredElectron())){//Checks calo acceptance
	//gEv->Print();
	cs2+=gEv->GetPSF()*gEv->XSec();//This is another way of calculating the total cross-section:
	//If cross-section computation is time-consuming (case of DVCS), we could accept all events and
	//weight them by the differential cross-section
	if((gRandom->Rndm())*(gEv->XSecMax())<gEv->XSec()){//This generates events according to the elastic cross-section
	  h->Fill(-(180./TMath::Pi())*TMath::ASin(gEv->GetScatteredElectron()->Px()/gEv->GetScatteredElectron()->P()));
	  cs+=(gEv->GetPSF())*gEv->XSecMax()/nev;//This calculates the total cross-section
	  j++;
	  gEv->Write2File();//This writes the event in a temporary file
	}
      }
    }
  }
}
gEv->DumpFinalFile("outelas.txt",nev,j);
h->Draw();
cout<<"====================================================="<<endl;
cout<<"Number of accepted events : "<<j<<endl;
cout<<"Total cross-section : "<<cs<<" barn"<<endl;
cout<<"Beam time at luminosity "<<lum<<" : "<<j/(lum*cs*1e-24*60)<<" min"<<endl;//1e-24 for bn->cm^2 conversion
cout<<"Counting rate : "<<j/(j/(lum*cs*1e-24))<<" Hz"<<endl;
cout<<"-----------------------------------------------------"<<endl;
cout<<"Total cross-section (DVCS way) : "<<cs2/nev<<" barn"<<endl;
cout<<"Beam time : "<<j/(lum*(cs2/nev)*1e-24*60);
cout<<" min  Counting rate : "<<j/(j/(lum*(cs2/nev)*1e-24))<<" Hz"<<endl;

}
