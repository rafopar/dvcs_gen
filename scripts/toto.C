{
//   example of macro to read data from an ascii file and
//   create a root file with an histogram and an ntuple.
   gROOT->Reset();
#include "Riostream.h"

   ifstream in;
// we assume a file basic.dat in the current directory
// this file has 3 columns of float data
   in.open("out1.txt");

   Float_t x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14;
   Float_t mmx2;
   Float_t xx[19];
   Int_t nlines = 0;
   TFile *f = new TFile("basic.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","vz:ebeam:kpx:kpy:kpz:qpx:qpy:qpz:ppx:ppy:ppz:sigp:sigm:psf:xb:q2:t:w2:phi");

   TLorentzVector k(0,0,11,11);
   TLorentzVector p(0,0,0,0.938272);
   TLorentzVector pprime,kprime,qprime,q;
   Float_t egam,oldegam;

   TRandom titi;

   while (1) {
      in >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >> x11 >> x12 >> x13 >> x14;

      xx[0]=x1;
      xx[1]=x2;
      xx[2]=x3;
      xx[3]=x4;
      xx[4]=x5;
      xx[5]=x6;
      xx[6]=x7;
      xx[7]=x8;
      xx[8]=x9;
      xx[9]=x10;
      xx[10]=x11;
      xx[11]=x12;
      xx[12]=x13;
      xx[13]=x14;

      kprime.SetPxPyPzE(x3,x4,x5,TMath::Sqrt(x3*x3+x4*x4+x5*x5));
      q=k-kprime;

      //     oldegam=TMath::Sqrt(x12*x12+x13*x13+x14*x14);
      //      egam=titi.Gaus(oldegam,0.05*oldegam);

      egam=TMath::Sqrt(x6*x6+x7*x7+x8*x8);

      qprime.SetPxPyPzE(x6,x7,x8,egam);

      xx[15]=-q.M2();
      xx[14]=xx[15]/2./0.938272/(11.-kprime.E());
      xx[16]=(q-qprime).M2();
      xx[17]=0.938272*0.938272+2.*0.938272*(11.-kprime.E())-xx[15];
      xx[18]=0;

      TLorentzVector mm=q+p-qprime;
      mmx2=mm.M2();

      if (!in.good()) break;

      if (nlines<5) cout << mmx2 << endl;

      ntuple->Fill(xx);
      nlines++;
   }
   printf(" found %d points \n",nlines);

   in.close();

   f->Write();
}

