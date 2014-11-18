//
// TGenDVCSCLAS.cxx, v1.0, Tue Aug 4 11:13:57
// Author: C. Munoz Camacho
//

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "mydvcs.hh"

#ifndef __TGenDVCSCLAS__
#include "TGenDVCSCLAS.h"
#endif

using namespace std;

ClassImp(TGenDVCSCLAS)

////////////////////////////////////////////////////////////////////////////////
//
// Event generator base class
// 
////////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________
  TGenDVCSCLAS::TGenDVCSCLAS(Double_t Ebeam, Int_t TargType, UInt_t seed1, UInt_t seed2):TGenBase(Ebeam,seed1,seed2)
{
  // Default constructor
  // Initial 4-vectors are initialized

   // if(!fgIsInit) Init();

  cout<<"TGenDVCSCLAS constructor"<<endl;

  fdmom=0;
  frho=0;
  fTargType=TargType;

  if(fTargType!=1 && fTargType!=0 && fTargType!=2){
    cout<<"Unknown target for DVCS Event generator"<<endl;
    exit(1);
  }

  if(fTargType==0) fm=PMass();
  if(fTargType==1) fm=NMass();
  if(fTargType==2) fm=DMass();

  fpini=0;

  ftmax=0.;
  ftmin=0.;

  fxb=0;
  fQ2=0;
  fe=0;
  fq=0;
  fg=0;
  fp=0;
  fSigmaP=0;
  fSigmaM=0;

  //Default parameters for the cross-section calculation
  fphasespace=1;
  fprop=1;
  fb=1.;
  ftdep=1;
  ftcoef=0.8;
  fDD=1;
  fJu=0.3;
  fJd=-0.1;
  fdterm=1;
  fpipole=1;
    
}

//_____________________________________________________________________________
TGenDVCSCLAS::TGenDVCSCLAS(const TGenDVCSCLAS& gen):TGenBase(gen)
{
  // Copy constructor
  //  ((TGenDVCSCLAS&)TCalobase).Copy(*this);
}

//_____________________________________________________________________________
 TGenDVCSCLAS::~TGenDVCSCLAS()
{
  // Default destructor

  if(fpini) delete fpini;
  if(fe)    delete fe;
  if(fp)    delete fp;
  if(fg)    delete fg;
}

//_____________________________________________________________________________
void TGenDVCSCLAS::IntRCBef(void)
{
  // Makes internal radiative corrections _before_ the vertex (to the initial)
  // electron) using the equivalent radiator technique.
  // Uses \Delta E=E_0 * R^(2./nu) with R randomly between 0 and 1.
  // Factor 1/2 is because internal corrections must be applied twice (before
  // and after the vertex) with equivalent radiator thickness half each time.

  fRadCor=kTRUE;
  Double_t nu=2.*0.007297352533*(TMath::Log(fQ2/TMath::Power(0.000510998902,2))-1.)/TMath::Pi(); 
  
  Double_t eel=(feini->E())*(1-TMath::Power(fRan->Rndm(),1./(nu/2.)));
  
  feini->SetPxPyPzE(0.,0.,eel,eel);
}

//_____________________________________________________________________________
void TGenDVCSCLAS::IntRCAft(void)
{
  // Makes internal radiative corrections _after_ the vertex (to the scattered
  // electron) using the equivalent radiator technique.
  // Uses \Delta E=E_0 * R^(2./nu) with R randomly between 0 and 1.
  // Factor 1/2 is because internal corrections must be applied twice (before
  // and after the vertex) with equivalent radiator thickness half each time.

  fRadCor=kTRUE;
  Double_t nu=2.*0.007297352533*(TMath::Log(fQ2/TMath::Power(0.000510998902,2))-1.)/TMath::Pi(); 
  
  Double_t deel=1-TMath::Power(fRan->Rndm(),1./(nu/2.));

  *feprerad=(*fe); // keep vertex scattered electron

  *fe=deel*(*fe);
}
//_____________________________________________________________________________
  void TGenDVCSCLAS::Settmin(Double_t tmin) { 
    //BEWARE : When generating events in a loop, the TGenDVCSCLAS::Clear() method,
    //which needs to be used at the end of the loop, sets tmin=-2. and tmax=0.
    //If you don't want these default values, you need to use TGenDVCSCLAS::Settmin()
    //and TGenDVCSCLAS::Settmax() at the beginning of the loop to set your values
ftmin=tmin ; 
}
//_____________________________________________________________________________
  void TGenDVCSCLAS::Settmax(Double_t tmax) {
    //BEWARE : When generating events in a loop, the TGenDVCSCLAS::Clear() method,
    //which needs to be used at the end of the loop, sets tmin=-2. and tmax=0.
    //If you don't want these default values, you need to use TGenDVCSCLAS::Settmin()
    //and TGenDVCSCLAS::Settmax() at the beginning of the loop to set your values
 ftmax=tmax ; 
}
//_____________________________________________________________________________
  void TGenDVCSCLAS::Clear(char* opt) 
    {
      // Sets initial electron and proton and sets tmin=-2 GeV and tmax=0 GeV
      // If you don't want these default values of tmin and tmax use 
      // TGenDVCSCLAS::Settmin() and TGenDVCSCLAS::Settmax() at the beginning of your
      // loop

      TGenBase::Clear(opt);
      ftmin=-2.;
      ftmax=0.;

    }

//_____________________________________________________________________________
 void TGenDVCSCLAS::ComputeDVCS(void) 
{ 
  //Computes the gamma* p -> gamma p' reaction in the center of mass.
  //The angle phi is generated and all vectors are boost back to the 
  //laboratory. Data members fp and fg are set.

  Compute2Body(0.);
}

//_____________________________________________________________________________
 void TGenDVCSCLAS::Compute2Body(Double_t m) 
{ 
  //Computes the gamma* p -> X p' reaction in the center of mass, as a
  //function of the mass m of the particle X.
  //The angle phi is generated and all vectors are boost back to the 
  //laboratory. Data members fp and fg are set.
  
  //////////////////////
  //Computation of t and the phase space factor
  //////////////////////

  Double_t nu=fQ2/(2.*fm*fxb);
  // BEWARE:Calculated for non-fermi smeared kinematics

  Double_t q3=TMath::Sqrt(fQ2+TMath::Power(nu,2.));

  Double_t q0primemax=0.5*fQ2*(1.-fxb)/(fxb*(fm+nu-q3));
  Double_t q0primemin=0.5*fQ2*(1.-fxb)/(fxb*(fm+nu+q3));
  Double_t tmax=-fQ2-2.*q0primemax*(nu-q3);
  Double_t tmin=-fQ2-2.*q0primemin*(nu+q3);
  ftmax=tmax-0.02;
  ftmin=TMath::Max(-1.,tmin+0.02);
  
  if(fFermi) ftmax=0;

  do {
    ft=ftmax+(ftmin-ftmax)*fRan->Rndm();
  } while (ft/fQ2>1.);

  //  cout<<ft<<" "<<ftmax<<" "<<ftmin<<endl;


  fPSF=(ftmax-ftmin)*(fxbmax-fxbmin)*(fQ2max-fQ2min);
  ///////////////////

  //  cout << "ftmax: " << ftmax << endl;
  //  cout << "ftmin: " << ftmin << endl;

  // fq=new TLorentzVector(feini->Px()-fe->Px(),feini->Py()-fe->Py(),feini->Pz()-fe->Pz(),feini->E()-fe->E());

  if(fpini==0 && fFermi==kFALSE) fpini=new TLorentzVector(0.,0.,0.,fm);
  if(fpini==0 && fFermi==kTRUE){
    cout<<"You must generate fermi recoil particle first"<<endl;
    exit(1);
  }
  
  TLorentzVector p=*fpini;
  TLorentzVector q=*fq;

  // Boost to center of mass p-q

  TLorentzVector cms = q + p;

  fs=cms.M2();

  q.Boost(-cms.BoostVector() );
  p.Boost(-cms.BoostVector() );

  // Rotation around Y
  
  TVector3 oz(0.,0.,1.);
  TVector3 perpvec=(q.Vect()).Cross(oz);
  Double_t angle=(q.Vect()).Angle(oz);

  q.Rotate(angle,perpvec.Unit());
  p.Rotate(angle,perpvec.Unit());

  //Double_t egammacm=(fs-fm*fm)/(2.*TMath::Sqrt(fs));
  //Double_t thetacm = (ft+fQ2)/(2.*egammacm*q.P())+q(3)/q.P();

  //////////////////////////////////////////

  Double_t ecm=(fs-fm*fm+m*m)/(2.*TMath::Sqrt(fs));
  Double_t pcm=TMath::Sqrt(TMath::Power(ecm,2.)-TMath::Power(m,2.));
  Double_t thetacm=(ft+fQ2-m*m+2.*q(3)*ecm)/(2.*pcm*q.P());

  ///////////////////////////////////////// 

  if (thetacm>1.) thetacm=1.;
  if (thetacm<-1.) thetacm=-1.;
  thetacm=TMath::ACos(thetacm);

  if(!fg) {
    fg=new TLorentzVector(pcm*TMath::Sin(thetacm),0.,TMath::Cos(thetacm)*pcm,ecm);
  }else{
    fg->SetPxPyPzE(pcm*TMath::Sin(thetacm),0.,TMath::Cos(thetacm)*pcm,ecm);
  }
  
  if(!fp){
    fp=new TLorentzVector(q.Px()+p.Px()-fg->Px(),q.Py()+p.Py()-fg->Py(),q.Pz()+p.Pz()-fg->Pz(),q.E()+p.E()-fg->E());
  }else{
    fp->SetPxPyPzE(q.Px()+p.Px()-fg->Px(),q.Py()+p.Py()-fg->Py(),q.Pz()+p.Pz()-fg->Pz(),q.E()+p.E()-fg->E());
  }
   
  // Rotation back
  fp->Rotate(-angle,perpvec.Unit());
  fg->Rotate(-angle,perpvec.Unit());
  q.Rotate(-angle,perpvec.Unit());
  
  // Boost to lab
  fp->Boost(cms.BoostVector());
  fg->Boost(cms.BoostVector());
  q.Boost(cms.BoostVector());

  // Now we have k along Oz and k' on the xOz plane
  // q=k-k' is necessarily on plane as well
  // We just have to rotate along Oy to bring q along Oz
  // and then rotate everything by phi except
  // k and k'. Then we rotate back along Oy.

  angle=(q.Vect()).Angle(oz);
  //  cout<<angle<<endl;

  fp->RotateY(angle);
  fg->RotateY(angle);
  
  fphi=2.*TMath::Pi()*fRan->Rndm();   // phi between 0 and 2pi
  
  fp->RotateZ(fphi);
  fg->RotateZ(fphi);

  fp->RotateY(-angle);
  fg->RotateY(-angle);

}

//_____________________________________________________________________________
void TGenDVCSCLAS::ApplySpecVerAcc(Double_t aav)
{
  // Applies vertical spectrometer acceptance by rotating all 4-vectors
  // around the beam axis. An angle can be specified, otherwise it's generated
  // randomly between spectrometer acceptances

  Double_t av=aav;
  if(aav==-1.) {
    av=2.*TMath::Pi()*fRan->Rndm();
  }
  TGenBase::ApplySpecVerAcc(av); // Rotates electron !
  fg->RotateZ(av);
  fp->RotateZ(av);

  fq->RotateZ(av);
  
  fPSF*=2.*TMath::Pi();

  // Remark:
  // The right phasespace factor is
  // dx.dq2.dt.dphi.dphie/2pi (because the x-section code gives 
  // phi_e-integrated x-sections) but dphi=2pi, so it simplifies to
  // dx.dq2.dt.dphie
}

//_____________________________________________________________________________
 TLorentzVector* TGenDVCSCLAS::GetFinalPhoton(void)
{
  // Returns the final photon 4-vector if it exists
  
  if(!fg) cout<<"Warning : Final photon doesn't exist"<<endl;
  return fg;

}

//_____________________________________________________________________________
 TLorentzVector* TGenDVCSCLAS::GetFinalProton(void)
{
  // Returns the final photon 4-vector if it exists
  
  if(!fp) cout<<"Warning : Final proton doesn't exist"<<endl;
  return fp;

}

//_____________________________________________________________________________
 Double_t TGenDVCSCLAS::GetFastWeight(void)
{
  // Returns a fast cross-section

  Double_t q2c=2.*fSpecMom*fEbeam*(1-TMath::Cos(fSpecAngle));
  Double_t weight=TMath::Power(q2c/fQ2,3.)*TMath::Power(1./(1.-ft/(fm*fm)),4.);

  return weight;
}

//_____________________________________________________________________________
 void TGenDVCSCLAS::XSec(void)
{
  // Computes the DVCS cross-section for both electron helicities

  SigmaDVCS(&fSigmaP,&fSigmaM,fTargType,fFermi,feini->E(),fQ2,fxb,ft,fphi,fpini->P(),fpini->Theta(),fpini->Phi(),fphasespace,fprop,fb,ftdep,ftcoef,fDD,fJu,fJd,fdterm,fpipole);

  if (TMath::IsNaN(fSigmaP) || TMath::IsNaN(fSigmaM)) {fSigmaP=0.;fSigmaM=0.;}

}

//_____________________________________________________________________________
 void TGenDVCSCLAS::SetTheoryParam(Int_t phasespace, Int_t prop, Double_t b, Int_t tdep, Double_t tcoef, Int_t DD, Double_t Ju, Double_t Jd, Int_t dterm, Int_t pipole)
{
  //Sets parameters for the library used to compute the DVCS cross-section

  fphasespace=phasespace;
  fprop=prop;
  fb=b;
  ftdep=tdep;
  ftcoef=tcoef;
  fDD=DD;
  fJu=Ju;
  fJd=Jd;
  fdterm=dterm;
  fpipole=pipole;

}

//_____________________________________________________________________________
void TGenDVCSCLAS::Write2File(void)
{

  if(!fNwrite) {
      fNwrite=14;
 }
  
  *output<<fVertex->Pz()<<" ";
  *output<<feini->Pz()<<" ";
  *output<<fe->Px()<<" "<<fe->Py()<<" "<<fe->Pz()<<" ";
  *output<<fg->Px()<<" "<<fg->Py()<<" "<<fg->Pz()<<" ";
  *output<<fp->Px()<<" "<<fp->Py()<<" "<<fp->Pz();
  *output<<" "<<fSigmaP<<" "<<fSigmaM<<" "<<fPSF<<endl;
  //  *output<<fVertez->Pz()<<endl;
  // *output<<endl;
}

//_____________________________________________________________________________
 void TGenDVCSCLAS::Print(char* opt)
{
  // Output on screen. If option "all" is specified the complete setup of the
  // event is printed out. By default only the final state 4-vectors and the 
  // virtual photon are displayed.

  TString option=opt;
  if(option.Contains("all")) TGenBase::Print();
  cout<<"======================================="<<endl;
  cout<<"        4-vectors (Px,Py,Pz,E)         "<<endl;
  cout<<"======================================="<<endl;
  if(feini) {
    cout<<"e("<<feini->Px()<<","<<feini->Py()<<","<<feini->Pz()<<","<<feini->E()<<")"<<endl;
  }else{
    cout<<"NO INITIAL ELECTRON DEFINED"<<endl;
  }
  if(fpini){
    cout<<"p("<<fpini->Px()<<","<<fpini->Py()<<","<<fpini->Pz()<<","<<fpini->E()<<")"<<endl;
  }else{
    cout<<"NO INITIAL TARGET PARTICLE DEFINED"<<endl;
  }
  if(fq){
  cout<<"g*("<<fq->Px()<<","<<fq->Py()<<","<<fq->Pz()<<","<<fq->E()<<")"<<endl;
  }else{
    cout<<"NO VIRTUAL PHOTON DEFINED"<<endl;
  }
  if(fe){
    cout<<"e'("<<fe->Px()<<","<<fe->Py()<<","<<fe->Pz()<<","<<fe->E()<<")"<<endl;
  }else{
    cout<<"NO SCATTERED ELECTRON DEFINED"<<endl;
  }
  if(fg){
    cout<<"g("<<fg->Px()<<","<<fg->Py()<<","<<fg->Pz()<<","<<fg->E()<<")"<<endl;
  }else{
    cout<<"NO EMITTED PHOTON DEFINED"<<endl;
  }
  if(fp){
    cout<<"p'("<<fp->Px()<<","<<fp->Py()<<","<<fp->Pz()<<","<<fp->E()<<")"<<endl;
  }else{
    cout<<"NO RECOIL PARTICLE DEFINED"<<endl;
  }
  cout<<"======================================="<<endl;
}
