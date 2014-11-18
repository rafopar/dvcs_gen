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
   Float_t xx[15];
   Int_t nlines = 0;
   TFile *f = new TFile("basic.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x1:x2:x3:x4:x5:x6:x7:x8:x9:x10:x11:x12:x13:x14:x15");

      in >> x1;
      in >> x1;
      in >> x2 >> x3 >> x4;
      in >> x2 >> x3;
      in >> x2 >> x3 >> x4;
      in >> x2 >> x3;

   while (1) {
      in >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >> x11 >> x12 >> x13 >> x14 >> x15;

  TVector3 qpt(x9+x12,x10+x13,x11+x14);
  TVector3 qt(-x6,-x7,x2-x8);
  TVector3 kt(0,0,x2);

  TVector3 qk=qt.Cross(kt);//perp at leptonic plan (initial electron,virtual photon)

  TVector3 qqp=qt.Cross(qpt);//perp at hadronic plan (final pi0, virtual photon)

  Double_t ang=(180./TMath::Pi())*(qk.Angle(qqp));//angle between the 2 plans

  if(qt.Dot(qk.Cross(qqp))<0) ang=360.-ang;

//   cout << "Phi_gen="<<fphi*180./3.141592 << endl;

//   cout << "pi0: (" << qpt.X()<<","<<qpt.Y()<<","<<qpt.Z()<<")"<<endl;

//   //  cout << "pi0 unit: (" << qpt.Unit().X()<<","<<qpt.Unit().Y()<<","<<qpt.Unit().Z()<<")"<<endl;
//   cout << "  k: (" << kt.X()<<","<<kt.Y()<<","<<kt.Z()<<")"<<endl;
//   cout << " kp: (" << fe->Px()<<","<<fe->Py()<<","<<fe->Pz()<<")"<<endl;
//   cout << "  q: (" << qt.X()<<","<<qt.Y()<<","<<qt.Z()<<")"<<endl;
//   //  cout << "  q unit: (" << qt.Unit().X()<<","<<qt.Unit().Y()<<","<<qt.Unit().Z()<<")"<<endl;
//   cout << " qk: (" << qk.X()<<","<<qk.Y()<<","<<qk.Z()<<")"<<endl;
//   cout << " qqp: (" << qqp.X()<<","<<qqp.Y()<<","<<qqp.Z()<<")"<<endl;
//   cout << "Phi_rec=" << ang << endl;
  //  cout << "Q2="<<fQ2<<endl;
  //  cout << "xb="<<fxb<<endl;
  //  cout << "t="<<ft<<endl;
  //  cout << "tmax="<<ftmax<<endl;
//   cout << "------------------" << endl;

//   cout << fphi*180./3.141592 << "   " << ang << endl;


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
//       xx[16]=x17;
//       xx[17]=x18;
//       xx[18]=ang;

      if (!in.good()) break;

      if (nlines<5) cout << xx[0] << " " << xx[1] << " " << xx[2] << " " << xx[3] << " " << xx[4] << " " << xx[5] << " " << xx[6] << endl;

      ntuple->Fill(xx);
      nlines++;
   }
   printf(" found %d points \n",nlines);

   in.close();

   f->Write();
}

