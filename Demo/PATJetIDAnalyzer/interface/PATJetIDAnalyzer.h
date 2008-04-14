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
// $Id: PATJetIDAnalyzer.h,v 1.2 2008/02/25 17:52:29 auterman Exp $
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

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

//
// class declerations
//
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
      void FourierTransformation( const unsigned int i, const pat::Jet& jet, const EBRecHitCollection& EcalRecHitEB );
      double norm(int n);     
      TH2F * FDCT(TH2F & k);
      void FakeNoise(TH2F & k, double * k);
      void FakeNoise2x2(TH2F & k, double * k);
      double dphi(double phi1, double phi2);
      // ----------member data ---------------------------

  const CaloSubdetectorGeometry* EBgeom;
  TRandom *random;

  //jets from PAT
  edm::InputTag _patJet;
  edm::InputTag _patMet;
  //EM-cells
  edm::InputTag _ebrechits;
 
  //jets from AOD
  edm::InputTag _recJet;
  edm::InputTag _genJet;
  edm::InputTag _recMet;
  edm::InputTag _genMet;
 
 
  std::string  _hist;
  double _jetminpt,  _jetmaxeta;
  bool _simulate_noise;
  double _NoiseMean;
  double _NoiseSigma;
  double _NoiseThreshold;
  bool   _DoNormalization;
  int _uniqueplotid;

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

  //Fourier Transformation
  unsigned static const N_Fourier_Bins_2D = 200;
  unsigned static const N_Fourier_Bins_1D = 20;
  unsigned static const Nf_Fourier_Bins_1D = N_Fourier_Bins_1D/2;
  TH2F *_ft_energy[_njets]; 
  TH2F *_ft_frequency[_njets]; 
  TH1F *_ft_k[_njets]; 
  TH1F *_ft_f[_njets]; 
  TH1F *_ft_ksubavg[_njets]; 
  
  TH1F *_noisecontrib[_njets];  //amount of added noise
  TH1I *_fto_n99[_njets];       //number of bins containing 90%
  TH1I *_fto_n95[_njets];       //number of bins containing 90%
  TH1I *_fto_n90[_njets];       //number of bins containing 90%
  TH1I *_fto_n60[_njets];       //number of bins containing 60%
  TH1I *_fto_n30[_njets];       //number of bins containing 30%
  TH1I *_fto_n10[_njets];       //number of bins containing 10%
  TH1F *_fto_F10[_njets];       //Sum of 10 largest bins
  TH1F *_fto_Fs10[_njets];      //Sum of 10 smallest (non-zero) bins
  TH1F *_fto_LowFvsHiF[_njets]; //diff. between low & high frequencies
  TH1F *_fto_LowFovHiF[_njets]; //ratio of low vs. high frequencies
  TH1F *_fto_det16[_njets];       //determinante
  TH1F *_fto_det8[_njets];       //determinante
  

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

