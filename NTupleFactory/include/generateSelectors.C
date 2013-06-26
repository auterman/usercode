{
   TChain chain("susyTree");
   chain.Add("@INPUTROOTFILE");
   susyTree->MakeSelector("@TREENAME");
}
