{
   TChain chain("@TREENAME");
   chain.Add("@INPUTROOTFILE");
   @TREENAME->MakeSelector("@SELECTORNAME");
}
