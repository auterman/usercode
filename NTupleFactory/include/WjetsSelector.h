//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 28 20:13:56 2013 by ROOT version 5.30/00
// from TChain susyTree/
//////////////////////////////////////////////////////////

#ifndef WjetsSelector_h
#define WjetsSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
   const Int_t kMaxphoton = 8;
   const Int_t kMaxjet = 16;
   const Int_t kMaxelectron = 2;
   const Int_t kMaxmuon = 3;

class WjetsSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           photon_;
   Float_t         photon_pt[kMaxphoton];   //[photon_]
   Float_t         photon_eta[kMaxphoton];   //[photon_]
   Float_t         photon_phi[kMaxphoton];   //[photon_]
   Float_t         photon_r9[kMaxphoton];   //[photon_]
   Float_t         photon_sigmaIetaIeta[kMaxphoton];   //[photon_]
   Float_t         photon_hadTowOverEm[kMaxphoton];   //[photon_]
   Float_t         photon_pixelseed[kMaxphoton];   //[photon_]
   Float_t         photon_chargedIso[kMaxphoton];   //[photon_]
   Float_t         photon_neutralIso[kMaxphoton];   //[photon_]
   Float_t         photon_photonIso[kMaxphoton];   //[photon_]
   Bool_t          photon_conversionSafeVeto[kMaxphoton];   //[photon_]
   Int_t           jet_;
   Float_t         jet_pt[kMaxjet];   //[jet_]
   Float_t         jet_eta[kMaxjet];   //[jet_]
   Float_t         jet_phi[kMaxjet];   //[jet_]
   Float_t         jet_bCSV[kMaxjet];   //[jet_]
   Float_t         jet_chargedHadronEnergy[kMaxjet];   //[jet_]
   Float_t         jet_neutralHadronEnergy[kMaxjet];   //[jet_]
   Float_t         jet_photonEnergy[kMaxjet];   //[jet_]
   Float_t         jet_electronEnergy[kMaxjet];   //[jet_]
   Float_t         jet_muonEnergy[kMaxjet];   //[jet_]
   Float_t         jet_HFHadronEnergy[kMaxjet];   //[jet_]
   Float_t         jet_HFEMEnergy[kMaxjet];   //[jet_]
   Float_t         jet_chargedEmEnergy[kMaxjet];   //[jet_]
   Float_t         jet_chargedMuEnergy[kMaxjet];   //[jet_]
   Float_t         jet_neutralEmEnergy[kMaxjet];   //[jet_]
   Int_t           electron_;
   Float_t         electron_pt[kMaxelectron];   //[electron_]
   Float_t         electron_eta[kMaxelectron];   //[electron_]
   Float_t         electron_phi[kMaxelectron];   //[electron_]
   Int_t           muon_;
   Float_t         muon_pt[kMaxmuon];   //[muon_]
   Float_t         muon_eta[kMaxmuon];   //[muon_]
   Float_t         muon_phi[kMaxmuon];   //[muon_]
   Float_t         met;
   Float_t         metPhi;
   Float_t         type1met;
   Float_t         type1metPhi;
   Float_t         ht;
   Int_t           nVertex;
   Float_t         pu_weight;

   // List of branches
   TBranch        *b_photon_;   //!
   TBranch        *b_photon_pt;   //!
   TBranch        *b_photon_eta;   //!
   TBranch        *b_photon_phi;   //!
   TBranch        *b_photon_r9;   //!
   TBranch        *b_photon_sigmaIetaIeta;   //!
   TBranch        *b_photon_hadTowOverEm;   //!
   TBranch        *b_photon_pixelseed;   //!
   TBranch        *b_photon_chargedIso;   //!
   TBranch        *b_photon_neutralIso;   //!
   TBranch        *b_photon_photonIso;   //!
   TBranch        *b_photon_conversionSafeVeto;   //!
   TBranch        *b_jet_;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_bCSV;   //!
   TBranch        *b_jet_chargedHadronEnergy;   //!
   TBranch        *b_jet_neutralHadronEnergy;   //!
   TBranch        *b_jet_photonEnergy;   //!
   TBranch        *b_jet_electronEnergy;   //!
   TBranch        *b_jet_muonEnergy;   //!
   TBranch        *b_jet_HFHadronEnergy;   //!
   TBranch        *b_jet_HFEMEnergy;   //!
   TBranch        *b_jet_chargedEmEnergy;   //!
   TBranch        *b_jet_chargedMuEnergy;   //!
   TBranch        *b_jet_neutralEmEnergy;   //!
   TBranch        *b_electron_;   //!
   TBranch        *b_electron_pt;   //!
   TBranch        *b_electron_eta;   //!
   TBranch        *b_electron_phi;   //!
   TBranch        *b_muon_;   //!
   TBranch        *b_muon_pt;   //!
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_phi;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metPhi;   //!
   TBranch        *b_type1met;   //!
   TBranch        *b_type1metPhi;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_nVertex;   //!
   TBranch        *b_pu_weight;   //!

   WjetsSelector(TTree * /*tree*/ =0) { }
   virtual ~WjetsSelector() { }
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

   ClassDef(WjetsSelector,0);
};

#endif

#ifdef WjetsSelector_cxx
void WjetsSelector::Init(TTree *tree)
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

   fChain->SetBranchAddress("photon", &photon_, &b_photon_);
   fChain->SetBranchAddress("photon.pt", photon_pt, &b_photon_pt);
   fChain->SetBranchAddress("photon.eta", photon_eta, &b_photon_eta);
   fChain->SetBranchAddress("photon.phi", photon_phi, &b_photon_phi);
   fChain->SetBranchAddress("photon.r9", photon_r9, &b_photon_r9);
   fChain->SetBranchAddress("photon.sigmaIetaIeta", photon_sigmaIetaIeta, &b_photon_sigmaIetaIeta);
   fChain->SetBranchAddress("photon.hadTowOverEm", photon_hadTowOverEm, &b_photon_hadTowOverEm);
   fChain->SetBranchAddress("photon.pixelseed", photon_pixelseed, &b_photon_pixelseed);
   fChain->SetBranchAddress("photon.chargedIso", photon_chargedIso, &b_photon_chargedIso);
   fChain->SetBranchAddress("photon.neutralIso", photon_neutralIso, &b_photon_neutralIso);
   fChain->SetBranchAddress("photon.photonIso", photon_photonIso, &b_photon_photonIso);
   fChain->SetBranchAddress("photon.conversionSafeVeto", photon_conversionSafeVeto, &b_photon_conversionSafeVeto);
   fChain->SetBranchAddress("jet", &jet_, &b_jet_);
   fChain->SetBranchAddress("jet.pt", jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet.eta", jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet.phi", jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet.bCSV", jet_bCSV, &b_jet_bCSV);
   fChain->SetBranchAddress("jet.chargedHadronEnergy", jet_chargedHadronEnergy, &b_jet_chargedHadronEnergy);
   fChain->SetBranchAddress("jet.neutralHadronEnergy", jet_neutralHadronEnergy, &b_jet_neutralHadronEnergy);
   fChain->SetBranchAddress("jet.photonEnergy", jet_photonEnergy, &b_jet_photonEnergy);
   fChain->SetBranchAddress("jet.electronEnergy", jet_electronEnergy, &b_jet_electronEnergy);
   fChain->SetBranchAddress("jet.muonEnergy", jet_muonEnergy, &b_jet_muonEnergy);
   fChain->SetBranchAddress("jet.HFHadronEnergy", jet_HFHadronEnergy, &b_jet_HFHadronEnergy);
   fChain->SetBranchAddress("jet.HFEMEnergy", jet_HFEMEnergy, &b_jet_HFEMEnergy);
   fChain->SetBranchAddress("jet.chargedEmEnergy", jet_chargedEmEnergy, &b_jet_chargedEmEnergy);
   fChain->SetBranchAddress("jet.chargedMuEnergy", jet_chargedMuEnergy, &b_jet_chargedMuEnergy);
   fChain->SetBranchAddress("jet.neutralEmEnergy", jet_neutralEmEnergy, &b_jet_neutralEmEnergy);
   fChain->SetBranchAddress("electron", &electron_, &b_electron_);
   fChain->SetBranchAddress("electron.pt", electron_pt, &b_electron_pt);
   fChain->SetBranchAddress("electron.eta", electron_eta, &b_electron_eta);
   fChain->SetBranchAddress("electron.phi", electron_phi, &b_electron_phi);
   fChain->SetBranchAddress("muon", &muon_, &b_muon_);
   fChain->SetBranchAddress("muon.pt", muon_pt, &b_muon_pt);
   fChain->SetBranchAddress("muon.eta", muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon.phi", muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metPhi", &metPhi, &b_metPhi);
   fChain->SetBranchAddress("type1met", &type1met, &b_type1met);
   fChain->SetBranchAddress("type1metPhi", &type1metPhi, &b_type1metPhi);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("nVertex", &nVertex, &b_nVertex);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
}

Bool_t WjetsSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef WjetsSelector_cxx
