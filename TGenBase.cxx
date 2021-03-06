//
// TGenBase.cxx, v1.0, Tue Aug 4 11:13:57
// Author: C. Munoz Camacho
//

#include <fstream>
#include <iostream>
#include <stdlib.h>

#ifndef __TGenBase__
#include "TGenBase.h"
#endif

#ifndef ROOT_TMath
#include "TMath.h"
#endif

#include "TRandom2.h"

using namespace std;

ClassImp(TGenBase)

////////////////////////////////////////////////////////////////////////////////
//
// Event generatorS base class
// 
////////////////////////////////////////////////////////////////////////////////

Bool_t           TGenBase::fgIsInit = kFALSE;
Bool_t           TGenBase::fgWarnings = kTRUE;
Bool_t           TGenBase::fgErrors = kTRUE;

//_____________________________________________________________________________
 TGenBase::TGenBase(Double_t Ebeam, UInt_t seed1, UInt_t seed2)
{
  // Default constructor
  // The default behaviour is : seeds 1 and 2, we ask for a particle in 
  // the calorimeter, don't ask for a proton in the proton array,
  // radiative corrections activated, fermi momentum disable.
  // Default detectors acceptances are set.

  fEbeam=Ebeam;
  feini=new TLorentzVector(0.,0.,fEbeam,fEbeam);//We neglect electron mass
  feprerad=new TLorentzVector(0.,0.,0.,0.);

  fRan=new TRandom2();
  fRan->SetSeed(0);

  fRadCor=kFALSE;
  fFermi=kFALSE;

  fVertex=0;
  fNwrite=0;
  output=new ofstream("out.txt.tmp");
}

//_____________________________________________________________________________
 TGenBase::TGenBase(const TGenBase& TCalobase)
{
  // Copy constructor
  //  ((TGenBase&)TCalobase).Copy(*this);
}

//_____________________________________________________________________________
 TGenBase::~TGenBase()
{
  // Default destructor
  if (fVertex) delete fVertex;
  delete feini;
  delete feprerad;
  delete fRan;
}

//_____________________________________________________________________________
 void TGenBase::ExtBrem(void)
{
  // Make external bremsstrahlung corrections before the vertex (straggling)
  // Uses \Delta E=E_0 * R^(1/b/t) with R randomly between 0 and 1.

  Double_t t=0; //Lengh of matter in radiation length
  Double_t eel=0.;
  fRadCor=kTRUE;

   if(fTargType!=0 && fTargType!=1 && fTargType!=2){
    cout<<" UNKOWN TARGET!"<<endl;
    exit(1);
  }
  if(fTargType==0){
    t=(fVertex->Pz()+fTargLength/2.)/PX0();
  }else{
    t=(fVertex->Pz()+fTargLength/2.)/NX0();
  }

    Double_t toto=TMath::Power(fRan->Rndm(),1./(b()*t));

    //cout<<"toto="<<toto<<endl;    

    eel=fEbeam*(1.-toto);
    feini->SetPxPyPzE(0.,0.,eel,eel);
}

//_____________________________________________________________________________
 void TGenBase::SetVertex(Double_t x, Double_t y, Double_t z) 
{ 
  // Set Vertex coordinates

  if(!fVertex){
    fVertex=new TVector3(x,y,z);
  }else{
    fVertex->SetXYZ(x,y,z);
  }
}

//_____________________________________________________________________________
 void TGenBase::SetVertex(TVector3* vertex) 
{ 
  // Sets vertex

  if(!fVertex){
    fVertex=new TVector3(vertex->Px(),vertex->Py(),vertex->Pz());
  }else{
    fVertex->SetXYZ(vertex->Px(),vertex->Py(),vertex->Pz());
  }
}

//_____________________________________________________________________________
 void TGenBase::GenerateVertex(void) 
{ 
  // Generates an uniformly distributed vertex along the target length
  // (no rastering), with offset from TGenGeo

  SetVertex(0.,0.,fTargLength*fRan->Rndm()-fTargLength/2.+fTargZoff);
}

//_____________________________________________________________________________
 void TGenBase::GenKin(void)
{
  // Generates the electron event : Q2, and xb (uniformly and randomly between limits)

  // These 4 following values could be set as non-persistent data members of TGenBase
  // and avoid recalculating them for each event.
  Double_t thetaemax=fSpecAngle+fSpecHorAccGen;
  Double_t thetaemin=fSpecAngle-fSpecHorAccGen;
  Double_t pemax=fSpecMom*(1+fSpecMomAccGen);
  Double_t pemin=fSpecMom*(1-fSpecMomAccGen);
 
  //cout<<"th_min="<<thetaemin<<"  th_max="<<thetaemax<<
  //    "pemin="<<pemin<<"pemax="<<pemax<<endl;


  Double_t eel=feini->E();

  fxbmin=TMath::Max(0.05,pemin*eel*(1.-TMath::Cos(thetaemin))/(fm*(eel-pemin)));
  fxbmax=TMath::Min(0.95,pemax*eel*(1.-TMath::Cos(thetaemax))/(fm*(eel-pemax)));
  if(eel<pemin){
    fxbmin=0.00000001;
    fxbmax=0.00000001;
  }//Again, a great energy loss might make our equations invalid

  if (fFermi) {
    fxbmin=TMath::Min(0.1,fxbmin*0.8);
    fxbmax=TMath::Max(0.9,fxbmax*1.2);
  }

  // This is also true for Fermi smeared kinematics

  fQ2min=2.*pemin*eel*(1-TMath::Cos(thetaemin));
  fQ2max=2.*pemax*eel*(1-TMath::Cos(thetaemax)); 
  
  fxb=fxbmin+(fxbmax-fxbmin)*fRan->Rndm();
  fQ2=fQ2min+(fQ2max-fQ2min)*fRan->Rndm();
 
}
//_____________________________________________________________________________
 void TGenBase::GenKinGen(void)
{
  // Generates the electron event : Q2, and xb (uniformly and randomly between limits)

  // These 4 following values could be set as non-persistent data members of TGenBase
  // and avoid recalculating them for each event.
//   Double_t thetaemax=TMath::Abs(fCaloAngle)+0.03;
//   Double_t thetaemin=TMath::Abs(fCaloAngle)-0.03;
  Double_t thetaemax=50.*TMath::DegToRad();
  Double_t thetaemin=0.;
  Double_t pemax=4.;
  Double_t pemin=0.1;
 
  Double_t eel=feini->E();

  fxbmin=TMath::Max(0.05,pemin*eel*(1.-TMath::Cos(thetaemin))/(fm*(eel-pemin)));
  fxbmax=TMath::Min(0.95,pemax*eel*(1.-TMath::Cos(thetaemax))/(fm*(eel-pemax)));
  if(eel<pemin){
    fxbmin=0.00000001;
    fxbmax=0.00000001;
  }//Again, a great energy loss might make our equations invalid

  if (fFermi) {
    fxbmin=TMath::Min(0.1,fxbmin*0.8);
    fxbmax=TMath::Max(0.9,fxbmax*1.2);
  }

  // This is also true for Fermi smeared kinematics

  fQ2min=2.*pemin*eel*(1-TMath::Cos(thetaemin));
  fQ2max=2.*pemax*eel*(1-TMath::Cos(thetaemax)); 
  
  fxb=fxbmin+(fxbmax-fxbmin)*fRan->Rndm();
  fQ2=fQ2min+(fQ2max-fQ2min)*fRan->Rndm();
 
}

//_____________________________________________________________________________
 void TGenBase::GenKinCLAS(void)
{
  // Generates the electron event : Q2, and xb (uniformly and randomly between limits)

  // These 4 following values could be set as non-persistent data members of TGenBase
  // and avoid recalculating them for each event.
//   Double_t thetaemax=fSpecAngle+fSpecHorAccGen;
//   Double_t thetaemin=fSpecAngle-fSpecHorAccGen;
//   Double_t pemax=fSpecMom*(1+fSpecMomAccGen);
//   Double_t pemin=fSpecMom*(1-fSpecMomAccGen);
   Double_t thetaemax=50.*TMath::DegToRad();
  Double_t thetaemin=0.;
  Double_t pemax=4.;
  Double_t pemin=0.1;

  Double_t eel=feini->E();

  fxbmin=0.1;
  fxbmax=0.7;

  fQ2min=0.1;
  fQ2max=5.;

  Double_t nuu=-1;
  Double_t ww=-1;

  Bool_t goodkin=kFALSE;

  while(!goodkin){

    fxb=fxbmin+(fxbmax-fxbmin)*fRan->Rndm();
    fQ2=fQ2min+(fQ2max-fQ2min)*fRan->Rndm();

  // Check if it's coherent

    nuu=fQ2/2./fxb/fm;
    ww=fm*fm+2*fm*nuu-fQ2;

    if(nuu<(fEbeam-0.1)) goodkin=kTRUE;

  }


}

TVector3* TGenBase::GetVertex(void)
{
  // Returns the pointer of the vertex TVector3
  return fVertex;
}


//_____________________________________________________________________________
 Int_t TGenBase::ComputeElectron(void) 
{ 
  //Sets the scattered electron data member.
  //
  //Warning: This method generates an electron "to the left". In the elastic 
  //setting, electrons are detected in the calorimeter and then must be 
  //generated to the right. All vectors will be rotated by 180 deg. in 
  //TGenElas::ComputeElas(). Be aware of a possible error when printing 
  //electron values before and after TGenElas::ComputeElas().
  Double_t pe,thetae;//,s;
  Double_t eel=feini->E();

  Int_t success=-1;

  if (!fFermi) {
    //s=fQ2*(1.-fxb)/fxb+TMath::Power(fm,2.);
    //pe=(s-fm*fm)/(2.*fm*(fxb-1.))+eel;
    pe=-fQ2/(2.*fm*fxb)+eel;
    // If pe<0, then the electron has lost too much energy through RC
    // for the equations to be true. In reality this event will not be detected
    // so we just set the momentum of the scattered electron well outside the
    // spectrometer acceptance. The event will be however considered as
    // "generated", so that normalization can be computed correctly later.

//    if( (-fQ2/(2.*pe*eel)+1.) < -1. || (-fQ2/(2.*pe*eel)+1.) > 1. || pe < 0. )
//      {
//	cout<<"-fQ2/(2.*pe*eel)+1. = "<<(-fQ2/(2.*pe*eel)+1.)<<"       pe = "<<pe<<endl;
//      }

    if(pe<0.) pe=1000.;
    thetae=TMath::ACos(-fQ2/(2.*pe*eel)+1.);
  }else{
    // This is the solution of a yucky quadratic equation for the general case
    // inelastic scattering off a nucleon
    Double_t pz=fpini->Pz();
    Double_t px=fpini->Px();
    Double_t Ep=fpini->E();
    Double_t pz2=TMath::Power(pz,2);//
    Double_t q4=TMath::Power(fQ2,2.);
    Double_t Ep2=TMath::Power(Ep,2.);//
    Double_t k2=TMath::Power(eel,2.);
    Double_t xb2=TMath::Power(fxb,2.);
    Double_t px2=TMath::Power(px,2.);//
    Double_t k3=TMath::Power(eel,3.);
    Double_t k4=TMath::Power(k2,2.);
    Double_t pxz2=(px2 + pz2);
    pe=(eel*pz*fQ2 + 
        2*Ep2*k2*fxb + 2*k2*pz2*fxb + px2*fQ2*fxb + pz2*fQ2*fxb - 
        Ep*(eel*fQ2 + 4*k2*pz*fxb + pz*fQ2*fxb) - 
        px*TMath::Sqrt(fQ2*fxb)*TMath::Sqrt(2*eel*pz*fQ2 + 4*k2*pz2*fxb + 
	Ep2*(4*k2 - fQ2)*fxb + pxz2*fQ2*fxb - 
	2*Ep*eel*(fQ2 + 4*eel*pz*fxb)))/(2.*eel*TMath::Power(Ep - pz,2.)*fxb);
 
    
    if(pe<0. || isnan(pe)==1) pe=1000.; // See commentary just above

    thetae=(4*k3*(-Ep + pz)*fQ2*fxb + eel*(Ep + pz)*q4*fxb + 
       4*k4*TMath::Power(Ep - pz,2.)*xb2 + Ep*pz*q4*xb2 + k2*fQ2*
       (fQ2 + 2.*(-Ep2 + pz2)*xb2) - px*TMath::Power(fQ2*fxb,1.5)*
       TMath::Sqrt(2*eel*pz*fQ2 + 4*k2*pz2*fxb + 
       Ep2*(4*k2 - fQ2)*fxb + pxz2*fQ2*fxb - 
       2*Ep*eel*(fQ2 + 4*eel*pz*fxb)))/(4*k3*(-Ep + pz)*fQ2*fxb + 
       2*eel*pz*q4*fxb + 4*k4*TMath::Power(Ep - pz,2.)*xb2 + 
       pxz2*q4*xb2 + k2*fQ2*(fQ2 + 4*pz*(-Ep + pz)*xb2));
    thetae=TMath::ACos(thetae);
    if(isnan(thetae)) thetae=3.14;// See commentary just above
  }

  // cout<<"fe ptr = "<<fe<<endl;
  // cout<<"fq ptr = "<<fq<<endl;
    
  if(!fe) {
    fe=new TLorentzVector(pe*TMath::Sin(thetae),0.,pe*TMath::Cos(thetae),pe);
  }else{
    fe->SetPxPyPzE(pe*TMath::Sin(thetae),0.,pe*TMath::Cos(thetae),pe);
  }
  if(!fq) {
  fq=new TLorentzVector(feini->Px()-fe->Px(),feini->Py()-fe->Py(),feini->Pz()-fe->Pz(),feini->E()-fe->E());
  }else{
    fq->SetPxPyPzE(feini->Px()-fe->Px(),feini->Py()-fe->Py(),feini->Pz()-fe->Pz(),feini->E()-fe->E());
  }

  if(feini->E()<fe->E()) {success=0;} 
  else {success=1;}

  return success;

}

//_____________________________________________________________________________
TLorentzVector* TGenBase::GenFermiIni(void)
{   
  // Generates an initial (later recoil) particle according to a fermi distribution
  // previously read from a file via the Init() method.
  

  if(!fgIsInit) Init();

  if(fdmom==0 || frho==0){
    cout<<"You must initialize the event first !"<<endl;
    exit(1);
  }
  
  Double_t costhetfermi=2.*fRan->Rndm()-1.;
  Double_t thetfermi=TMath::ACos(costhetfermi);
  Double_t phifermi=2.*TMath::Pi()*fRan->Rndm();
  Int_t keepfermi = 0;
  Double_t slope,rho_p,pfermi;
  
  while (!keepfermi) {
    pfermi=1000.*fRan->Rndm(); // in MeV
    Int_t nn=0;
    while(pfermi > fdmom[nn]) {
      nn++;
    }
    
    if (nn<200 && nn>0) {
      slope=(frho[nn-1]-frho[nn])/(fdmom[nn-1]-fdmom[nn]);
      rho_p=slope*pfermi+frho[nn]-slope*fdmom[nn];
    } else if (nn==0) {
      rho_p=0.;
    } else {
      rho_p=0.;
    }
    
    Double_t randn2 = fRan->Rndm();
    if (randn2<rho_p) keepfermi=1;
  }
  pfermi*=0.001;  // In GeV
  
  Double_t px=pfermi*TMath::Sin(thetfermi)*TMath::Cos(phifermi);
  Double_t py=pfermi*TMath::Sin(thetfermi)*TMath::Sin(phifermi);
  Double_t pz=pfermi*TMath::Cos(thetfermi);
  Double_t Ep=TMath::Sqrt(px*px+py*py+pz*pz+fm*fm);

  if(!fpini){
    fpini=new TLorentzVector(px,py,pz,Ep);
  }else{
    fpini->SetPxPyPzE(px,py,pz,Ep);
  }
  return fpini;

}

//_____________________________________________________________________________
 TLorentzVector* TGenBase::GetScatteredElectron(void)
{
  // Returns the scattered electron 4-vector if it exists
  
  if(!fe) cout<<"Warning : Scattered electron doesn't exist"<<endl;
  return fe;

}

 TLorentzVector* TGenBase::GetInitialElectron(void)
{
  // Returns the Initial electron 4-vector if it exists
  
  if(!feini) cout<<"Warning : initial electron doesn't exist"<<endl;
  return feini;
}

//_____________________________________________________________________________
 void TGenBase::ApplySpecVerAcc(Double_t aav)
{
  // Applies vertical spectrometer acceptance by rotating the scattered 
  // electron around the beam axis

  fe->RotateZ(aav);
  feprerad->RotateZ(aav);

}

//_____________________________________________________________________________
 void TGenBase::Init(void)
{
  // Initializes the fermi momentum spread. Reads from file "deut_momdist.dat"
  cout<<"this"<<endl;
  if (fFermi) {
    fdmom=new Double_t[200];
    frho=new Double_t[200];
    ifstream deutmomdist("deut_momdist.dat");
    cout << "Using Fermi momentum spread (D2 target)" << endl;
    if(!deutmomdist) {
      cout << "Cannot open file deut_momdist.dat";
      exit(1);
    }
    for(Int_t ind=0;ind<200;ind++) {
      deutmomdist >> fdmom[ind];
      deutmomdist >> frho[ind];
    }
  }
  fgIsInit=kTRUE;
}

//_____________________________________________________________________________
 void TGenBase::Clear(char* opt)
{
  feini->SetPxPyPzE(0.,0.,fEbeam,fEbeam);
  if (fpini) fpini->SetPxPyPzE(0.,0.,0.,fm);
}

//_____________________________________________________________________________
void TGenBase::CloseTmpFile()
{
  output->close();
}

//_____________________________________________________________________________
void TGenBase::DumpFinalFile(char* finalfile, Int_t Ngen, Int_t Nacc)
{

  cout<<"Dumping final file..."<<endl;
  ifstream input("out.txt.tmp");
  ofstream totofile(finalfile);
  totofile<<Ngen<<endl;
  totofile<<Nacc<<endl;
  totofile<<fEbeam<<" "<<fSpecAngle<<" "<<fSpecMom<<endl;
  totofile<<fCaloDist<<" "<<fCaloAngle<<" "<<endl;
  totofile<<fTargType<<" "<<fTargLength<<" "<<fTargDens<<endl;
  totofile<<fFermi<<" "<<fRadCor<<endl;

  Double_t dum[fNwrite];

  //  cout << fNwrite << endl;

  if(Nacc>0){ 
    while(!input.eof()){
      for(Int_t i=0;i<fNwrite;i++){
	input>>dum[i];
      }    
      if(!input.eof()) {
	for(Int_t i=0;i<fNwrite;i++){
	  totofile<<dum[i]<<" ";
	}
	totofile<<endl;
      }
    }
  }
  remove("out.txt.tmp");
  totofile.close();
}

//_____________________________________________________________________________
 void TGenBase::Print(char* opt)
{
  // Output on screen

  cout<<"================================================================="<<endl;
  cout<<"TGenBase"<<endl;
  cout<<"================================================================="<<endl;
  cout<<"Energy beam : "<<fEbeam<<endl;
  cout<<"------"<<endl;
  if(fFermi){
    cout<<"Fermi momentum enable"<<endl;
  }else{
    cout<<"Fermi momentum disable"<<endl;
  }
  if(fRadCor){
    cout<<"Radiative corrections enable"<<endl;
  }else{
    cout<<"Radiative corrections disable"<<endl;
  }
  cout<<"------"<<endl;
  if(fgWarnings){
    cout<<"Warnings enable"<<endl;
  }else{
    cout<<"Warnings disable"<<endl;
  }
  if(fgErrors){
    cout<<"Errors enable"<<endl;
  }else{
    cout<<"Errors disable"<<endl;
  }
  cout<<"================================================================="<<endl;
}
