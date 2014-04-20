//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr 17 16:17:58 2014 by ROOT version 5.34/18
// from TChain photonTree/
//////////////////////////////////////////////////////////

#ifndef Data_Photon_h
#define Data_Photon_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class Data_Photon : public TSelector {
public :
int ThePhoton;
float ThePhotonPt;
float ThePhotonPhi;
float ThePhotonEta;

const static Int_t kMaxphotons = 10;
const static Int_t kMaxjets = 20;
const static Int_t kMaxelectrons = 5;
const static Int_t kMaxmuons = 5;
const static Int_t kMaxgenPhotons = 5;
const static Int_t kMaxgenElectrons = 5;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           photons_;
   Float_t         photons_pt[kMaxphotons];   //[photons_]
   Float_t         photons_eta[kMaxphotons];   //[photons_]
   Float_t         photons_phi[kMaxphotons];   //[photons_]
   Float_t         photons__ptJet[kMaxphotons];   //[photons_]
   Float_t         photons_sigmaIphiIphi[kMaxphotons];   //[photons_]
   Float_t         photons_r9[kMaxphotons];   //[photons_]
   Float_t         photons_sigmaIetaIeta[kMaxphotons];   //[photons_]
   Float_t         photons_hadTowOverEm[kMaxphotons];   //[photons_]
   Float_t         photons_chargedIso[kMaxphotons];   //[photons_]
   Float_t         photons_neutralIso[kMaxphotons];   //[photons_]
   Float_t         photons_photonIso[kMaxphotons];   //[photons_]
   Bool_t          photons_conversionSafeVeto[kMaxphotons];   //[photons_]
   Int_t           photons_pixelseed[kMaxphotons];   //[photons_]
   Short_t         photons_genInformation[kMaxphotons];   //[photons_]
   Short_t         photons_matchedJetIndex[kMaxphotons];   //[photons_]
   Int_t           jets_;
   Float_t         jets_pt[kMaxjets];   //[jets_]
   Float_t         jets_eta[kMaxjets];   //[jets_]
   Float_t         jets_phi[kMaxjets];   //[jets_]
   Short_t         jets_matchInformation[kMaxjets];   //[jets_]
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
   Int_t           muons_;
   Float_t         muons_pt[kMaxmuons];   //[muons_]
   Float_t         muons_eta[kMaxmuons];   //[muons_]
   Float_t         muons_phi[kMaxmuons];   //[muons_]
   Int_t           genPhotons_;
   Float_t         genPhotons_pt[kMaxgenPhotons];   //[genPhotons_]
   Float_t         genPhotons_eta[kMaxgenPhotons];   //[genPhotons_]
   Float_t         genPhotons_phi[kMaxgenPhotons];   //[genPhotons_]
   Int_t           genElectrons_;
   Float_t         genElectrons_pt[kMaxgenElectrons];   //[genElectrons_]
   Float_t         genElectrons_eta[kMaxgenElectrons];   //[genElectrons_]
   Float_t         genElectrons_phi[kMaxgenElectrons];   //[genElectrons_]
   Float_t         met;
   Float_t         metSig;
   Float_t         metPhi;
   Float_t         metShiftxy;
   Float_t         metShiftxyPhi;
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
   TBranch        *b_photons__ptJet;   //!
   TBranch        *b_photons_sigmaIphiIphi;   //!
   TBranch        *b_photons_r9;   //!
   TBranch        *b_photons_sigmaIetaIeta;   //!
   TBranch        *b_photons_hadTowOverEm;   //!
   TBranch        *b_photons_chargedIso;   //!
   TBranch        *b_photons_neutralIso;   //!
   TBranch        *b_photons_photonIso;   //!
   TBranch        *b_photons_conversionSafeVeto;   //!
   TBranch        *b_photons_pixelseed;   //!
   TBranch        *b_photons_genInformation;   //!
   TBranch        *b_photons_matchedJetIndex;   //!
   TBranch        *b_jets_;   //!
   TBranch        *b_jets_pt;   //!
   TBranch        *b_jets_eta;   //!
   TBranch        *b_jets_phi;   //!
   TBranch        *b_jets_matchInformation;   //!
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
   TBranch        *b_muons_;   //!
   TBranch        *b_muons_pt;   //!
   TBranch        *b_muons_eta;   //!
   TBranch        *b_muons_phi;   //!
   TBranch        *b_genPhotons_;   //!
   TBranch        *b_genPhotons_pt;   //!
   TBranch        *b_genPhotons_eta;   //!
   TBranch        *b_genPhotons_phi;   //!
   TBranch        *b_genElectrons_;   //!
   TBranch        *b_genElectrons_pt;   //!
   TBranch        *b_genElectrons_eta;   //!
   TBranch        *b_genElectrons_phi;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metSig;   //!
   TBranch        *b_metPhi;   //!
   TBranch        *b_metShiftxy;   //!
   TBranch        *b_metShiftxyPhi;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_nVertex;   //!
   TBranch        *b_nGoodJets;   //!
   TBranch        *b_nTracksPV;   //!
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_luminosityBlockNumber;   //!

   Data_Photon(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~Data_Photon() { }
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

   //ClassDef(Data_Photon,0);
};

#endif

#ifdef Data_Photon_cxx
void Data_Photon::Init(TTree *tree)
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
   fChain->SetBranchAddress("photons._ptJet", photons__ptJet, &b_photons__ptJet);
   fChain->SetBranchAddress("photons.sigmaIphiIphi", photons_sigmaIphiIphi, &b_photons_sigmaIphiIphi);
   fChain->SetBranchAddress("photons.r9", photons_r9, &b_photons_r9);
   fChain->SetBranchAddress("photons.sigmaIetaIeta", photons_sigmaIetaIeta, &b_photons_sigmaIetaIeta);
   fChain->SetBranchAddress("photons.hadTowOverEm", photons_hadTowOverEm, &b_photons_hadTowOverEm);
   fChain->SetBranchAddress("photons.chargedIso", photons_chargedIso, &b_photons_chargedIso);
   fChain->SetBranchAddress("photons.neutralIso", photons_neutralIso, &b_photons_neutralIso);
   fChain->SetBranchAddress("photons.photonIso", photons_photonIso, &b_photons_photonIso);
   fChain->SetBranchAddress("photons.conversionSafeVeto", photons_conversionSafeVeto, &b_photons_conversionSafeVeto);
   fChain->SetBranchAddress("photons.pixelseed", photons_pixelseed, &b_photons_pixelseed);
   fChain->SetBranchAddress("photons.genInformation", photons_genInformation, &b_photons_genInformation);
   fChain->SetBranchAddress("photons.matchedJetIndex", photons_matchedJetIndex, &b_photons_matchedJetIndex);
   fChain->SetBranchAddress("jets", &jets_, &b_jets_);
   fChain->SetBranchAddress("jets.pt", jets_pt, &b_jets_pt);
   fChain->SetBranchAddress("jets.eta", jets_eta, &b_jets_eta);
   fChain->SetBranchAddress("jets.phi", jets_phi, &b_jets_phi);
   fChain->SetBranchAddress("jets.matchInformation", jets_matchInformation, &b_jets_matchInformation);
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
   fChain->SetBranchAddress("muons", &muons_, &b_muons_);
   fChain->SetBranchAddress("muons.pt", muons_pt, &b_muons_pt);
   fChain->SetBranchAddress("muons.eta", muons_eta, &b_muons_eta);
   fChain->SetBranchAddress("muons.phi", muons_phi, &b_muons_phi);
   fChain->SetBranchAddress("genPhotons", &genPhotons_, &b_genPhotons_);
   fChain->SetBranchAddress("genPhotons.pt", &genPhotons_pt, &b_genPhotons_pt);
   fChain->SetBranchAddress("genPhotons.eta", &genPhotons_eta, &b_genPhotons_eta);
   fChain->SetBranchAddress("genPhotons.phi", &genPhotons_phi, &b_genPhotons_phi);
   fChain->SetBranchAddress("genElectrons", &genElectrons_, &b_genElectrons_);
   fChain->SetBranchAddress("genElectrons.pt", &genElectrons_pt, &b_genElectrons_pt);
   fChain->SetBranchAddress("genElectrons.eta", &genElectrons_eta, &b_genElectrons_eta);
   fChain->SetBranchAddress("genElectrons.phi", &genElectrons_phi, &b_genElectrons_phi);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metSig", &metSig, &b_metSig);
   fChain->SetBranchAddress("metPhi", &metPhi, &b_metPhi);
   fChain->SetBranchAddress("metShiftxy", &metShiftxy, &b_metShiftxy);
   fChain->SetBranchAddress("metShiftxyPhi", &metShiftxyPhi, &b_metShiftxyPhi);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("nVertex", &nVertex, &b_nVertex);
   fChain->SetBranchAddress("nGoodJets", &nGoodJets, &b_nGoodJets);
   fChain->SetBranchAddress("nTracksPV", &nTracksPV, &b_nTracksPV);
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("luminosityBlockNumber", &luminosityBlockNumber, &b_luminosityBlockNumber);
}

Bool_t Data_Photon::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef Data_Photon_cxx
