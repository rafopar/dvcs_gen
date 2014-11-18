#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include "TDVCSDB.h"
#include "TGenElas.h"
#include <TMath.h>

#include <iostream>

using namespace std;

int main()
{
  int run_number = 7878;
  const int n_sim = 10000;
  const double radian = TMath::RadToDeg();
  
  TFile *file_out = new TFile("elast_gen.root", "Recreate");

  TH1D *h_th_em = new TH1D("h_th_em", "", 200, 10., 40.);
  TH1D *h_th_prot = new TH1D("h_th_prot", "", 200, 10., 40.);
  TH1D *h_P_prot = new TH1D("h_P_prot", "", 200, 0.5, 4.5);
  
  TH2D *h_phi_vz_prot1 = new TH2D("h_phi_vz_prot1", "", 200, -9., 9., 200, 35.6, 38.);

  TDVCSDB *db=new TDVCSDB("dvcs","clrlpc",3306,"munoz","");
  //Reading the database for this run number
  Double_t Ebeam=*db->GetEntry_d("BEAM_param_Energy",run_number);
  Double_t calo_dist=*db->GetEntry_d("CALO_geom_Dist",run_number);
  Double_t calo_angle=*db->GetEntry_d("CALO_geom_Yaw",run_number);
  Double_t HRS_angle=*db->GetEntry_d("SIMU_param_HRSangle",run_number);
  Double_t HRS_mom=*db->GetEntry_d("SIMU_param_HRSmomentum",run_number);
  Int_t targettype=*db->GetEntry_i("SIMU_param_TargetType",run_number);
  int genprocess=*db->GetEntry_i("SIMU_param_GenProcess",run_number);

  cout<<"Run "<<run_number<<endl;
  cout<<"Eb = "<<Ebeam<<" GeV "<<endl;
  cout<<"calo_dist ="<<calo_dist<<" cm"<<endl;
  cout<<"calo_angle = "<<calo_angle*radian<<" deg"<<endl;
  cout<<"HRS_angle = "<<HRS_angle*radian<<" deg"<<endl;
  cout<<"HRS_mom = "<<HRS_mom<<" GeV"<<endl;
  cout<<"target_type = "<<targettype<<endl;

  TGenElas *gEv = new TGenElas(Ebeam, genprocess, int(10), int(20)); // P target
  gEv->SetTargetParam(15., 0., 0.0723);
  gEv->SetGeometry(HRS_angle, HRS_mom, calo_angle, calo_dist); 
  //  gEv->SetGeometry(37./radian, 2.406, calo_angle, calo_dist); 

  for( int i = 0; i < n_sim; i++ )
    {  
      gEv->GenerateVertex();//Generates the vertex
      gEv->ExtBrem();//Make external pre-vertex radiative corrections
      gEv->GenKin();//Computes scattered electron kinematics
      gEv->IntRCBef();//Internal radiative corrections (before vertex)
 
      if(gEv->ComputeElectron())//Compute the scattered electron 4-vector
	{
	  gEv->IntRCAft();//Internal radiative corrections (after vertex)
	  
	  gEv->ComputeElas();
	  gEv->ApplySpecVerAcc();
	  //gEv->Print();

	  double vz = (gEv->GetVertex())->Z();
	  TLorentzVector L_em_scat = *gEv->GetScatteredElectron();
	  TLorentzVector L_prot_final = *gEv->GetFinalProton();

	  double px_P = L_prot_final.Px();
	  double py_P = L_prot_final.Py();
	  double pz_P = L_prot_final.Pz();

	  double phi_prot = atan(px_P/pz_P)*radian;

	  double th_em = atan(-L_em_scat.Px()/L_em_scat.Pz())*radian;
	  double th_prot = atan( L_prot_final.Px()/L_prot_final.Pz() )*radian;
	  double P_prot = L_prot_final.P();
	  
	  h_phi_vz_prot1->Fill(vz, phi_prot);

	  h_th_em->Fill(th_em);
	  h_th_prot->Fill(th_prot);
	  h_P_prot->Fill(P_prot);
	}
    }

  h_th_em->Write();
  h_th_prot->Write();
  h_P_prot->Write();
  h_phi_vz_prot1->Write();
}


