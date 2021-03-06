//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jul 22 11:09:36 2014 by ROOT version 5.34/18
// from TChain photonJetTree/
//////////////////////////////////////////////////////////

#ifndef Data_Jet_h
#define Data_Jet_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
class Data_Jet : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
const static Int_t kMaxphotons = 20;
const static Int_t kMaxjets = 90;
const static Int_t kMaxelectrons = 10;
const static Int_t kMaxmuons = 10;
const static Int_t kMaxgenPhotons = 10;
const static Int_t kMaxgenElectrons = 10;
int ThePhoton;
float ThePhotonPt;
float ThePhotonPhi;
float ThePhotonEta;

   // Declaration of leaf types
   Int_t           photons_;
   Float_t         photons_pt[kMaxphotons];   //[photons_]
   Float_t         photons_eta[kMaxphotons];   //[photons_]
   Float_t         photons_phi[kMaxphotons];   //[photons_]
   Short_t         photons_bitFlag[kMaxphotons];   //[photons_]
   Float_t         photons__ptJet[kMaxphotons];   //[photons_]
   Float_t         photons__etaJet[kMaxphotons];   //[photons_]
   Float_t         photons__phiJet[kMaxphotons];   //[photons_]
   Float_t         photons_sigmaIphiIphi[kMaxphotons];   //[photons_]
   Float_t         photons_r9[kMaxphotons];   //[photons_]
   Float_t         photons_sigmaIetaIeta[kMaxphotons];   //[photons_]
   Float_t         photons_hadTowOverEm[kMaxphotons];   //[photons_]
   Float_t         photons_chargedIso[kMaxphotons];   //[photons_]
   Float_t         photons_neutralIso[kMaxphotons];   //[photons_]
   Float_t         photons_photonIso[kMaxphotons];   //[photons_]
   Bool_t          photons_conversionSafeVeto[kMaxphotons];   //[photons_]
   Int_t           photons_pixelseed[kMaxphotons];   //[photons_]
   Short_t         photons_matchedJetIndex[kMaxphotons];   //[photons_]
   Int_t           jets_;
   Float_t         jets_pt[kMaxjets];   //[jets_]
   Float_t         jets_eta[kMaxjets];   //[jets_]
   Float_t         jets_phi[kMaxjets];   //[jets_]
   Short_t         jets_bitFlag[kMaxjets];   //[jets_]
   Float_t         jets_bCSV[kMaxjets];   //[jets_]
   Float_t         jets_chargedHadronEnergy[kMaxjets];   //[jets_]
   Float_t         jets_neutralHadronEnergy[kMaxjets];   //[jets_]
   Float_t         jets_photonEnergy[kMaxjets];   //[jets_]
   Float_t         jets_electronEnergy[kMaxjets];   //[jets_]
   Float_t         jets_muonEnergy[kMaxjets];   //[jets_]
   Float_t         jets_HFHadronEnergy[kMaxjets];   //[jets_]
   Float_t         jets_HFEMEnergy[kMaxjets];   //[jets_]
   Float_t         jets_chargedEmEnergy[kMaxjets];   //[jets_]
   Float_t         jets_chargedMuEnergy[kMaxjets];   //[jets_]
   Float_t         jets_neutralEmEnergy[kMaxjets];   //[jets_]
   Int_t           electrons_;
   Float_t         electrons_pt[kMaxelectrons];   //[electrons_]
   Float_t         electrons_eta[kMaxelectrons];   //[electrons_]
   Float_t         electrons_phi[kMaxelectrons];   //[electrons_]
   Short_t         electrons_bitFlag[kMaxelectrons];   //[electrons_]
   Int_t           muons_;
   Float_t         muons_pt[kMaxmuons];   //[muons_]
   Float_t         muons_eta[kMaxmuons];   //[muons_]
   Float_t         muons_phi[kMaxmuons];   //[muons_]
   Short_t         muons_bitFlag[kMaxmuons];   //[muons_]
   Int_t           genPhotons_;
   Float_t         genPhotons_pt[kMaxgenPhotons];   //[genPhotons_]
   Float_t         genPhotons_eta[kMaxgenPhotons];   //[genPhotons_]
   Float_t         genPhotons_phi[kMaxgenPhotons];   //[genPhotons_]
   Short_t         genPhotons_bitFlag[kMaxgenPhotons];   //[genPhotons_]
   Int_t           genElectrons_;
   Float_t         genElectrons_pt[kMaxgenElectrons];   //[genElectrons_]
   Float_t         genElectrons_eta[kMaxgenElectrons];   //[genElectrons_]
   Float_t         genElectrons_phi[kMaxgenElectrons];   //[genElectrons_]
   Short_t         genElectrons_bitFlag[kMaxgenElectrons];   //[genElectrons_]
   Float_t         metSig;
   Float_t         met;
   Float_t         metPhi;
   Float_t         metShiftxy;
   Float_t         metShiftxyPhi;
   Float_t         met01corr;
   Float_t         met01corrPhi;
   Float_t         recoil;
   Float_t         recoilPhi;
   Float_t         mht;
   Float_t         mhtPhi;
   Float_t         ht;
   Float_t         weight;
   Int_t           nVertex;
   UInt_t          nGoodJets;
   UInt_t          nTracksPV;
   UInt_t          runNumber;
   UInt_t          eventNumber;
   UInt_t          luminosityBlockNumber;

   // List of branches
   TBranch        *b_photons_;   //!
   TBranch        *b_photons_pt;   //!
   TBranch        *b_photons_eta;   //!
   TBranch        *b_photons_phi;   //!
   TBranch        *b_photons_bitFlag;   //!
   TBranch        *b_photons__ptJet;   //!
   TBranch        *b_photons__etaJet;   //!
   TBranch        *b_photons__phiJet;   //!
   TBranch        *b_photons_sigmaIphiIphi;   //!
   TBranch        *b_photons_r9;   //!
   TBranch        *b_photons_sigmaIetaIeta;   //!
   TBranch        *b_photons_hadTowOverEm;   //!
   TBranch        *b_photons_chargedIso;   //!
   TBranch        *b_photons_neutralIso;   //!
   TBranch        *b_photons_photonIso;   //!
   TBranch        *b_photons_conversionSafeVeto;   //!
   TBranch        *b_photons_pixelseed;   //!
   TBranch        *b_photons_matchedJetIndex;   //!
   TBranch        *b_jets_;   //!
   TBranch        *b_jets_pt;   //!
   TBranch        *b_jets_eta;   //!
   TBranch        *b_jets_phi;   //!
   TBranch        *b_jets_bitFlag;   //!
   TBranch        *b_jets_bCSV;   //!
   TBranch        *b_jets_chargedHadronEnergy;   //!
   TBranch        *b_jets_neutralHadronEnergy;   //!
   TBranch        *b_jets_photonEnergy;   //!
   TBranch        *b_jets_electronEnergy;   //!
   TBranch        *b_jets_muonEnergy;   //!
   TBranch        *b_jets_HFHadronEnergy;   //!
   TBranch        *b_jets_HFEMEnergy;   //!
   TBranch        *b_jets_chargedEmEnergy;   //!
   TBranch        *b_jets_chargedMuEnergy;   //!
   TBranch        *b_jets_neutralEmEnergy;   //!
   TBranch        *b_electrons_;   //!
   TBranch        *b_electrons_pt;   //!
   TBranch        *b_electrons_eta;   //!
   TBranch        *b_electrons_phi;   //!
   TBranch        *b_electrons_bitFlag;   //!
   TBranch        *b_muons_;   //!
   TBranch        *b_muons_pt;   //!
   TBranch        *b_muons_eta;   //!
   TBranch        *b_muons_phi;   //!
   TBranch        *b_muons_bitFlag;   //!
   TBranch        *b_genPhotons_;   //!
   TBranch        *b_genPhotons_pt;   //!
   TBranch        *b_genPhotons_eta;   //!
   TBranch        *b_genPhotons_phi;   //!
   TBranch        *b_genPhotons_bitFlag;   //!
   TBranch        *b_genElectrons_;   //!
   TBranch        *b_genElectrons_pt;   //!
   TBranch        *b_genElectrons_eta;   //!
   TBranch        *b_genElectrons_phi;   //!
   TBranch        *b_genElectrons_bitFlag;   //!
   TBranch        *b_metSig;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metPhi;   //!
   TBranch        *b_metShiftxy;   //!
   TBranch        *b_metShiftxyPhi;   //!
   TBranch        *b_met01corr;   //!
   TBranch        *b_met01corrPhi;   //!
   TBranch        *b_recoil;   //!
   TBranch        *b_recoilPhi;   //!
   TBranch        *b_mht;   //!
   TBranch        *b_mhtPhi;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_nVertex;   //!
   TBranch        *b_nGoodJets;   //!
   TBranch        *b_nTracksPV;   //!
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_luminosityBlockNumber;   //!

   Data_Jet(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~Data_Jet() { }
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

   //ClassDef(Data_Jet,0);
};

#endif

#ifdef Data_Jet_cxx
void Data_Jet::Init(TTree *tree)
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

   fChain->SetBranchAddress("photons", &photons_, &b_photons_);
   fChain->SetBranchAddress("photons.pt", photons_pt, &b_photons_pt);
   fChain->SetBranchAddress("photons.eta", photons_eta, &b_photons_eta);
   fChain->SetBranchAddress("photons.phi", photons_phi, &b_photons_phi);
   fChain->SetBranchAddress("photons.bitFlag", photons_bitFlag, &b_photons_bitFlag);
   fChain->SetBranchAddress("photons._ptJet", photons__ptJet, &b_photons__ptJet);
   fChain->SetBranchAddress("photons._etaJet", photons__etaJet, &b_photons__etaJet);
   fChain->SetBranchAddress("photons._phiJet", photons__phiJet, &b_photons__phiJet);
   fChain->SetBranchAddress("photons.sigmaIphiIphi", photons_sigmaIphiIphi, &b_photons_sigmaIphiIphi);
   fChain->SetBranchAddress("photons.r9", photons_r9, &b_photons_r9);
   fChain->SetBranchAddress("photons.sigmaIetaIeta", photons_sigmaIetaIeta, &b_photons_sigmaIetaIeta);
   fChain->SetBranchAddress("photons.hadTowOverEm", photons_hadTowOverEm, &b_photons_hadTowOverEm);
   fChain->SetBranchAddress("photons.chargedIso", photons_chargedIso, &b_photons_chargedIso);
   fChain->SetBranchAddress("photons.neutralIso", photons_neutralIso, &b_photons_neutralIso);
   fChain->SetBranchAddress("photons.photonIso", photons_photonIso, &b_photons_photonIso);
   fChain->SetBranchAddress("photons.conversionSafeVeto", photons_conversionSafeVeto, &b_photons_conversionSafeVeto);
   fChain->SetBranchAddress("photons.pixelseed", photons_pixelseed, &b_photons_pixelseed);
   fChain->SetBranchAddress("photons.matchedJetIndex", photons_matchedJetIndex, &b_photons_matchedJetIndex);
   fChain->SetBranchAddress("jets", &jets_, &b_jets_);
   fChain->SetBranchAddress("jets.pt", jets_pt, &b_jets_pt);
   fChain->SetBranchAddress("jets.eta", jets_eta, &b_jets_eta);
   fChain->SetBranchAddress("jets.phi", jets_phi, &b_jets_phi);
   fChain->SetBranchAddress("jets.bitFlag", jets_bitFlag, &b_jets_bitFlag);
   fChain->SetBranchAddress("jets.bCSV", jets_bCSV, &b_jets_bCSV);
   fChain->SetBranchAddress("jets.chargedHadronEnergy", jets_chargedHadronEnergy, &b_jets_chargedHadronEnergy);
   fChain->SetBranchAddress("jets.neutralHadronEnergy", jets_neutralHadronEnergy, &b_jets_neutralHadronEnergy);
   fChain->SetBranchAddress("jets.photonEnergy", jets_photonEnergy, &b_jets_photonEnergy);
   fChain->SetBranchAddress("jets.electronEnergy", jets_electronEnergy, &b_jets_electronEnergy);
   fChain->SetBranchAddress("jets.muonEnergy", jets_muonEnergy, &b_jets_muonEnergy);
   fChain->SetBranchAddress("jets.HFHadronEnergy", jets_HFHadronEnergy, &b_jets_HFHadronEnergy);
   fChain->SetBranchAddress("jets.HFEMEnergy", jets_HFEMEnergy, &b_jets_HFEMEnergy);
   fChain->SetBranchAddress("jets.chargedEmEnergy", jets_chargedEmEnergy, &b_jets_chargedEmEnergy);
   fChain->SetBranchAddress("jets.chargedMuEnergy", jets_chargedMuEnergy, &b_jets_chargedMuEnergy);
   fChain->SetBranchAddress("jets.neutralEmEnergy", jets_neutralEmEnergy, &b_jets_neutralEmEnergy);
   fChain->SetBranchAddress("electrons", &electrons_, &b_electrons_);
   fChain->SetBranchAddress("electrons.pt", electrons_pt, &b_electrons_pt);
   fChain->SetBranchAddress("electrons.eta", electrons_eta, &b_electrons_eta);
   fChain->SetBranchAddress("electrons.phi", electrons_phi, &b_electrons_phi);
   fChain->SetBranchAddress("electrons.bitFlag", electrons_bitFlag, &b_electrons_bitFlag);
   fChain->SetBranchAddress("muons", &muons_, &b_muons_);
   fChain->SetBranchAddress("muons.pt", muons_pt, &b_muons_pt);
   fChain->SetBranchAddress("muons.eta", muons_eta, &b_muons_eta);
   fChain->SetBranchAddress("muons.phi", muons_phi, &b_muons_phi);
   fChain->SetBranchAddress("muons.bitFlag", muons_bitFlag, &b_muons_bitFlag);
   fChain->SetBranchAddress("genPhotons", &genPhotons_, &b_genPhotons_);
   fChain->SetBranchAddress("genPhotons.pt", &genPhotons_pt, &b_genPhotons_pt);
   fChain->SetBranchAddress("genPhotons.eta", &genPhotons_eta, &b_genPhotons_eta);
   fChain->SetBranchAddress("genPhotons.phi", &genPhotons_phi, &b_genPhotons_phi);
   fChain->SetBranchAddress("genPhotons.bitFlag", &genPhotons_bitFlag, &b_genPhotons_bitFlag);
   fChain->SetBranchAddress("genElectrons", &genElectrons_, &b_genElectrons_);
   fChain->SetBranchAddress("genElectrons.pt", &genElectrons_pt, &b_genElectrons_pt);
   fChain->SetBranchAddress("genElectrons.eta", &genElectrons_eta, &b_genElectrons_eta);
   fChain->SetBranchAddress("genElectrons.phi", &genElectrons_phi, &b_genElectrons_phi);
   fChain->SetBranchAddress("genElectrons.bitFlag", &genElectrons_bitFlag, &b_genElectrons_bitFlag);
   fChain->SetBranchAddress("metSig", &metSig, &b_metSig);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metPhi", &metPhi, &b_metPhi);
   fChain->SetBranchAddress("metShiftxy", &metShiftxy, &b_metShiftxy);
   fChain->SetBranchAddress("metShiftxyPhi", &metShiftxyPhi, &b_metShiftxyPhi);
   fChain->SetBranchAddress("met01corr", &met01corr, &b_met01corr);
   fChain->SetBranchAddress("met01corrPhi", &met01corrPhi, &b_met01corrPhi);
   fChain->SetBranchAddress("recoil", &recoil, &b_recoil);
   fChain->SetBranchAddress("recoilPhi", &recoilPhi, &b_recoilPhi);
   fChain->SetBranchAddress("mht", &mht, &b_mht);
   fChain->SetBranchAddress("mhtPhi", &mhtPhi, &b_mhtPhi);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("nVertex", &nVertex, &b_nVertex);
   fChain->SetBranchAddress("nGoodJets", &nGoodJets, &b_nGoodJets);
   fChain->SetBranchAddress("nTracksPV", &nTracksPV, &b_nTracksPV);
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("luminosityBlockNumber", &luminosityBlockNumber, &b_luminosityBlockNumber);
}

Bool_t Data_Jet::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef Data_Jet_cxx
