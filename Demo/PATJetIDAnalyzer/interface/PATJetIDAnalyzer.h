// -*- C++ -*-
//
// Package:    PATJetIDAnalyzer
// Class:      PATJetIDAnalyzer
// 
/**\class PATJetIDAnalyzer PATJetIDAnalyzer.h Demo/PATJetIDAnalyzer/interface/PATJetIDAnalyzer.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Mon Feb 25 11:33:02 CET 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declerations
//
class PtGreater {
public:
  template <typename T> bool operator () (const T& i, const T& j) {
    return (i.pt() > j.pt());
  }
};

class PATJetIDAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PATJetIDAnalyzer(const edm::ParameterSet&);
      ~PATJetIDAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      virtual void makeMatchingMaps(edm::Handle<reco::GenJetCollection> GenJets,
                                    edm::Handle<reco::CaloJetCollection> CaloJets);

      // ----------member data ---------------------------

  //jets from PAT
  edm::InputTag _patJet;
  edm::InputTag _patMet;
 
  //jets from AOD
  edm::InputTag _recJet;
  edm::InputTag _genJet;
  edm::InputTag _recMet;
  edm::InputTag _genMet;
 
 
  std::string  _hist;
  double _jetminpt, _jetmaxeta;
  
  //CaloJets
  unsigned static const _njets = 4;
  TH1F *_jetmult;               //total number of jets
  TH1F *_pt_jet[_njets];    	//pt of a specific jet
  TH1F *_eta_jet[_njets];	//eta
  TH1F *_phi_jet[_njets];	//phi
  TH1F *_emfrac_jet[_njets];	//electromagnetic energy fraction
  TH1F *_hadfrac_jet[_njets];	//hadronic energy fraction
  TH1F *_n60_jet[_njets];	//number of components containing 60% of the energy
  TH1F *_n90_jet[_njets];	//number of components containing 90% of the energy
  TH1F *_area_jet[_njets];	//area covered by the jet's towers
  //GenJets
  unsigned static const _ngenjets = 4;
  TH1F *_genjetmult;               	//total number of genjets
  TH1F *_pt_genjet[_ngenjets];    	//pt of a specific genjet
  TH1F *_eta_genjet[_ngenjets];		//eta
  TH1F *_phi_genjet[_ngenjets];		//phi
  TH1F *_emfrac_genjet[_ngenjets];	//electromagnetic energy fraction
  TH1F *_hadfrac_genjet[_ngenjets];	//hadronic energy fraction
  TH1F *_invisible_genjet[_ngenjets];	//electromagnetic energy fraction
  TH1F *_aux_genjet[_ngenjets];		//hadronic energy fraction
  //met
  TH1F *_met;			//missing transverse energy
  TH1F *_metmult;		//size of the met std::vector
  TH1F *_metx;			//missing transverse energy, x-component
  TH1F *_mety;			//missing transverse energy, y-component
  TH1F *_metsig;		//MET significance = sqrt(met)/sum et
  //genmet
  TH1F *_genmet;		//missing transverse energy
  TH1F *_genmetmult;		//size of the met std::vector
  TH1F *_genmetx;		//missing transverse energy, x-component
  TH1F *_genmety;		//missing transverse energy, y-component
  //other
  TH1F *_ht;			//sum of all hadronic energy in the event
  TH1F *_htmet;			//sum of all energy, incl. MET in the event
  TH1F *_dijet;			//invariant mass of two leading jets
  TH1F *_nv;			//the "new variable"
  TH1F *_nv2;			//the "new variable 2"
  TH1F *_genht;			//sum of all hadronic energy in the event (gen)
  TH1F *_genhtmet;		//sum of all energy, incl. MET in the event (gen)
  TH1F *_gendijet;			//invariant mass of two leading jets (gen)
  //matching
  TH2F *_GenOnCalo_match;       //Matching of GenJets on CaloJets
  TH2F *_CaloOnGen_match;       //Matching of Calojets on GenJets
  TH2F *_GenVsMatched_pt;       //Generated vs. Matched quantities
  TH2F *_GenVsMatched_eta;
  TH2F *_GenVsMatched_phi;
  TH1F *_RecoEff_pt;            //Reconstruction efficiencies
  TH1F *_RecoEff_eta;
  TH1F *_RecoEff_phi;
  // helper histograms
  TH1F *_RecoTot_pt;
  TH1F *_RecoTot_eta;
  TH1F *_RecoTot_phi;

  // Matching Map: GenJets -> CaloJets
  std::map<const reco::GenJet*, const reco::CaloJet*> MatchingMapGen;
  // Matching Map: CaloJets -> GenJets
  std::map<const reco::CaloJet*, const reco::GenJet*> MatchingMapJet;



};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

