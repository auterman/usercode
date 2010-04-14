// -*- C++ -*-
//
// Package:    FinalPlots
// Class:      FinalPlots
// 
/**\class FinalPlots FinalPlots.cc Analysis/FinalPlots/src/FinalPlots.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann,68/112,2115,
//         Created:  Sun Oct 18 20:00:45 CEST 2009
// $Id: FinalPlots.cc,v 1.1.1.1 2010/03/26 17:01:13 auterman Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <cmath>
#include <numeric>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/eventSetupGetImplementation.h"
#include "PhysicsTools/IsolationAlgos/interface/PropagateToCal.h"

#include "TH1F.h"
#include "TH2F.h"

//#include "CommonTools/Utils/interface/PtComparator.h"
template<typename T>
struct PtrGreaterByPt {
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T & t1, const T & t2 ) const {
    return t1->pt() > t2->pt();
  }
};

//
// class decleration
//

class FinalPlots : public edm::EDAnalyzer {
   public:
      explicit FinalPlots(const edm::ParameterSet&);
      ~FinalPlots();


   private:
      void bookHistograms(const edm::EventSetup&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      double dPhiBiased(const std::vector<const reco::Candidate*>& jets, const reco::Candidate& excljet);
      double summedTowerEnergy(CaloTowerDetId& towerId, CaloTowerCollection towers, int ntw);

      typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzV;
      std::vector<double> deltaSumPt_permutations(const std::vector<LorentzV>& p4s); 
      double alphaT(const std::vector<LorentzV>& p4s); 

      // ----------member data ---------------------------
      edm::InputTag weightName_;
      edm::InputTag Jet_;
      edm::InputTag Met_;
      std::string name_;
      double weight_, jetptmin_, jetetamax_;
      double tmaxEta_, tminPt_, tmaxChiSquare_;
      int    tminNumOfHits_;
      double tminDeltaR_;

      //Jet & MET plots
      TH1F * HT_;
      TH1F * MHT_;
      TH1F * MET_; 
      TH1F * AllJetsPt_;
      TH1F * Jet1Pt_, *Jet2Pt_, *Jet3Pt_, *Jet4Pt_;
      TH1F * Jet1E_,  *Jet2E_,  *Jet3E_,  *Jet4E_;
      TH1F * Jet1Phi_,*Jet2Phi_,*Jet3Phi_,*Jet4Phi_;
      TH1F * Jet1Eta_,*Jet2Eta_,*Jet3Eta_,*Jet4Eta_;
      TH1F * JetMult_; 
      TH1F * dPhiMin_;
      TH1F * dPhiBiased_;

      //Track & Tower plots
      TH1F * AllTracksPt_, *AllTracksPhi_;
      TH1F * Track1Pt_, *Track2Pt_, *Track3Pt_, *Track4Pt_;
      TH1F * Track1Phi_,*Track2Phi_,*Track3Phi_,*Track4Phi_,*Track15Phi_;
      TH1F * Track1Eta_,*Track2Eta_,*Track3Eta_,*Track4Eta_;
      TH1F * Track1NumOfHits_,*Track2NumOfHits_,*Track3NumOfHits_,*Track4NumOfHits_;
      TH1F * Track1ChiSquare_,*Track2ChiSquare_,*Track3ChiSquare_,*Track4ChiSquare_;
      TH2F * Track1NOHvsPhi_,*Track2NOHvsPhi_,*Track3NOHvsPhi_,*Track4NOHvsPhi_;
      TH2F * Track1PtvsPhi_,*Track2PtvsPhi_,*Track3PtvsPhi_,*Track4PtvsPhi_;
      TH1F * TrackMult_;

      TH1F * IsoTrack1Pt_;
      TH1F * IsoTrack1Phi_;
      TH1F * IsoTrack1Eta_;
      TH1F * IsoTrack1NumOfHits_;
      TH1F * IsoTrack1ChiSquare_;
      TH2F * IsoTrack1NOHvsPhi_;
      TH2F * IsoTrack1PtvsPhi_;
      TH1F * IsoTrack1DeltaR_;
      TH1F * IsoTrack1OuterDeltaR_;
      TH1F * TwoTracksDeltaPhi_,*TwoTracksDeltaPt_;

      edm::InputTag tracks_;
      edm::InputTag towers_;
      //PropagateToCal* toEcal_;
      PropagateToCal* toHcal_;
      TH1F * TrackTower_dEta_, *TrackTower_dPhi_, *TrackTower_dPt_;
      TH1F * TrackTower_E1x1_, *TrackTower_E3x3_;
      TH1F * Response_E1x1_, *Response_E3x3_;
      int nTowerGroup_;
      double maxDeltaEta_, maxDeltaPhi_;
  
      bool histogramsBooked;
      int tracksFound, towersFound;
};

FinalPlots::FinalPlots(const edm::ParameterSet& iConfig):
  weightName_(iConfig.getParameter<edm::InputTag>("weightName") ),
  Jet_(       iConfig.getParameter<edm::InputTag>("Jet") ),
  Met_(       iConfig.getParameter<edm::InputTag>("MET") ),
  name_(      iConfig.getParameter<std::string>("uncertainty_name") ),
  jetptmin_ ( iConfig.getParameter<double>("JetPtMin") ),
  jetetamax_( iConfig.getParameter<double>("JetEtaMax") ),
  //tracks & Towers:
  tracks_( iConfig.getParameter<edm::InputTag>( "Tracks" ) ),
  towers_( iConfig.getParameter<edm::InputTag>( "Towers" ) ),
  nTowerGroup_( iConfig.getParameter<int>( "GroupNTowers" ) ),
  maxDeltaEta_ ( iConfig.getParameter<double>("TowerDeltaEtaMax") ),
  maxDeltaPhi_ ( iConfig.getParameter<double>("TowerDeltaPhiMax") ),
  tmaxEta_ ( iConfig.getParameter<double>("TrackEtaMax") ),
  tminPt_( iConfig.getParameter<double>("TrackPtMin") ),
  tmaxChiSquare_( iConfig.getParameter<double>("TrackChiSquareMax") ),
  tminNumOfHits_( iConfig.getParameter<int>( "TrackNumOfHitsMin" ) ),
  tminDeltaR_( iConfig.getParameter<double>("TrackDeltaRMin") )
{
  histogramsBooked = false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
FinalPlots::bookHistograms(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  if( !fs ){
    throw edm::Exception( edm::errors::Configuration,
                          "TFile Service is not registered in cfg file" );
  }

  std::string histname = "HT"+name_;
  HT_    = fs->make<TH1F>(histname.c_str(),";HT [GeV];events",   100, 0.0, 100.0);
  HT_->Sumw2();  

  histname = "MHT"+name_;
  MHT_     = fs->make<TH1F>(histname.c_str(),";MHT [GeV];events",  100, 0.0, 50.0);
  MHT_->Sumw2();  
  histname = "MET"+name_;
  MET_     = fs->make<TH1F>(histname.c_str(),";MET [GeV];events",  100, 0.0, 50.0);
  MET_->Sumw2();  

  histname = "AllJetsPt"+name_;
  AllJetsPt_  = fs->make<TH1F>(histname.c_str(),";All Jets, Pt [GeV];events",  100, 0.0, 50.0);
  AllJetsPt_->Sumw2();  

  histname = "Jet1Pt"+name_;
  Jet1Pt_  = fs->make<TH1F>(histname.c_str(),";1. Jet, Pt [GeV];events",  40, 0.0, 20.0);
  Jet1Pt_->Sumw2();  
  histname = "Jet2Pt"+name_;
  Jet2Pt_  = fs->make<TH1F>(histname.c_str(),";2. Jet, Pt [GeV];events",  40, 0.0, 20.0);
  Jet2Pt_->Sumw2();  
  histname = "Jet3Pt"+name_;
  Jet3Pt_  = fs->make<TH1F>(histname.c_str(),";3. Jet, Pt [GeV];events",  40, 0.0, 20.0);
  Jet3Pt_->Sumw2();  
  histname = "Jet4Pt"+name_;
  Jet4Pt_  = fs->make<TH1F>(histname.c_str(),";4. Jet, Pt [GeV];events",  40, 0.0, 20.0);
  Jet4Pt_->Sumw2();  

  histname = "Jet1E"+name_;
  Jet1E_  = fs->make<TH1F>(histname.c_str(),";1. Jet, E [GeV];events",  40, 0.0, 20.0);
  Jet1E_->Sumw2();  
  histname = "Jet2E"+name_;
  Jet2E_  = fs->make<TH1F>(histname.c_str(),";2. Jet, E [GeV];events",  40, 0.0, 20.0);
  Jet2E_->Sumw2();  
  histname = "Jet3E"+name_;
  Jet3E_  = fs->make<TH1F>(histname.c_str(),";3. Jet, E [GeV];events",  40, 0.0, 20.0);
  Jet3E_->Sumw2();  
  histname = "Jet4E"+name_;
  Jet4E_  = fs->make<TH1F>(histname.c_str(),";4. Jet, E [GeV];events",  40, 0.0, 20.0);
  Jet4E_->Sumw2();  

  histname = "Jet1Phi"+name_;
  Jet1Phi_  = fs->make<TH1F>(histname.c_str(),";1. Jet, #phi;events", 40, -3.2,  3.2);
  Jet1Phi_->Sumw2();  
  histname = "Jet2Phi"+name_;
  Jet2Phi_  = fs->make<TH1F>(histname.c_str(),";2. Jet, #phi;events", 40, -3.2,  3.2);
  Jet2Phi_->Sumw2();  
  histname = "Jet3Phi"+name_;
  Jet3Phi_  = fs->make<TH1F>(histname.c_str(),";3. Jet, #phi;events", 40, -3.2,  3.2);
  Jet3Phi_->Sumw2();  
  histname = "Jet4Phi"+name_;
  Jet4Phi_  = fs->make<TH1F>(histname.c_str(),";4. Jet, #phi;events", 40, -3.2,  3.2);
  Jet4Phi_->Sumw2();  

  histname = "Jet1Eta"+name_;
  Jet1Eta_  = fs->make<TH1F>(histname.c_str(),";1. Jet, #eta;events", 40, -4.0,  4.0);
  Jet1Eta_->Sumw2();  
  histname = "Jet2Eta"+name_;
  Jet2Eta_  = fs->make<TH1F>(histname.c_str(),";2. Jet, #eta;events", 40, -4.0,  4.0);
  Jet2Eta_->Sumw2();  
  histname = "Jet3Eta"+name_;
  Jet3Eta_  = fs->make<TH1F>(histname.c_str(),";3. Jet, #eta;events", 40, -4.0,  4.0);
  Jet3Eta_->Sumw2();  
  histname = "Jet4Eta"+name_;
  Jet4Eta_  = fs->make<TH1F>(histname.c_str(),";4. Jet, #eta;events", 40, -4.0,  4.0);
  Jet4Eta_->Sumw2();  

  histname = "JetMult"+name_;
  JetMult_ = fs->make<TH1F>(histname.c_str(),";Jet multiplicity;events",  21, -0.5, 20.5);
  JetMult_->Sumw2();  
  histname = "dPhiMin"+name_;
  dPhiMin_ = fs->make<TH1F>(histname.c_str(),";#Delta#phi_{min};events",  20, 0.0, 3.2);
  dPhiMin_->Sumw2();  
  histname = "dPhiBiased"+name_;
  dPhiBiased_ = fs->make<TH1F>(histname.c_str(),";#Delta#phi_{biased};events",  20, 0.0, 3.2);
  dPhiBiased_->Sumw2();  


  // Track & Tower   
  histname = "AllTracksPt"+name_;
  AllTracksPt_  = fs->make<TH1F>(histname.c_str(),";All Tracks, Pt [GeV];events",  100, 0.0, 50.0);
  AllTracksPt_->Sumw2();  
  histname = "AllTracksPhi"+name_;
  AllTracksPhi_  = fs->make<TH1F>(histname.c_str(),";All Tracks, #phi;events", 40, -3.2,  3.2);
  AllTracksPhi_->Sumw2(); 

  histname = "Track1Pt"+name_;
  Track1Pt_  = fs->make<TH1F>(histname.c_str(),";1. Track, Pt [GeV];events",  40, 0.0, 20.0);
  Track1Pt_->Sumw2();  
  histname = "Track2Pt"+name_;
  Track2Pt_  = fs->make<TH1F>(histname.c_str(),";2. Track, Pt [GeV];events",  40, 0.0, 20.0);
  Track2Pt_->Sumw2();  
  histname = "Track3Pt"+name_;
  Track3Pt_  = fs->make<TH1F>(histname.c_str(),";3. Track, Pt [GeV];events",  40, 0.0, 20.0);
  Track3Pt_->Sumw2();  
  histname = "Track4Pt"+name_;
  Track4Pt_  = fs->make<TH1F>(histname.c_str(),";4. Track, Pt [GeV];events",  40, 0.0, 20.0);
  Track4Pt_->Sumw2();  
  histname = "IsoTrack1Pt"+name_;
  IsoTrack1Pt_  = fs->make<TH1F>(histname.c_str(),";1. Track, Pt [GeV];events",  40, 0.0, 20.0);
  IsoTrack1Pt_->Sumw2();  

  histname = "Track1Phi"+name_;
  Track1Phi_  = fs->make<TH1F>(histname.c_str(),";1. Track, #phi;events", 40, -3.2,  3.2);
  Track1Phi_->Sumw2();  
  histname = "Track2Phi"+name_;
  Track2Phi_  = fs->make<TH1F>(histname.c_str(),";2. Track, #phi;events", 40, -3.2,  3.2);
  Track2Phi_->Sumw2();  
  histname = "Track3Phi"+name_;
  Track3Phi_  = fs->make<TH1F>(histname.c_str(),";3. Track, #phi;events", 40, -3.2,  3.2);
  Track3Phi_->Sumw2();  
  histname = "Track4Phi"+name_;
  Track4Phi_  = fs->make<TH1F>(histname.c_str(),";4. Track, #phi;events", 40, -3.2,  3.2);
  Track4Phi_->Sumw2();  
  histname = "Track15Phi"+name_;
  Track15Phi_  = fs->make<TH1F>(histname.c_str(),";15. Track, #phi;events", 40, -3.2,  3.2);
  Track15Phi_->Sumw2();  
  histname = "IsoTrack1Phi"+name_;
  IsoTrack1Phi_  = fs->make<TH1F>(histname.c_str(),";1. Track, #phi;events", 40, -3.2,  3.2);
  IsoTrack1Phi_->Sumw2();


  histname = "Track1Eta"+name_;
  Track1Eta_  = fs->make<TH1F>(histname.c_str(),";1. Track, #eta;events", 40, -4.0,  4.0);
  Track1Eta_->Sumw2();  
  histname = "Track2Eta"+name_;
  Track2Eta_  = fs->make<TH1F>(histname.c_str(),";2. Track, #eta;events", 40, -4.0,  4.0);
  Track2Eta_->Sumw2();  
  histname = "Track3Eta"+name_;
  Track3Eta_  = fs->make<TH1F>(histname.c_str(),";3. Track, #eta;events", 40, -4.0,  4.0);
  Track3Eta_->Sumw2();  
  histname = "Track4Eta"+name_;
  Track4Eta_  = fs->make<TH1F>(histname.c_str(),";4. Track, #eta;events", 40, -4.0,  4.0);
  Track4Eta_->Sumw2();  
  histname = "IsoTrack1Eta"+name_;
  IsoTrack1Eta_  = fs->make<TH1F>(histname.c_str(),";1. Track, #eta;events", 40, -4.0,  4.0);
  IsoTrack1Eta_->Sumw2(); 

  histname = "Track1NumOfHits"+name_;
  Track1NumOfHits_  = fs->make<TH1F>(histname.c_str(),";1. Track, Number Of Hits;events", 40, 0.,  40.);
  Track1NumOfHits_->Sumw2();  
  histname = "Track2NumOfHits"+name_;
  Track2NumOfHits_  = fs->make<TH1F>(histname.c_str(),";2. Track, Number Of Hits;events", 40, 0.,  40.);
  Track2NumOfHits_->Sumw2();  
  histname = "Track3NumOfHits"+name_;
  Track3NumOfHits_  = fs->make<TH1F>(histname.c_str(),";3. Track, Number Of Hits;events", 40, 0.,  40.);
  Track3NumOfHits_->Sumw2();  
  histname = "Track4NumOfHits"+name_;
  Track4NumOfHits_  = fs->make<TH1F>(histname.c_str(),";4. Track, Number Of Hits;events", 40, 0.,  40.);
  Track4NumOfHits_->Sumw2(); 
histname = "IsoTrack1NumOfHits"+name_;
  IsoTrack1NumOfHits_  = fs->make<TH1F>(histname.c_str(),";1. Track, Number Of Hits;events", 40, 0.,  40.);
  IsoTrack1NumOfHits_->Sumw2(); 

  histname = "Track1ChiSquare"+name_;
  Track1ChiSquare_  = fs->make<TH1F>(histname.c_str(),";1. Track, #chi^{2};events", 40, 0.,  10.);
  Track1ChiSquare_->Sumw2();  
  histname = "Track2ChiSquare"+name_;
  Track2ChiSquare_  = fs->make<TH1F>(histname.c_str(),";2. Track, #chi^{2};events", 40, -0.,  10.);
  Track2ChiSquare_->Sumw2();  
  histname = "Track3ChiSquare"+name_;
  Track3ChiSquare_  = fs->make<TH1F>(histname.c_str(),";3. Track, #chi^{2};events", 40, 0.,  10.);
  Track3ChiSquare_->Sumw2();  
  histname = "Track4ChiSquare"+name_;
  Track4ChiSquare_  = fs->make<TH1F>(histname.c_str(),";4. Track, #chi^{2};events", 40, 0.,  10.);
  Track4ChiSquare_->Sumw2();  
  histname = "IsoTrack1ChiSquare"+name_;
  IsoTrack1ChiSquare_  = fs->make<TH1F>(histname.c_str(),";1. Track, #chi^{2};events", 40, 0.,  10.);
  IsoTrack1ChiSquare_->Sumw2(); 

  histname = "Track1NOHvsPhi"+name_;
  Track1NOHvsPhi_  = fs->make<TH2F>(histname.c_str(),";1. Track, Number Of Hits;#phi;events", 40, 0.,  40., 40, -4.0, 4.0);
  Track1NOHvsPhi_->Sumw2();  
  histname = "Track2NOHvsPhi"+name_;
  Track2NOHvsPhi_  = fs->make<TH2F>(histname.c_str(),";2. Track, Number Of Hits;#phi;events", 40, 0.,  40., 40, -4.0, 4.0);
  Track2NOHvsPhi_->Sumw2();  
  histname = "Track3NOHvsPhi"+name_;
  Track3NOHvsPhi_  = fs->make<TH2F>(histname.c_str(),";3. Track, Number Of Hits;#phi;events", 40, 0.,  40., 40, -4.0, 4.0);
  Track3NOHvsPhi_->Sumw2();  
  histname = "Track4NOHvsPhi"+name_;
  Track4NOHvsPhi_  = fs->make<TH2F>(histname.c_str(),";4. Track, Number Of Hits;#phi;events", 40, 0.,  40., 40, -4.0, 4.0);
  Track4NOHvsPhi_->Sumw2(); 
  histname = "IsoTrack1NOHvsPhi"+name_;
  IsoTrack1NOHvsPhi_  = fs->make<TH2F>(histname.c_str(),";1. Track, Number Of Hits;#phi;events", 40, 0.,  40., 40, -4.0, 4.0);
  IsoTrack1NOHvsPhi_->Sumw2();

  histname = "Track1PtvsPhi"+name_;
  Track1PtvsPhi_  = fs->make<TH2F>(histname.c_str(),";1. Track, Pt [GeV];#phi;events", 40, 0.,  20., 40, -4.0, 4.0);
  Track1PtvsPhi_->Sumw2();  
  histname = "Track2PtvsPhi"+name_;
  Track2PtvsPhi_  = fs->make<TH2F>(histname.c_str(),";2. Track, Pt [GeV];#phi;events", 40, 0.,  20., 40, -4.0, 4.0);
  Track2PtvsPhi_->Sumw2();  
  histname = "Track3PtvsPhi"+name_;
  Track3PtvsPhi_  = fs->make<TH2F>(histname.c_str(),";3. Track, Pt [GeV];#phi;events", 40, 0.,  20., 40, -4.0, 4.0);
  Track3PtvsPhi_->Sumw2();  
  histname = "Track4PtvsPhi"+name_;
  Track4PtvsPhi_  = fs->make<TH2F>(histname.c_str(),";4. Track, Pt [GeV];#phi;events", 40, 0.,  20., 40, -4.0, 4.0);
  Track4PtvsPhi_->Sumw2(); 
  histname = "IsoTrack1PtvsPhi"+name_;
  IsoTrack1PtvsPhi_  = fs->make<TH2F>(histname.c_str(),";1. Track, Pt [GeV];#phi;events", 40, 0.,  20., 40, -4.0, 4.0);
  IsoTrack1PtvsPhi_->Sumw2(); 

  histname = "TrackMult"+name_;
  TrackMult_ = fs->make<TH1F>(histname.c_str(),";Track multiplicity;events",  21, -0.5, 20.5);
  TrackMult_->Sumw2();  

  histname = "IsoTrack1DeltaR"+name_;
  IsoTrack1DeltaR_  = fs->make<TH1F>(histname.c_str(),";#DeltaR_{min} of 1. Track;events",  40, 0.0, 6.0);
  IsoTrack1DeltaR_->Sumw2();  
  histname = "IsoTrack1OuterDeltaR"+name_;
  IsoTrack1OuterDeltaR_  = fs->make<TH1F>(histname.c_str(),";#DeltaR_{min} of 1. Track;events",  40, 0.0, 6.0);
  IsoTrack1OuterDeltaR_->Sumw2();  

  histname = "TrackTower_E1x1"+name_;
  TrackTower_E1x1_  = fs->make<TH1F>(histname.c_str(),";Tower Energy in 1x1 cell, E [GeV];events",  40, 0.0, 20.0);
  TrackTower_E1x1_->Sumw2();  
  histname = "TrackTower_E3x3"+name_;
  TrackTower_E3x3_  = fs->make<TH1F>(histname.c_str(),";Tower Energy in 3x3 cell, E [GeV];events",  40, 0.0, 20.0);
  TrackTower_E3x3_->Sumw2();  

  histname = "Response_E1x1"+name_;
  Response_E1x1_  = fs->make<TH1F>(histname.c_str(),";Tower Response in 1x1 cell, #frac{E_{CaloTower} [GeV]}{Pt_{Track} [GeV]};events",  40, 0.0, 5.0);
  Response_E1x1_->Sumw2();  
  histname = "Response_E3x3"+name_;
  Response_E3x3_  = fs->make<TH1F>(histname.c_str(),";Tower Response in 3x3 cell, #frac{E_{CaloTower} [GeV]}{Pt_{Track} [GeV]};events",  40, 0.0, 5.0);
  Response_E3x3_->Sumw2();  

  histname = "TwoTracksDeltaPhi"+name_;
  TwoTracksDeltaPhi_  = fs->make<TH1F>(histname.c_str(),";#Delta#phi in two track events;events",  40, 0.0, 4.0);
  TwoTracksDeltaPhi_->Sumw2(); 
  histname = "TwoTracksDeltaPt"+name_;
  TwoTracksDeltaPt_  = fs->make<TH1F>(histname.c_str(),";|#DeltaPt| [GeV] in two track events;events",  40, 0.0, 20.0);
  TwoTracksDeltaPt_->Sumw2(); 

  histname = "TrackTower_dEta"+name_;
  TrackTower_dEta_ = fs->make<TH1F>(histname.c_str(),";#Delta#eta(track, tower);events",  40, -5.0, 5.0);
  TrackTower_dEta_->Sumw2();  
  
  histname = "TrackTower_dPhi"+name_;
  TrackTower_dPhi_ = fs->make<TH1F>(histname.c_str(),";#Delta#phi(track, tower);events",  40, 0.0, 4.0);
  TrackTower_dPhi_->Sumw2();  
  
  histname = "TrackTower_dPt"+name_;
  TrackTower_dPt_ = fs->make<TH1F>(histname.c_str(),";#DeltaPt(track, tower);events",  40, -20.0, 20.0);
  TrackTower_dPt_->Sumw2();  

  histogramsBooked = true;
}


FinalPlots::~FinalPlots()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//
// ------------ biased delta phi ----------------------------
double FinalPlots::dPhiBiased(const std::vector<const reco::Candidate*>& jets, const reco::Candidate& excljet)
{
  math::PtEtaPhiMLorentzVector htvec(0.0, 0.0, 0.0, 0.0);
  for (std::vector<const reco::Candidate*>::const_iterator jet=jets.begin();
       jet!=jets.end(); ++jet){
     if ( fabs( (*jet)->eta())>jetetamax_ ) continue;
     if ( (*jet)->pt()<jetptmin_ ) continue;
     if ( (*jet)->phi()==excljet.phi()&&(*jet)->eta()==excljet.eta() ) continue;
     htvec+=(*jet)->p4();
  }
  return fabs(deltaPhi(htvec, excljet));
}

std::vector<double> FinalPlots::deltaSumPt_permutations(const std::vector<LorentzV>& p4s) 
{
    std::vector<std::vector<double> > ht( 1<<(p4s.size()-1) , std::vector<double>( 2, 0.) );
    for(unsigned i=0; i < ht.size(); i++) {
      for(unsigned j=0; j < p4s.size(); j++) {
	ht [i] [(i/(1<<j))%2] += p4s[j].pt();
      }
    }
    std::vector<double> deltaHT;  for(unsigned i=0; i<ht.size(); i++) deltaHT.push_back(fabs(ht[i][0]-ht[i][1]));
    return deltaHT;
}
  
double FinalPlots::summedTowerEnergy(CaloTowerDetId& towerId, CaloTowerCollection towers, int ntw)
{
  double summedTowerEnergy=0;
  for(int iphi = towerId.iphi()-ntw; iphi<=towerId.iphi()+ntw; ++iphi){ 
    for(int ieta = towerId.ieta()-ntw; ieta<=towerId.ieta()+ntw; ++ieta){
      CaloTowerDetId id(ieta,iphi);
      CaloTowerCollection::const_iterator tower = towers.find(id);
      if(tower == towers.end() ) continue; 
      summedTowerEnergy+=tower->et();
    }
  }
  return summedTowerEnergy;
}

/*
void FinalPlots::createPDFs()
{
  edm::Service<TFileService> fs;
  if( !fs ){
    throw edm::Exception( edm::errors::Configuration,
                          "TFile Service is not registered in cfg file" );
  }
  
  TCanvas *cHT = fs->make<TCanvas>("cHT","cHT",800,800);
  cHT->cd(1);
  HT_->Draw();
  cHT->Update();
  cHT->Print("histograms/HT.pdf","Portrait pdf");
}
*/

// ------------ method called to for each event  ------------
void
FinalPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   if (!histogramsBooked) bookHistograms(iSetup);

   //Make Jet & MET plots: -------------------------------------------------
   edm::Handle< double > event_weight;
   iEvent.getByLabel(weightName_, event_weight);
   weight_ = (event_weight.isValid() ? (*event_weight) : 1.0);

   edm::Handle<edm::View<reco::Candidate> > jet_hnd;
   iEvent.getByLabel(Jet_, jet_hnd);
   edm::View<reco::Candidate> Jets = *jet_hnd;
   

   edm::Handle<edm::View<reco::MET> > met_hnd;
   iEvent.getByLabel(Met_, met_hnd);

   MET_->Fill( met_hnd->front().et(), weight_ );

   //apply JEC for PAT jets:
   std::vector<const reco::Candidate*> cJets;
   for (edm::View<reco::Candidate>::const_iterator jet=Jets.begin();
        jet!=Jets.end(); ++jet){
      const pat::Jet * patJet = 0;
      patJet = dynamic_cast <const pat::Jet*> ( &(*jet) );
      const reco::Candidate * cJet;
      if (patJet) 
	cJet = new pat::Jet( patJet->correctedJet("part", "b") );
      else 
        cJet = &(*jet);
      cJets.push_back( cJet );	
   }
   // sort jets in Et
   PtrGreaterByPt<const reco::Candidate*>        pTComparator_;
   std::sort(cJets.begin(), cJets.end(), pTComparator_);
      	
   double ht=0.0, dphimin=999., dphibiased=999.;
   int jetmult = 0;
   math::PtEtaPhiMLorentzVector htvec(0.0, 0.0, 0.0, 0.0);
   std::vector <LorentzV> forAlphaT;
   
   for (std::vector<const reco::Candidate*>::const_iterator cJet=cJets.begin();
        cJet!=cJets.end(); ++cJet) {

      if ( fabs((*cJet)->eta())>jetetamax_ ) continue;
      if ( (*cJet)->pt()<jetptmin_ ) continue;
      forAlphaT.push_back((*cJet)->p4());
      htvec+=(*cJet)->p4();
      ht+=(*cJet)->pt();
      AllJetsPt_->Fill( (*cJet)->pt() );
      if (jetmult==0) {
        Jet1E_->Fill( (*cJet)->energy(), weight_ );
        Jet1Pt_->Fill( (*cJet)->pt(), weight_ );
        Jet1Phi_->Fill( (*cJet)->phi(), weight_ );
        Jet1Eta_->Fill( (*cJet)->eta(), weight_ );
      }	
      else if (jetmult==1) {
        Jet2E_->Fill( (*cJet)->energy(), weight_ );
        Jet2Pt_->Fill( (*cJet)->pt(), weight_ );
        Jet2Phi_->Fill( (*cJet)->phi(), weight_ );
        Jet2Eta_->Fill( (*cJet)->eta(), weight_ );
      }
      else if (jetmult==2) {
        Jet3E_->Fill( (*cJet)->energy(), weight_ );
        Jet3Pt_->Fill( (*cJet)->pt(), weight_ );
        Jet3Phi_->Fill( (*cJet)->phi(), weight_ );
        Jet3Eta_->Fill( (*cJet)->eta(), weight_ );
      }
      else if (jetmult==3) {
        Jet4E_->Fill( (*cJet)->energy(), weight_ );
        Jet4Pt_->Fill( (*cJet)->pt(), weight_ );
        Jet4Phi_->Fill( (*cJet)->phi(), weight_ );
        Jet4Eta_->Fill( (*cJet)->eta(), weight_ );
      }
      ++jetmult;
      
      if ( fabs(deltaPhi(**cJet, met_hnd->front())) < dphimin )
        dphimin = fabs(deltaPhi( **cJet, met_hnd->front()));
	
      double thisdphi = dPhiBiased(cJets, **cJet);
      if (thisdphi < dphibiased)
        dphibiased = thisdphi;

  }    
  HT_ ->Fill( ht, weight_ );
  MHT_->Fill( htvec.pt(), weight_ );
  JetMult_->Fill( jetmult, weight_ );
  dPhiMin_->Fill( dphimin, weight_ );
  dPhiBiased_->Fill( dphibiased, weight_ );

  //other variables
  //MPT...   


  //Analyze Tracks <-> Towers ----------------------------------------------------------------------

  edm::Handle<CaloTowerCollection> towers;
  iEvent.getByLabel(towers_,towers);

  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(tracks_,tracks);
  reco::TrackCollection trackcollection = *tracks;

  std::vector<const reco::Track*> trackPtrs;
  reco::TrackCollection::const_iterator tracki = tracks->begin();
  for( ; tracki!=tracks->end(); ++tracki ){
      trackPtrs.push_back( &(*tracki) );	
  }

  // sort tracks in Pt
  PtrGreaterByPt<const reco::Track*>        trackPtComparator_;
  std::sort(trackPtrs.begin(), trackPtrs.end(), trackPtComparator_);


/*
  double trkIso=-1.;
  reco::Track selTrack;
  //----------------------------------------------------------------
  //search for most isolated track of good quality
  //----------------------------------------------------------------
  //Isolation::const_iterator iso = isolation->begin();

  //these parameters should be defined in the cfg file:
  double maxEta_ = 2.4;
  double minPt_  = 5.0;
  double maxChiSquare_ = 40.0; //just a guess!
  int    minNumOfHits_ = 10;

  reco::TrackCollection::const_iterator track = tracks->begin();
  for(iso = isolation->begin(); iso!=isolation->end(); ++iso ){

    
    //double isolation = *iso-track->pt();
    //if(trkIso<0 || isolation<trkIso){
    //  selTrack =*track;
    //  trkIso=isolation;
    //}
  }
*/


 
  int trackmult = 0;
  bool found_isolated_track = false;
  std::vector<const reco::Track*>::const_iterator track = trackPtrs.begin();

  double trackDeltaR, deltaR;
  double trackOuterDeltaR, outerDeltaR;
  reco::Track track1, track2;
  reco::TrackCollection::const_iterator iso;
  for( ; track!=trackPtrs.end(); ++track ){ //loop over all tracks

    if(	!(fabs((*track)->eta())<tmaxEta_) ) continue;
    if(	!(tminPt_<(*track)->pt()) )  continue;
    if( !((*track)->normalizedChi2()<tmaxChiSquare_) ) continue;
    if( !((*track)->found()>tminNumOfHits_) ) continue;
    //check also for isolation
    //e.g. check also if track is a Pion (if running on MC)

    AllTracksPt_->Fill( (*track)->pt() );
    AllTracksPhi_->Fill( (*track)->phi() );
    if (trackmult==0) {
        track1 = *(*track);
        Track1Pt_->Fill( (*track)->pt(), weight_ );
        Track1Phi_->Fill( (*track)->phi(), weight_ );
        Track1Eta_->Fill( (*track)->eta(), weight_ );
        Track1NumOfHits_->Fill( (*track)->found(), weight_ );
        Track1ChiSquare_->Fill( (*track)->normalizedChi2(), weight_ );
        Track1NOHvsPhi_->Fill( (*track)->found(), (*track)->phi(), weight_ );
        Track1PtvsPhi_->Fill( (*track)->pt(), (*track)->phi(), weight_ );
    }	
    else if (trackmult==1) {
        track2 = *(*track);
        Track2Pt_->Fill( (*track)->pt(), weight_ );
        Track2Phi_->Fill( (*track)->phi(), weight_ );
        Track2Eta_->Fill( (*track)->eta(), weight_ );
        Track2NumOfHits_->Fill( (*track)->found(), weight_ );
        Track2ChiSquare_->Fill( (*track)->normalizedChi2(), weight_ );
        Track2NOHvsPhi_->Fill( (*track)->found(), (*track)->phi(), weight_ );
        Track2PtvsPhi_->Fill( (*track)->pt(), (*track)->phi(), weight_ );
    }
    else if (trackmult==2) {
        Track3Pt_->Fill( (*track)->pt(), weight_ );
        Track3Phi_->Fill( (*track)->phi(), weight_ );
        Track3Eta_->Fill( (*track)->eta(), weight_ );
        Track3NumOfHits_->Fill( (*track)->found(), weight_ );
        Track3ChiSquare_->Fill( (*track)->normalizedChi2(), weight_ );
        Track3NOHvsPhi_->Fill( (*track)->found(), (*track)->phi(), weight_ );
        Track3PtvsPhi_->Fill( (*track)->pt(), (*track)->phi(), weight_ );
    }
    else if (trackmult==3) {
        Track4Pt_->Fill( (*track)->pt(), weight_ );
        Track4Phi_->Fill( (*track)->phi(), weight_ );
        Track4Eta_->Fill( (*track)->eta(), weight_ );
        Track4NumOfHits_->Fill( (*track)->found(), weight_ );
        Track4ChiSquare_->Fill( (*track)->normalizedChi2(), weight_ );
        Track4NOHvsPhi_->Fill( (*track)->found(), (*track)->phi(), weight_ );
        Track4PtvsPhi_->Fill( (*track)->pt(), (*track)->phi(), weight_ );
    }
    else if (trackmult==14) {
        Track15Phi_->Fill( (*track)->phi(), weight_ );
    }



  //----------------------------------------------------------------
  //search for highest Pt isolated track that is isolated enough
  //----------------------------------------------------------------
  if (!found_isolated_track) //only leading isolated track
  { 
    trackDeltaR = -1.;
    trackOuterDeltaR = -1.;
    iso  = tracks->begin();
    ++iso;
    for(; iso!=tracks->end(); ++iso )
    {
      deltaR = reco::deltaR((*track)->eta(), (*track)->phi(), iso->eta(), iso->phi());
      outerDeltaR = reco::deltaR((*track)->outerEta(), (*track)->outerPhi(), iso->outerEta(), iso->outerPhi());
      if (deltaR < tminDeltaR_ || outerDeltaR < tminDeltaR_)
      {
        trackDeltaR = -1.;
        trackOuterDeltaR = -1.;
        continue;
      }
      if(trackDeltaR < 0 || deltaR < trackDeltaR)
      {
        trackDeltaR = deltaR;
      }
      if(trackOuterDeltaR < 0 || outerDeltaR < trackOuterDeltaR)
      {
        trackOuterDeltaR = outerDeltaR;
      }
    }

    if (trackDeltaR >= 0)
    {
        found_isolated_track = true;
        IsoTrack1Pt_->Fill( (*track)->pt(), weight_ );
        IsoTrack1Phi_->Fill( (*track)->phi(), weight_ );
        IsoTrack1Eta_->Fill( (*track)->eta(), weight_ );
        IsoTrack1NumOfHits_->Fill( (*track)->found(), weight_ );
        IsoTrack1ChiSquare_->Fill( (*track)->normalizedChi2(), weight_ );
        IsoTrack1NOHvsPhi_->Fill( (*track)->found(), (*track)->phi(), weight_ );
        IsoTrack1PtvsPhi_->Fill( (*track)->pt(), (*track)->phi(), weight_ );
        IsoTrack1DeltaR_->Fill( trackDeltaR, weight_ );
    }
    if (trackOuterDeltaR >= 0)
    {
        IsoTrack1OuterDeltaR_->Fill( trackOuterDeltaR, weight_ );
    }


    //Propagate a track to the calorimeter surface:
/*
    edm::ESHandle<MagneticField> field;
    iSetup.get<IdealMagneticFieldRecord>().get(field);  
*/


    edm::ESHandle<CaloGeometry> geometry;
    iSetup.get<CaloGeometryRecord>().get(geometry);
    const CaloSubdetectorGeometry* towerGeometry = geometry->getSubdetectorGeometry(DetId::Calo, CaloTowerDetId::SubdetId);

    //these parameters should be defined in the cfg file:
    double maxDeltaEta_ = 0.3;
    double maxDeltaPhi_ = 0.3;


    double en1x1=0, enSum=0, dPhi=0, dEta=0;
    if(trackDeltaR >= 0){ // && trkIso<maxIsolation_ ){
      //----------------------------------------------------------------
      //propagate track to hcal and 
      //search for closest calo tower
      //----------------------------------------------------------------

/*
      const GlobalPoint vtx((*track)->vx(), (*track)->vy(), (*track)->vz());
      GlobalVector atHcal((*track)->px(), (*track)->py(), (*track)->pz());//track at hcal
      toHcal_->propagate( vtx, atHcal, (*track)->charge(), &*field);  //no field atm
      const GlobalPoint poI(atHcal.x(), atHcal.y(), atHcal.z()); //point of incidence
      CaloTowerDetId towerId = towerGeometry->getClosestCell(poI);    
    
      CaloTowerDetId id(towerId.ieta(),towerId.iphi());
      CaloTowerCollection::const_iterator selTower = towers->find(id);
      if(selTower!=towers->end()){
*/

//      GlobalVector::Polar atOuterTracker((*track)->outerTheta(), (*track)->outerPhi(), 1.5*(*track)->outerRadius());
//      const GlobalPoint poI(atOuterTracker.x(), atOuterTracker.y(), atOuterTracker.z()); //point of incidence
      const GlobalPoint poI ( GlobalPoint::Polar((*track)->outerTheta(), (*track)->outerPhi(), 1.5*(*track)->outerRadius()) ); //point of incidence
      CaloTowerDetId towerId = towerGeometry->getClosestCell(poI);    
    
      CaloTowerDetId id(towerId.ieta(),towerId.iphi());
      fprintf(stdout, "ieta = %i, iphi = %i\n", towerId.ieta(), towerId.iphi());
      CaloTowerCollection::const_iterator selTower = towers->find(id);
      tracksFound++;
      if(selTower!=towers->end()){
        towersFound++;


        dEta = selTower->eta()-(**track).eta();
        dPhi = deltaPhi(selTower->phi(),(**track).phi());
      
        if( fabs(dEta)<maxDeltaEta_ && fabs(dPhi<maxDeltaPhi_) ){
	  for(int iphi = towerId.iphi()-nTowerGroup_; iphi<=towerId.iphi()+nTowerGroup_; ++iphi){ 
	    for(int ieta = towerId.ieta()-nTowerGroup_; ieta<=towerId.ieta()+nTowerGroup_; ++ieta){
              CaloTowerDetId id(ieta,iphi);
              CaloTowerCollection::const_iterator twr = towers->find(id);
	      if(twr == towers->end() ) continue;   
	    }
	  }
	  en1x1 = summedTowerEnergy(towerId, *towers, 0);
	  enSum = summedTowerEnergy(towerId, *towers, nTowerGroup_);
          TrackTower_E1x1_->Fill(en1x1);
          TrackTower_E3x3_->Fill(enSum);
          Response_E1x1_->Fill(en1x1 / (*track)->pt());
          Response_E3x3_->Fill(enSum / (*track)->pt());
        }
      }
    }

  }


    double dRmin2 = 99999.;
    double dEta=0, dPhi=0, dPt=0, dEtaMin, dPhiMin;
    
    //loop over all towers
    for (CaloTowerCollection::const_iterator tower = towers->begin(); tower!=towers->end(); ++tower) {

      //dEta = track->eta() - tower->p4( track.vertex() ).eta();
      dEta = (*track)->outerEta() - tower->eta();
      dPhi = deltaPhi((*track)->outerPhi(),tower->phi());
      if (dEta*dEta+dPhi*dPhi<dRmin2) { //find closest tower to track
        dRmin2 = dEta*dEta+dPhi*dPhi;
	dPt = (*track)->pt() - tower->pt();
	dEtaMin = dEta;
	dPhiMin = dPhi;
      }
    }

    //fill histograms
    TrackTower_dEta_->Fill( dEtaMin );
    TrackTower_dPhi_->Fill( dPhiMin );
    TrackTower_dPt_ ->Fill( dPt );

    ++trackmult;
  }
  TrackMult_->Fill( trackmult, weight_ );

  if (trackmult == 2)
  {
    TwoTracksDeltaPhi_->Fill( reco::deltaPhi(track1.phi(), track2.phi()) );
    TwoTracksDeltaPt_->Fill( fabs(track1.pt() - track2.pt()) );
  }
}


// ------------ method called once each job just after ending the event loop  ------------
void 
FinalPlots::endJob() {
   fprintf(stdout, "Found %i Towers for %i Tracks.\n", towersFound, tracksFound);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FinalPlots);
