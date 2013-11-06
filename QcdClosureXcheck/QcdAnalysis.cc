#include <TChain.h>

#include "MySelector.h"
#include "PhotonSelector.h"
#include "QcdAnalysis.h"

int QcdAnalysis()
{
   TChain chain("photonTree");

   chain.Add("/Users/auterman/InSyncOwnCloud/usercode/QcdClosureXcheck/data/subSampleFromGJets_200_400_V02.35_tree.root");
//   chain.Add("/Users/auterman/InSyncOwnCloud/usercode/QcdClosureXcheck/data/subSampleFromGJets_400_inf_V02.35_tree.root");
 
   //chain.SetProof();   
   //TProof *p = TProof::Open("lite://");
   
   //chain.Process("QcdSelector.cc+");
   //p->EnablePackage("MySelector_cc.so");
   //p->EnablePackage("QcdSelector_cc.so");
   
   //chain.Process("PhotonSelector.cc+");
 
   chain.Process("PhotonSelector.cc+");
 
   chain.Process("MySelector.cc+");




   TChain fake_chain("photonJetTree");
   fake_chain.Add("/Users/auterman/InSyncOwnCloud/usercode/QcdClosureXcheck/data/subSampleFromGJets_200_400_V02.35_tree.root");
   fake_chain.Process("MySelector.cc");
}

int main()
{
  QcdAnalysis();
  return 0;
}
