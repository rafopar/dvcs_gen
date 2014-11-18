{
 THtml html;

 gSystem.Load("libTGenGeo.so");
 gSystem.Load("libTGenBase.so");
 gSystem.Load("libTGenDVCS.so");
 gSystem.Load("libTGenPi0.so");
 gSystem.Load("libTGenElas.so");

// html.MakeAll(kTRUE,"TGen*");
 html.MakeAll(kFALSE,"TGen*");

}
