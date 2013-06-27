{
   TChain chain("susyTree");
   chain.Add("/.automount/home/home__home4/institut_1b/auterman/Programs/usercode/ProofAnalysis/input/QCD_250_500_V01.07_tree.root");
   chain.Add("/.automount/home/home__home4/institut_1b/auterman/Programs/usercode/ProofAnalysis/input/QCD_500_1000_V01.07_tree.root");
   chain.Add("/.automount/home/home__home4/institut_1b/auterman/Programs/usercode/ProofAnalysis/input/QCD_1000_inf_V01.07_tree.root");
 
   chain.SetProof();
   
   TProof *p = TProof::Open("lite://");
   chain.Process("QcdSelector.cc+");
}
