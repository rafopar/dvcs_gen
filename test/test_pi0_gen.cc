#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include "TGenGeo.h"
#include "TGenPi0.h"
#include "TDVCSDB.h"
#include "TGenBase.h"
#include <TLorentzVector.h>

int main( int argc, char **argv)
{
  
  using namespace std;

  if( argc != 2 )
    {
      cout<<"The programm needs run number, please set run number as an argument"<<endl;
      cout<<"The programm will exit now"<<endl;
      exit(1);
    }   
      
  const double radian = 57.2957795130823229;
  const int nsim = 10000000;
  TDVCSDB *db=new TDVCSDB("dvcs","clrlpc",3306,"munoz","");
  int run_number = atoi(argv[1]);
  //Reading the database for this run number
  Double_t Ebeam=*db->GetEntry_d("BEAM_param_Energy",run_number);
  Double_t calo_dist=*db->GetEntry_d("CALO_geom_Dist",run_number);
  Double_t calo_angle=*db->GetEntry_d("CALO_geom_Yaw",run_number);
  Double_t HRS_angle=*db->GetEntry_d("SIMU_param_HRSangle",run_number);
  Double_t HRS_mom=*db->GetEntry_d("SIMU_param_HRSmomentum",run_number);
  int gen_proc=*db->GetEntry_i("SIMU_param_GenProcess",run_number);
	 
  double targ_length = 15; //cm 
  double targ_offs = 0; // Probably cm/dens
  double targ_dens = 0.0723;
  double Mtarg;

  if( gen_proc == 0 )
    {
      Mtarg = 0.9383;
      targ_dens = 0.0723;
    }
  else if( gen_proc == 1 )
    {
      Mtarg = 0.939565378;
      targ_dens = 0.167;
    }
  else if( gen_proc == 2 )
    {
      Mtarg = 1.875612859;
      targ_dens = 0.167;
    }
  
  TGenPi0 *gEv = new TGenPi0(Ebeam, gen_proc, UInt_t(10), UInt_t(20)); // P target
  gEv->SetTargetParam(targ_length, targ_offs, targ_dens);
  gEv->SetGeometry(HRS_angle, HRS_mom, calo_angle, calo_dist);

  TLorentzVector L_em_scat_v;
  TLorentzVector L_scat_el;
  TLorentzVector L_b(0, 0, Ebeam, Ebeam);
  TLorentzVector L_targ(0., 0., 0., Mtarg);

  TLorentzVector L_final_phot1, L_final_phot2, L_final_prot;
  TLorentzVector L_em_init, L_em_scat;

  double tM;
  double th, phi;

  TFile *file_out = new TFile("test_pi0_gen.root", "Recreate");
  TH2D *h_th_tM_1 = new TH2D("h_th_tM_1", "", 200, 0., 2.5, 200, 10., 30);
  TH2D *h_th_tM_2 = new TH2D("h_th_tM_2", "", 200, 0., 3.5, 200, 0., 50);
  TH2D *h_th_tM_3 = new TH2D("h_th_tM_3", "", 200, 0., 2.5, 200, 0., 50);
  TH2D *h_th_tM_4 = new TH2D("h_th_tM_4", "", 200, 0., 2.5, 200, 0., 50);
  TH2D *h_th_tM_5 = new TH2D("h_th_tM_5", "", 200, 0., 2.5, 200, 0., 50);

  TH2D *h_th_em_thg_1 = new TH2D("h_th_em_thg_1", "", 200, 0., 50, 200, 5, 35);
  TH2D *h_P_em_thg_1 = new TH2D("h_P_em_thg_1", "", 200, 0., 50., 200, 1.5, 4.5);
  TH2D *h_th_em_thg_2 = new TH2D("h_th_em_thg_2", "", 200, 0., 50, 200, 5, 35);
  TH2D *h_P_em_thg_2 = new TH2D("h_P_em_thg_2", "", 200, 0., 50., 200, 1.5, 4.5);

  TH2D *h_Q2_xB1 = new TH2D("h_Q2_xB1", "", 200, 0., 0.9, 200, 0., 5.);
  TH2D *h_Q2_xB2 = new TH2D("h_Q2_xB2", "", 200, 0., 0.9, 200, 0., 5.);
  TH2D *h_Q2_xB3 = new TH2D("h_Q2_xB3", "", 200, 0., 0.9, 200, 0., 5.);
  TH2D *h_Q2_xB4 = new TH2D("h_Q2_xB4", "", 200, 0., 0.9, 200, 0., 5.);

  TH1D *h_tM1 = new TH1D("h_tM1", "", 200, 0.,3.5);
  TH1D *h_tM2 = new TH1D("h_tM2", "", 200, 0., 3.5);

  TH2D *h_Q2_tM1 = new TH2D("h_Q2_tM1", "", 200, 0., 3.5, 200, 0.9, 2.4);
  TH2D *h_Q2_tM2 = new TH2D("h_Q2_tM2", "", 200, 0., 3.5, 200, 0.9, 2.4);

  for( int i = 0; i < nsim; i++ )
    {
      gEv->GenerateVertex();//Generates the vertex
      //gEv->ExtBrem();//Make external pre-vertex radiative corrections
      gEv->GenKin();//Computes scattered electron kinematics
      //gEv->IntRCBef();//Internal radiative corrections (before vertex)
  
      if(gEv->ComputeElectron())//Compute the scattered electron 4-vector
	{
	  L_em_scat_v = *gEv->GetScatteredElectron();
	  //  gEv->IntRCAft();//Internal radiative corrections (after vertex)
	  L_scat_el = *gEv->GetScatteredElectron();

	  double Q2 = gEv->GetQ2();
	  double xB = gEv->GetXb();
	  
	  h_Q2_xB1->Fill(xB, Q2);
	  
	  gEv->Settmin( -3.); //Use this method to change tmin (default -2 GeV)
	  gEv->Settmax(0.);  //Use this method to change tmax (default 0 GeV)
          
	  gEv->ComputePi0(); // \gamma* p -> pi0 p
	  gEv->TwoBodyDecay(0.1349766,0.,0.); // pi0 mass
      
	  gEv->ApplySpecVerAcc();//Rotates all final vectors around the beam axis
	  
	  double psf = gEv->GetPSF();

	  L_final_phot1 = *gEv->GetFinalPhoton1();
	  L_final_phot2 = *gEv->GetFinalPhoton2();
	      
	  L_final_prot = *gEv->GetFinalProton();
	  L_em_init = *gEv->GetInitialElectron();
	  L_em_scat = *gEv->GetScatteredElectron();
	  double th_em = atan(L_em_scat.Px()/L_em_scat.Pz())*radian;
	  
	  double t_exact = (L_targ - L_final_prot).M2();
	  
	  
	  h_tM1->Fill(-t_exact);
	  h_tM2->Fill(-t_exact, psf);
	  if( xB > 0.4 && xB < 0.402 )
	    {
	      h_Q2_tM1->Fill(-t_exact, Q2);
	      h_Q2_tM2->Fill(-t_exact, Q2, psf);
	    }
	  

	  if( L_final_phot1.E() > L_final_phot2.E() )
	    {
	      tM = (L_em_init - L_em_scat - L_final_phot1).M2();
	      th = -atan(L_final_phot1.Px()/L_final_phot1.Pz())*radian;
	    }
	  else
	    {
	      tM = (L_em_init - L_em_scat - L_final_phot2).M2();
	      th = -atan(L_final_phot2.Px()/L_final_phot2.Pz())*radian;
	    }

	  if( xB > 0.35 && xB < 0.375 && Q2 > 1.87 && Q2 < 1.95)
	    {
	      h_th_tM_3->Fill(-tM, th);
	    }
	  
	  if( xB > 0.21 && xB < 0.235 && Q2 > 1.14 && Q2 < 1.22 )
	    {
	      h_th_tM_4->Fill(-tM, th);
	    }

	  if( xB > 0.53 && xB < 0.555 && Q2 > 2.74 && Q2 < 2.82 )
	    {
	      h_th_tM_5->Fill(-tM, th);
	    }

	  if(gEv->HitsSpectro(gEv->GetScatteredElectron()) && (gEv->HitsCalo(gEv->GetFinalPhoton1()) || gEv->HitsCalo(gEv->GetFinalPhoton2())) )
	    {
	      h_Q2_xB2->Fill(xB, Q2);
	      
	      if( -tM < 0.21 )
		{
		  h_Q2_xB3->Fill(xB, Q2);
		}

	      if( th > -21*tM + 16. && th < 30 )
		{
		  h_Q2_xB4->Fill(xB, Q2);
		}

	      h_th_tM_1->Fill(-tM, th);
	      h_th_tM_2->Fill(-tM, th);
	      h_th_em_thg_1->Fill(th, th_em);
	      h_P_em_thg_1->Fill(th, L_em_scat.P());
	      if( -tM < 0.3 )
		{
		  h_th_em_thg_2->Fill(th, th_em);
		  h_P_em_thg_2->Fill(th, L_em_scat.P());
		}
	    }
	}
    }
  h_Q2_xB1->Write();
  h_Q2_xB2->Write();
  h_Q2_xB3->Write();
  h_Q2_xB4->Write();
  h_th_tM_1->Write();
  h_th_tM_2->Write();
  h_th_tM_3->Write();
  h_th_tM_4->Write();
  h_th_tM_5->Write();
  h_th_em_thg_1->Write();
  h_P_em_thg_1->Write();
  h_th_em_thg_2->Write();
  h_P_em_thg_2->Write();
  h_tM1->Write();
  h_tM2->Write();
  h_Q2_tM1->Write();
  h_Q2_tM2->Write();
  file_out->Close();
  return 0;
}