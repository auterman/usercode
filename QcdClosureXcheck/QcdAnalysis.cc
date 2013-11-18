#include <TChain.h>
#include <TEnv.h>
#include <TSystem.h>
#include <TProof.h>
#include <TString.h>

#include <string>
#include <iostream>

//#include "Files/MySelector.h"
//#include "Files/PhotonSelector.h"

#define PROOF false

void QcdAnalysis()
{

  std::string mydir   = (gSystem->GetFromPipe("pwd")).Data(); 
  std::string rootdir = (gSystem->GetFromPipe("echo $ROOTSYS")).Data();

  TChain chain("photonTree");

  chain.Add( (mydir+"/data/subSampleFromGJets_200_400_V02.35_tree.root").c_str() );
  chain.Add( (mydir+"/data/subSampleFromGJets_400_inf_V02.35_tree.root").c_str() );
 
  int nevents = chain.GetEntries();

  gSystem->Load( (rootdir+"/lib/libPhysics.so").c_str() );
  gSystem->Load( (mydir+"/Files/PhotonSelector_cc.so").c_str() );  
  gSystem->Load( (mydir+"/Files/MySelector_cc.so").c_str() );  

  TProof *p = 0;
  if (PROOF) {
    TString connect = gSystem->GetFromPipe("pod-info -c");
    p = TProof::Open(connect);
    gEnv->SetValue("Packetizer.MaxWorkersPerNode", 2);
    //  p->SetProgressDialog(1);
    chain.SetProof(1);
 
    p->Exec( "gSystem->Load( \"/afs/naf.desy.de/products/root/amd64_rhel50/5.34.00/lib/libPhysics.so\" );" );
    p->Exec( "gSystem->Load( \"/afs/naf.desy.de/user/a/auterman/usercode/QcdClosureXcheck/Files/PhotonSelector_cc.so\" );" );
    p->Exec( "gSystem->Load( \"/afs/naf.desy.de/user/a/auterman/usercode/QcdClosureXcheck/Files/MySelector_cc.so\" );" );
  }

  chain.Process("MySelector", TString::Format(	"Hello=World \
  					     	 nevents=%d",    //either like this or with \n
					      nevents ));
  //chain.Process("PhotonSelector");
  if (PROOF && p) p->Close();
}

int main()
{
  QcdAnalysis();
  return 0;
}
