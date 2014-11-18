#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include "TGenGeo.h"
#include "TGenPi0.h"
#include "TDVCSDB.h"
#include "TGenDVCS.h"
#include "TGenBase.h"
#include <TDVCSEvent.h>
#include <TLorentzVector.h>

#include "kin_funcs.cc"

int main( int argc, char **argv)
{
  
  using namespace std;

  if( argc != 2 )
    {
      cout<<"The programm needs run number, please set run number as an argument"<<endl;
      cout<<"The programm will exit now"<<endl;
      exit(1);
    }   
  TDVCSEvent *dvcs_ev = new TDVCSEvent();
  dvcs_ev->SetVertex(0., 0., 0.);
  const double radian = 57.2957795130823229;
  const double Mp = 0.9383;
  const int nsim = 10000000;
  TDVCSDB *db=new TDVCSDB("dvcs","clrlpc",3306,"munoz","");
  int run_number = atoi(argv[1]);
  //Reading the database for this run number
  Double_t Ebeam=*db->GetEntry_d("BEAM_param_Energy",run_number);
  Double_t calo_dist=*db->GetEntry_d("CALO_geom_Dist",run_number);
  Double_t calo_angle=*db->GetEntry_d("CALO_geom_Yaw",run_number);
  Double_t HRS_angle=*db->GetEntry_d("SIMU_param_HRSangle",run_number);
  Double_t HRS_mom=*db->GetEntry_d("SIMU_param_HRSmomentum",run_number);
   
  double targ_length = 15; //cm 
  double targ_offs = 0; // Probably cm/dens
  double targ_dens = 0.0723;

  TGenDVCS *gEv = new TGenDVCS(Ebeam, 0, UInt_t(10), UInt_t(20)); // P target
  gEv->SetTargetParam(targ_length, targ_offs, targ_dens);
  gEv->SetGeometry(HRS_angle, HRS_mom, calo_angle, calo_dist);

  TLorentzVector L_em_scat_v;
  TLorentzVector L_scat_el;
  TLorentzVector L_b(0, 0, Ebeam, Ebeam);
  
  TLorentzVector L_final_phot, L_final_prot;
  TLorentzVector L_em_init, L_em_scat;

  double tM;
  double th, phi;

  TFile *file_out = new TFile("test_DVCS_gen.root", "Recreate");
  TH2D *h_th_tM_1 = new TH2D("h_th_tM_1", "", 200, 0., 2.5, 200, 10., 30);
  TH2D *h_th_tM_2 = new TH2D("h_th_tM_2", "", 200, 0., 3.5, 200, 0., 50);

  TH2D *h_th_em_thg_1 = new TH2D("h_th_em_thg_1", "", 200, 0., 50, 200, 5, 35);
  TH2D *h_P_em_thg_1 = new TH2D("h_P_em_thg_1", "", 200, 0., 50., 200, 1.5, 4.5);
  TH2D *h_th_em_thg_2 = new TH2D("h_th_em_thg_2", "", 200, 0., 50, 200, 5, 35);
  TH2D *h_P_em_thg_2 = new TH2D("h_P_em_thg_2", "", 200, 0., 50., 200, 1.5, 4.5);

  TH2D *h_Q2_xB1 = new TH2D("h_Q2_xB1", "", 200, 0., 0.9, 200, 0., 5.);
  TH2D *h_fQ2_fxB1 = new TH2D("h_fQ2_fxB1", "", 200, 0., 0.9, 200, 0., 5.);
  TH2D *h_Q2_xB2 = new TH2D("h_Q2_xB2", "", 200, 0., 0.9, 200, 0., 5.);

  TH2D *h_thq_tM_tmin1 = new TH2D("h_thq_tM_tmin1", "", 200, 0., 1., 200, 10., 30.);
  TH2D *h_th1_tmin1 = new TH2D("h_th1_tmin1", "", 200, 0., 1., 200, 10, 30);
  TH2D *h_thq_tM_tmin2 = new TH2D("h_thq_tM_tmin2", "", 200, 0., 1., 200, 10., 30.);
  TH2D *h_th1_tmin2 = new TH2D("h_th1_tmin2", "", 200, 0., 1., 200, 10, 30);

  for( int i = 0; i < nsim; i++ )
    {
      gEv->GenerateVertex();//Generates the vertex
      //      gEv->ExtBrem();//Make external pre-vertex radiative corrections
      gEv->GenKin();//Computes scattered electron kinematics
      //      gEv->IntRCBef();//Internal radiative corrections (before vertex)
  
      if(gEv->ComputeElectron())//Compute the scattered electron 4-vector
	{
	  L_em_scat_v = *gEv->GetScatteredElectron();
	  //	  gEv->IntRCAft();//Internal radiative corrections (after vertex)
	  L_scat_el = *gEv->GetScatteredElectron();
	  
	  L_em_init = *gEv->GetInitialElectron();
	  L_em_scat = *gEv->GetScatteredElectron();

	  double fQ2 = gEv->GetQ2();
	  double fxB = gEv->GetXb();
	  double Q2 = -(L_em_init - L_em_scat).M2();
	  double nu = (L_em_init - L_em_scat).E();
	  double xB = Q2/(2*Mp*nu);
	  Double_t q3=TMath::Sqrt(Q2+TMath::Power(nu,2.));
          Double_t q0primemax=0.5*Q2*(1.-xB)/(xB*(Mp+nu-q3));
          Double_t q0primemin=0.5*Q2*(1.-xB)/(xB*(Mp+nu+q3));
          
	  double s = -Q2 + Mp*Mp + 2*Mp*nu;
	  double t_min_calc = T_min(-Q2, Mp*Mp, 0., Mp*Mp, s);
	  
          Double_t tmax=-Q2-2.*q0primemax*(nu-q3);
          Double_t tmin=-Q2-2.*q0primemin*(nu+q3);

	  //cout<<"tmax - t_min_calc = "<<tmax - t_min_calc<<endl;

	  gEv->Settmin(tmax - 2.); //Use this method to change tmin (default -2 GeV)
	  gEv->Settmax(0.);  //Use this method to change tmax (default 0 GeV)
          
	  gEv->ComputeDVCS(); // \gamma* p -> pi0 p
	  
	  gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis
	  
	  L_final_phot = *gEv->GetFinalPhoton();
	  
	  L_final_prot = *gEv->GetFinalProton();
	  double th_em = atan(L_em_scat.Px()/L_em_scat.Pz())*radian;
	  double th_q = atan(-L_final_phot.Px()/L_final_phot.Pz())*radian;

	  double tM = (L_b - L_em_scat - L_final_phot).M2();
	  
	  h_Q2_xB1->Fill(xB, Q2);
	  h_fQ2_fxB1->Fill(fxB, fQ2);
	  h_th_tM_1->Fill(-tM, th_q);
	  
	  h_th1_tmin1->Fill(-t_min_calc, th_q);
	  h_thq_tM_tmin1->Fill(t_min_calc - tM, th_q);
	  
	  if(gEv->HitsSpectro(gEv->GetScatteredElectron()) )
	    {
	      h_Q2_xB2->Fill(xB, Q2);
	      h_th_tM_2->Fill(-tM, th_q);
	      
	      h_th1_tmin2->Fill(-t_min_calc, th_q);
	      h_thq_tM_tmin2->Fill(t_min_calc - tM, th_q);

		  if(gEv->HitsCalo(gEv->GetFinalPhoton()))
		    {
		      
		    }
	    }
	}
    }
  h_Q2_xB1->Write();
  h_Q2_xB2->Write();
  h_fQ2_fxB1->Write();
  h_th_tM_1->Write();
  h_th_tM_2->Write();
  h_th_em_thg_1->Write();
  h_P_em_thg_1->Write();
  h_th_em_thg_2->Write();
  h_P_em_thg_2->Write();
  h_th1_tmin1->Write();
  h_thq_tM_tmin1->Write();
  h_th1_tmin2->Write();
  h_thq_tM_tmin2->Write();
  file_out->Close();
  return 0;
}
