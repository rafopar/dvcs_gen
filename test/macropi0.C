{
TGenPi0 *gEv=new TGenPi0(5.7572,0,19852,132132);
gEv->SetTargetParam(15.,0.78,0.0723);
gEv->SetGeometry(0.417308,2.35,0.25836,1119.98);
//gEv->SetFermi();
Int_t nev=1000;
Int_t j=0;
cout<<"Generating "<<nev<<" events..."<<endl;

for(Int_t i=0;i<nev;i++){
 gEv->Settmin(-1.);
 gEv->Settmax(0.);
 gEv->GenerateVertex();//Generates the vertex
 gEv->ExtBrem();//Make external pre-vertex radiative corrections
 gEv->GenKin();//Computes scattered electron kinematics

 // Remplace GenKin pour generer l'electron au milieu de l'acceptance

 //gEv->SetQ2(2.32);
 //gEv->SetXb(0.364);

 gEv->IntRCBef();//Internal radiative corrections (before vertex)
 //gEv->GenFermiIni();//To use if fFermi is set
 if(gEv->ComputeElectron()) {//Compute the scattered electron 4-vector
   gEv->IntRCAft();//Internal radiative corrections (after vertex)
   if(gEv->HitsSpectro(gEv->GetScatteredElectron())){//Check hor spectro accep.
     gEv->ComputePi0();//Compute the gamma*p->pi0 p collision
     //cout<<"Event number : "<<i<<endl;
     gEv->TwoBodyDecay(0.1349766,0.,0.);
     gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis
     if( (gEv->HitsCalo(gEv->GetFinalPhoton1())) || (gEv->HitsCalo(gEv->GetFinalPhoton2()))){
       j++;
       gEv->Write2File();//This writes the event in a temporary file
     }
   }
 }
}
cout<<"j="<<j<<endl;
gEv->DumpFinalFile("outpi0.txt",nev,j);
}
