#include <TF2.h>
#include <iostream>
#include "TGVKelly.h"
#include <TCanvas.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH2D.h>
#include <fstream>

using namespace std;

//Initialization of TGV and read data file

Double_t V=new Double_t***[8];
for(Int_t i=0;i<8;i++){
  V[i]=new Double_t**[10];
  for(Int_t j=0;j<10;j++){
    V[i][j]=new Double_t*[100];
    for(Int_t k=0;k<100;k++) V[i][j][k]=new Double_t[90];
  }
 }

Double_t *CFF=new Double_t[8];

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
//===========================================================

double BH(double *, double *);

Double_t* Interpol_CFF(Double_t, Double_t, Double_t );


int main()
//int test_drawings()
{
  gSystem->Load("libTGVKelly.so");
  cout<<"Kuku"<<endl;
  
  TFile *file_out = new TFile("test_drawings.root", "Recreate");

  TCanvas *c1 = new TCanvas();

  const double xB = 0.39;
  const double Q2 = 3.2;
  double tmp;

  TF2 *f_BH = new TF2("f_BH", BH, 0., 360., -1., 0., 2);
  f_BH->SetNpx(500);
  f_BH->SetNpy(500);
  f_BH->SetParameters(Q2, xB);
  f_BH->Draw("colz");
  TH2D *h_BH = (TH2D*)f_BH->GetHistogram();
  h_BH->SetName("h_BH");
  cout<<"KUKUKUKU"<<endl;
  cout<<f_BH->Eval(-0.23, 131)<<endl;
  
  f_BH->Write();
  h_BH->Write();
  file_out->Close();
}

double BH(double *x, double *par)
{
  const double Eb = 6.6;

  TGVKelly *tgv2=new TGVKelly(Eb,kFALSE,kTRUE);

  double Q2 = par[0];
  double xb = par[1];

  double phi = x[0]*TMath::DegToRad();
  double t = x[1];

  cout<<"Q2="<<Q2<<"  xB="<<xb<<"  t="<<t<<"  phi="<<phi<<endl;

  double BHp = tgv2->CrossSectionBH( Q2, xb, t, -phi, 1, 0, kTRUE );
  double BHm = tgv2->CrossSectionBH( Q2, xb, t, -phi, -1, 0, kTRUE );

  cout<<"BHp = "<<BHp<<"  BHm = "<<BHm<<endl;

  double crs = BHm + BHp;
  if( crs > 0 )
    {
      return crs;
    }
  else return 0;
  delete tgv2;
}


Double_t* Interpol_CFF(Double_t Q2, Double_t xb, Double_t t);
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
