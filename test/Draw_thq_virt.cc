#include <TF1.h>
#include <TCanvas.h>

void Draw_thq_virt()
{
  TCanvas *c1 = new TCanvas("c1", "", 750, 750);

  TF1 *f_thq = new TF1("f_thq", "acos((1 + [0]/( 0.9383*x ))/( 2*[0]*sqrt( 1/(4*0.9383*0.9383*x*x) + 1./[1] ) ) )*TMath::RadToDeg()", 0.1, 1.);
  
  double Eb = 5.75;
  double Q2 = 2.7;
  double xB_min = Q2/(2*0.9383*Eb);
  cout<<"xB+min = "<<xB_min<<endl;
  f_thq->SetParameters(Eb, Q2);
  f_thq->SetRange(0.29, 1.);

  f_thq->Draw();
}
