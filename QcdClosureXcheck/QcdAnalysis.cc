#include <TChain.h>
#include <TEnv.h>
#include <TSystem.h>
#include <TProof.h>
#include <TString.h>

#include <string>
#include <iostream>

//#include "Files/MySelector.h"
//#include "Files/PhotonSelector.h"

void QcdAnalysis()
{
   std::string mydir   = "/afs/naf.desy.de/user/a/auterman/usercode/QcdClosureXcheck"; //pwd
   std::string rootdir = "/afs/naf.desy.de/products/root/amd64_rhel50/5.34.00"; //echo $ROOTSYS

   TChain chain("photonTree");

   chain.Add( (mydir+"/data/subSampleFromGJets_200_400_V02.35_tree.root").c_str() );
   chain.Add( (mydir+"/data/subSampleFromGJets_400_inf_V02.35_tree.root").c_str() );
 
/*
  Long64_t nentries = c.GetEntries();
  Int_t cachesize = 200000000; //100 MBytes
  c.SetCacheSize(cachesize);     //<<<
  TString connect = gSystem.GetFromPipe("pod-info -c");
  TProof *p = TProof::Open(connect);
  gEnv->SetValue("Packetizer.MaxWorkersPerNode", 2);
  p->SetProgressDialog(1);
  c.SetProof(1);

  //cout<< "/afs/naf.desy.de/user/a/auterman/usercode/NafProofExample2/files/ZJetSel_cc.so" << endl;

  gSystem->Load( "/afs/naf.desy.de/user/a/auterman/usercode/NafProofExample2/files/ZJetSel_cc.so" );  
  p->Exec( "gSystem->Load( \"/afs/naf.desy.de/user/a/auterman/usercode/NafProofExample2/files/ZJetSel_cc.so\" );" );

  c.Process("ZJetSel","");
  p->Close();
*/  
  int nevents = chain.GetEntries();
  std::cout << "nevents: "<< nevents << std::endl;

  TString connect = gSystem->GetFromPipe("pod-info -c");
  TProof *p = TProof::Open(connect);
  gEnv->SetValue("Packetizer.MaxWorkersPerNode", 2);
//  p->SetProgressDialog(1);
  chain.SetProof(1);

  gSystem->Load( (rootdir+"/lib/libPhysics.so").c_str() );
  gSystem->Load( (mydir+"/Files/PhotonSelector_cc.so").c_str() );  
  gSystem->Load( (mydir+"/Files/MySelector_cc.so").c_str() );  
 
  p->Exec( "gSystem->Load( \"/afs/naf.desy.de/products/root/amd64_rhel50/5.34.00/lib/libPhysics.so\" );" );
  p->Exec( "gSystem->Load( \"/afs/naf.desy.de/user/a/auterman/usercode/QcdClosureXcheck/Files/PhotonSelector_cc.so\" );" );
  p->Exec( "gSystem->Load( \"/afs/naf.desy.de/user/a/auterman/usercode/QcdClosureXcheck/Files/MySelector_cc.so\" );" );

  chain.Process("MySelector", TString::Format("Hello=World nevents=%d",nevents ));
  //chain.Process("PhotonSelector");
  p->Close();


}

int main()
{
  QcdAnalysis();
  return 0;
}
