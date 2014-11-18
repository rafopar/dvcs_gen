{
TGenOme *gEv=new TGenOme(5.7572,0,1744,354);
gEv->SetTargetParam(15.,0.78,0.0723);
gEv->SetGeometry(0.417308,2.35,0.25836,1119.98);
//gEv->SetFermi();
Int_t nev=10000000;
Int_t j=0;
cout<<"Generating "<<nev<<" events..."<<endl;

for(Int_t i=0;i<nev;i++){
 gEv->Settmin(0.);
 gEv->Settmax(-1.);
 gEv->GenerateVertex();//Generates the vertex
 gEv->ExtBrem();//Make external pre-vertex radiative corrections
 gEv->GenKin();//Computes scattered electron kinematics
 gEv->IntRCBef();//Internal radiative corrections (before vertex)
 //gEv->GenFermiIni();//To use if fFermi is set
  if(gEv->ComputeElectron()) {//Compute the scattered electron 4-vector
    gEv->IntRCAft();//Internal radiative corrections (after vertex)
    if(gEv->HitsSpectro(gEv->GetScatteredElectron())){//Check hor spectro accep.
      gEv->ComputeOme();//Compute the gamma*p->pi0 p collision
   //cout<<"Event number : "<<i<<endl;
      gEv->TwoBodyDecay(0.1349766,0.,0.);
      gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis
      if((gEv->HitsCalo(gEv->GetFinalPhoton1())) && (gEv->HitsCalo(gEv->GetFinalPhoton2()))){//Checks calo acceptance
	j++;
	gEv->Write2File();//This writes the event in a temporary file
      }
    }
  }
}
cout<<"j="<<j<<endl;
gEv->DumpFinalFile("outome.txt",nev,j);

}
