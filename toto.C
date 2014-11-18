{
//   example of macro to read data from an ascii file and
//   create a root file with an histogram and an ntuple.
   gROOT->Reset();
#include "Riostream.h"

   ifstream in;
// we assume a file basic.dat in the current directory
// this file has 3 columns of float data
   in.open("outpip.txt");

   Float_t x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21;
   Float_t xx[19];
   Double_t q2,s,xb;


   TLorentzVector k,kprime,p,pprime,q,qprime;

p.SetPxPyPzE(0,0,0,0.938271998);

   Int_t nlines = 0;
   TFile *f = new TFile("basic.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x1:x2:x3:x4:x5:x6:x7:x8:x9:
                                  x10:x11:x12:x13:x14:x15:x16:x17:t:tmin");

      in >> x1;
      in >> x1;
      in >> x2 >> x3 >> x4;
      in >> x2 >> x3;
      in >> x2 >> x3 >> x4;
      in >> x2 >> x3;

   while (1) {
      in >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >> x11 >> x12 >> x13 >> x14 >> x15>> x16 >> x17;


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
      xx[14]=x15;
      xx[15]=x16;
      xx[16]=x17;

      kprime.SetPxPyPzE(x3,x4,x5,sqrt(x3*x3+x4*x4+x5*x5));
      k.SetPxPyPzE(0,0,x2,x2);
      qprime.SetPxPyPzE(x9,x10,x11,sqrt(x9*x9+x10*x10+x11*x11+0.13957018*0.13957018));
      pprime.SetPxPyPzE(x12,x13,x14,sqrt(x12*x12+x13*x13+x14*x14+0.93956533*0.93956533));
      q=k-kprime;
      xx[17]=(q-qprime).M2();
      xx[18]=q.M2()+0.13957018*0.13957018-2*qprime.E()*(k.E()-kprime.E())+2*qprime.P()*q.P();

      if (!in.good()) break;

      if (nlines<5) cout << xx[0] << " " << xx[1] << " " << xx[2] << " " << xx[3] << " " << xx[4] << " " << xx[5] << " " << xx[6] << endl;


	q2=-q.M2();
	s=(q+p).M2();
	xb=q2/(s-0.938271998*0.938271998+q2);

      if ( xb>0.3 && xb<0.4 && xx[17]>-0.2) {

	cout << "---------------------------------" << endl;
	cout << "Ebeam_v="<< x2 << endl;
	cout << "k'=("<<x3 << "," << x4 << "," << x5<<")"<<endl;
	cout << "q'=("<<x9 << "," << x10 << "," << x11<<")" <<endl;
	cout << "p'=("<<x12 << "," << x13 << "," << x14<<")" <<endl;
	cout << "--------"<<endl;;
	cout << "Q2="<< q2 <<endl;
	cout << "xb="<< xb <<endl;
	cout << " s="<< s  <<endl;
	cout << " t="<< xx[17] <<endl;
	cout << "--------"<<endl;;
	cout << "mon   tmin=" << xx[16] <<endl;
	cout << "Malek tmin=" << xx[18] <<endl;
	
      }

      ntuple->Fill(xx);
      nlines++;
   }
   printf(" found %d points \n",nlines);

   in.close();

   f->Write();
}

