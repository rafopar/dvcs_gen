#include "TGenDVCS.h"
#include "TH1.h"
#include <iostream.h>
void gen()
{
TGenDVCS *gEv=new TGenDVCS(5.7572,0,9465,7846);
gEv->SetTargetParam(15.,0.78,0.0723);
gEv->SetGeometry(0.417308,2.35,0.25836,1119.98);
  //gEv->SetCaloAcceptance(1000.,1000.,1000.,1000.);
  //gEv->SetFermi();
  Int_t nev=200000;
  Int_t j=0;
  cout<<"Generating "<<nev<<" events..."<<endl;
  
  // TH1F *h=new TH1F("h","h",100,0,0.2);

  for(Int_t i=0;i<nev;i++){
    if ((i%100)==0) cout<<i<<"/"<<nev<<endl;
    gEv->Settmin(-1.); //Use this method to change tmin (default -2 GeV)
    gEv->Settmax(0.);  //Use this method to change tmax (default 0 GeV)
    gEv->GenerateVertex();//Generates the vertex
    gEv->ExtBrem();//Make external pre-vertex radiative corrections
    gEv->GenKin();//Computes scattered electron kinematics
    gEv->IntRCBef();//Internal radiative corrections (before vertex)
    // gEv->GenFermiIni();//To use if fFermi is set
    if(gEv->ComputeElectron()){//Compute the scattered electron 4-vector
      gEv->IntRCAft();//Internal radiative corrections (after vertex)
      if(gEv->HitsSpectro(gEv->GetScatteredElectron())){//Check hor spectro accep.
	gEv->ComputeDVCS();//Compute the gamma*p->gamma p collision
	gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis

		cout<<"gEv->GetInitialElectron().Pz() = "<<gEv->GetInitialElectron()->Pz()<<endl;


	if(gEv->HitsCalo(gEv->GetFinalPhoton())){//Checks calo acceptance
	  gEv->XSec();
	  //cout<<gEv->XSecSum()<<" "<<gEv->XSecDif()<<" "<<endl;
	  //	  h->Fill(gEv->XSecSum());
	  j++;
	  gEv->Write2File();//This writes the event in a temporary file
	}
      }
    }
  }
  cout<<"j="<<j<<endl;
  gEv->DumpFinalFile("outdvcs.txt",nev,j);
  //  h->Draw();
}
