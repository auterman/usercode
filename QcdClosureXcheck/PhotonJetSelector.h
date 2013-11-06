//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct 30 10:48:22 2013 by ROOT version 5.30/00
// from TChain photonJetTree/
//////////////////////////////////////////////////////////

#ifndef PhotonJetSelector_h
#define PhotonJetSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

class PhotonJetSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Double_t        met;
   Double_t        ht;
   Double_t        pt;
   Double_t        weight;

   // List of branches
   TBranch        *b_met;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_weight;   //!

   PhotonJetSelector(TTree * /*tree*/ =0) { }
   virtual ~PhotonJetSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(PhotonJetSelector,0);
};

#endif

#ifdef PhotonJetSelector_cxx
void PhotonJetSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
}

Bool_t PhotonJetSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef PhotonJetSelector_cxx
