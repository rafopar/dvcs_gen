#include <TF2.h>
#include <iostream>
#include "TGVKelly.h"
#include <TCanvas.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH2D.h>
#include <fstream>
#include <TRandom.h>

using namespace std;

//Initialization of TGV and read data file

Double_t ****V=new Double_t***[8];
Double_t *CFF=new Double_t[8];

//===========================================================

void Read_File();
Double_t* Interpol_CFF(Double_t, Double_t, Double_t );
double BH(double *, double *);
double VCS_INT_sum(double *, double *);
double VCS_INT_dif(double *, double *);

int main()
{
  Read_File();
  
  const double Eb = 11.;

  TRandom *rand1 = new TRandom();

  TGVKelly *tgv2=new TGVKelly(Eb,kFALSE,kTRUE);
  
  for( int i = 0; i < 25; i++ ){

    
    double fQ2 = rand1->Uniform(2., 2.5);// 1.5;// par[0];
    double fxb = rand1->Uniform(0.31, 0.65); //0.39;// par[1];

    double fphi = rand1->Uniform(0.2, 3.1); //1.18;// x[0]*TMath::DegToRad();
    double ft = rand1->Uniform(-1.12, -0.12); //-0.23;//x[1];

    Double_t* check=Interpol_CFF(fQ2,fxb,ft);
    if(!check) return 0;
  

    Double_t ConvGeV2nbarn = 0.389379304e+6;
    double BHm, BHp;
    Double_t VCSp, VCSm, Ip, Im;
    Double_t SigmaTotPlus, SigmaTotMoins;
    BHp = tgv2->CrossSectionBH( fQ2, fxb, ft, -fphi, 1, 0, kTRUE );
    VCSp = tgv2->CrossSectionVCS( fQ2, fxb, ft, -fphi, 1, 0, CFF[0], CFF[1], CFF[2], CFF[3], CFF[4], CFF[5], CFF[6], CFF[7], kTRUE );
    Ip = tgv2->CrossSectionInterf( fQ2, fxb, ft, -fphi, 1, 0, -1, CFF[0], CFF[1], CFF[2], CFF[3], CFF[4], CFF[5], CFF[6], CFF[7], kTRUE );
    BHm = tgv2->CrossSectionBH( fQ2, fxb, ft, -fphi, -1, 0, kTRUE );
    VCSm = tgv2->CrossSectionVCS( fQ2, fxb, ft, -fphi, -1, 0, CFF[0], CFF[1], CFF[2], CFF[3], CFF[4], CFF[5], CFF[6], CFF[7], kTRUE );
    Im = tgv2->CrossSectionInterf( fQ2, fxb, ft, -fphi, -1, 0, -1, CFF[0], CFF[1], CFF[2], CFF[3], CFF[4], CFF[5], CFF[6], CFF[7], kTRUE );
    SigmaTotPlus = VCSp + Ip;
    SigmaTotMoins = VCSm + Im;

    cout<<"BHp = "<<BHp<<"  BHm="<<BHm<<"   VCSp="<<VCSp<<"   Ip="<<Ip<<endl;
  
    double crs = TMath::Pi()*(SigmaTotPlus + SigmaTotMoins)*ConvGeV2nbarn;
    // cout<<CFF[0]<<" "<<CFF[1]<<" "<<CFF[2]<<" "<<CFF[3]<<" "<<CFF[4]<<" "<<CFF[5]<<" "<<CFF[6]<<" "<<CFF[7]<<endl;
    // cout<<" here "<<fQ2<<" "<<fxb<<" "<<ft<<" "<<fphi<<" "<<TMath::Pi() * ( SigmaTotPlus + SigmaTotMoins ) * ConvGeV2nbarn<<endl;
  }
  delete tgv2;

}


void Read_File()
{
  cout<<"Reading file"<<endl;
  for(Int_t i=0; i<8; i++){
    V[i]=new Double_t**[10];
    for(Int_t j=0;j<10;j++){
      V[i][j]=new Double_t*[100];
    for(Int_t k=0;k<100;k++) {V[i][j][k]=new Double_t[90];}
    }
  }
  
  ifstream f("./CFFoutput_LO.dat");
  Double_t dum1,dum2,dum3;
  for ( register unsigned int iQ2 = 0; iQ2 < 10; iQ2++ ) {
    for ( register unsigned int iXb = 0; iXb < 100; iXb++ ) {
      for ( register unsigned int it = 0; it < 90; it++ ) {
      f>>dum1>>dum2>>dum3;
      for(Int_t j=0;j<8;j++) f>>V[j][iQ2][iXb][it];
      }
    }
  }
  cout<<"file Read"<<endl;
}


Double_t* Interpol_CFF(Double_t Q2, Double_t xb, Double_t t)
{

  //  if(Q2<1||Q2>10||xb<0.2||xb>0.7||t>tmin||t<tmin-1)
  // cout<<Q2<<" "<<xb<<" "<<t<<" ==========="<<endl;
  Double_t eps2=4.*xb*xb*0.938272*0.938272/Q2;
  Double_t tmin = -Q2*(2.*(1.-xb)*(1-TMath::Sqrt(1+eps2))+eps2)/(4.*xb*(1.-xb)+eps2);

  if(Q2<1||Q2>10||xb<0.2||xb>0.7||t<-3.){
    cout<<"Kinematics (Q2,xb,t,tmin) out of range for cross-section evaluation: "<<Q2<<" "<<xb<<" "<<t<<" "<<tmin<<endl;
    return 0;
  }
    
  Int_t Q2_0=int(Q2-1.);
  Int_t Q2_1=int(Q2-1.)+1;
  Int_t xb_0=int( (xb-0.2)*99/0.5 );
  Int_t xb_1=int( (xb-0.2)*99/0.5 )+1;
  Int_t t_0=int( -(t-0.)*89/3. );
  Int_t t_1=int( -(t-0.)*89/3. )+1;

  Double_t Q2d=(Q2-(1+Q2_0))/((1+Q2_1)-(1+Q2_0));
  Double_t xbd=(xb-(0.2+0.5*xb_0/99.))/((0.2+0.5*xb_1/99.)-(0.2+0.5*xb_0/99.));
  Double_t td=(t-(0.-3.*t_0/89.))/((0.-3.*t_1/89.)-(0.-3.*t_0/89.));

  for(Int_t GPD=0;GPD<8;GPD++){
    
    Double_t c00=V[GPD][Q2_0][xb_0][t_0]*(1-Q2d)+V[GPD][Q2_1][xb_0][t_0]*Q2d;
    Double_t c10=V[GPD][Q2_0][xb_1][t_0]*(1-Q2d)+V[GPD][Q2_1][xb_1][t_0]*Q2d;
    Double_t c01=V[GPD][Q2_0][xb_0][t_1]*(1-Q2d)+V[GPD][Q2_1][xb_0][t_1]*Q2d;
    Double_t c11=V[GPD][Q2_0][xb_1][t_1]*(1-Q2d)+V[GPD][Q2_1][xb_1][t_1]*Q2d;
    
    Double_t c0=c00*(1-xbd)+c10*xbd;
    Double_t c1=c01*(1-xbd)+c11*xbd;
    
    CFF[GPD]=c0*(1-td)+c1*td;
  }
  return CFF;
}
