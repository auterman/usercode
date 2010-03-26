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
// $Id: FinalPlots.cc,v 1.2 2010/01/27 13:34:08 auterman Exp $
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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "PhysicsTools/IsolationAlgos/interface/PropagateToCal.h"

#include "TH1F.h"

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
      virtual void beginJob(const edm::EventSetup&);
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
      edm::InputTag tracks_;
      edm::InputTag towers_;
      PropagateToCal* toEcal_;
      PropagateToCal* toHcal_;
      TH1F * TrackTower_dEta_, *TrackTower_dPhi_, *TrackTower_dPt_;
      int nTowerGroup_;
  
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
  nTowerGroup_( iConfig.getParameter<int>( "GroupNTowers" ) )
{
}

// ------------ method called once each job just before starting event loop  ------------
void 
FinalPlots::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  if( !fs ){
    throw edm::Exception( edm::errors::Configuration,
                          "TFile Service is not registered in cfg file" );
  }

  std::string histname = "HT"+name_;
  HT_    = fs->make<TH1F>(histname.c_str(),";HT [GeV];events",   40, 0.0, 100.0);
  HT_->Sumw2();  

  histname = "MHT"+name_;
  MHT_     = fs->make<TH1F>(histname.c_str(),";MHT [GeV];events",  40, 0.0, 50.0);
  MHT_->Sumw2();  
  histname = "MET"+name_;
  MET_     = fs->make<TH1F>(histname.c_str(),";MET [GeV];events",  40, 0.0, 50.0);
  MET_->Sumw2();  

  histname = "AllJetsPt"+name_;
  AllJetsPt_  = fs->make<TH1F>(histname.c_str(),";All Jets Pt [GeV];events",  40, 0.0, 50.0);
  AllJetsPt_->Sumw2();  

  histname = "Jet1Pt"+name_;
  Jet1Pt_  = fs->make<TH1F>(histname.c_str(),";1. Jet Pt [GeV];events",  40, 0.0, 20.0);
  Jet1Pt_->Sumw2();  
  histname = "Jet2Pt"+name_;
  Jet2Pt_  = fs->make<TH1F>(histname.c_str(),";2. Jet Pt [GeV];events",  40, 0.0, 20.0);
  Jet2Pt_->Sumw2();  
  histname = "Jet3Pt"+name_;
  Jet3Pt_  = fs->make<TH1F>(histname.c_str(),";3. Jet Pt [GeV];events",  40, 0.0, 20.0);
  Jet3Pt_->Sumw2();  
  histname = "Jet4Pt"+name_;
  Jet4Pt_  = fs->make<TH1F>(histname.c_str(),";4. Jet Pt [GeV];events",  40, 0.0, 20.0);
  Jet4Pt_->Sumw2();  

  histname = "Jet1E"+name_;
  Jet1E_  = fs->make<TH1F>(histname.c_str(),";1. Jet E [GeV];events",  40, 0.0, 20.0);
  Jet1E_->Sumw2();  
  histname = "Jet2E"+name_;
  Jet2E_  = fs->make<TH1F>(histname.c_str(),";2. Jet E [GeV];events",  40, 0.0, 20.0);
  Jet2E_->Sumw2();  
  histname = "Jet3E"+name_;
  Jet3E_  = fs->make<TH1F>(histname.c_str(),";3. Jet E [GeV];events",  40, 0.0, 20.0);
  Jet3E_->Sumw2();  
  histname = "Jet4E"+name_;
  Jet4E_  = fs->make<TH1F>(histname.c_str(),";4. Jet E [GeV];events",  40, 0.0, 20.0);
  Jet4E_->Sumw2();  

  histname = "Jet1Phi"+name_;
  Jet1Phi_  = fs->make<TH1F>(histname.c_str(),";1. Jet #Phi;events", 40, -3.2,  3.2);
  Jet1Phi_->Sumw2();  
  histname = "Jet2Phi"+name_;
  Jet2Phi_  = fs->make<TH1F>(histname.c_str(),";2. Jet #Phi;events", 40, -3.2,  3.2);
  Jet2Phi_->Sumw2();  
  histname = "Jet3Phi"+name_;
  Jet3Phi_  = fs->make<TH1F>(histname.c_str(),";3. Jet #Phi;events", 40, -3.2,  3.2);
  Jet3Phi_->Sumw2();  
  histname = "Jet4Phi"+name_;
  Jet4Phi_  = fs->make<TH1F>(histname.c_str(),";4. Jet #Phi;events", 40, -3.2,  3.2);
  Jet4Phi_->Sumw2();  

  histname = "Jet1Eta"+name_;
  Jet1Eta_  = fs->make<TH1F>(histname.c_str(),";1. Jet #Eta;events", 40, -4.0,  4.0);
  Jet1Eta_->Sumw2();  
  histname = "Jet2Eta"+name_;
  Jet2Eta_  = fs->make<TH1F>(histname.c_str(),";2. Jet #Eta;events", 40, -4.0,  4.0);
  Jet2Eta_->Sumw2();  
  histname = "Jet3Eta"+name_;
  Jet3Eta_  = fs->make<TH1F>(histname.c_str(),";3. Jet #Eta;events", 40, -4.0,  4.0);
  Jet3Eta_->Sumw2();  
  histname = "Jet4Eta"+name_;
  Jet4Eta_  = fs->make<TH1F>(histname.c_str(),";4. Jet #Eta;events", 40, -4.0,  4.0);
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
  histname = "TrackTower_dEta"+name_;
  TrackTower_dEta_ = fs->make<TH1F>(histname.c_str(),";#Delta#Eta(track, tower);events",  40, -5.0, 5.0);
  TrackTower_dEta_->Sumw2();  
  
  histname = "TrackTower_dPhi"+name_;
  TrackTower_dPhi_ = fs->make<TH1F>(histname.c_str(),";#Delta#Phi(track, tower);events",  40, 0.0, 4.0);
  TrackTower_dPhi_->Sumw2();  
  
  histname = "TrackTower_dPt"+name_;
  TrackTower_dPt_ = fs->make<TH1F>(histname.c_str(),";#Delta#Pt(track, tower);events",  40, -20.0, 20.0);
  TrackTower_dPt_->Sumw2();  
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


// ------------ method called to for each event  ------------
void
FinalPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

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

  //Propagate a track to the calorimeter surface:
 /*
  edm::ESHandle<MagneticField> field;
  iSetup.get<IdealMagneticFieldRecord>().get(field);
  
  edm::ESHandle<CaloGeometry> geometry;
  iSetup.get<IdealGeometryRecord>().get(geometry);
  const CaloSubdetectorGeometry* towerGeometry = geometry->getSubdetectorGeometry(DetId::Calo, CaloTowerDetId::SubdetId);


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
  for( ; track!=tracks->end(); //, iso!=isolation->end(); 
       ++track//,++iso
       ){
    if(	!(fabs(track->eta())<maxEta_) ) 
      continue;

    if(	!(minPt_<track->pt()) ) 
      continue;
    
    if( !(track->normalizedChi2()<maxChiSquare_) )
      continue;

    if( !(track->found()>minNumOfHits_) )
      continue;
    
    //double isolation = *iso-track->pt();
    //if(trkIso<0 || isolation<trkIso){
    //  selTrack =*track;
    //  trkIso=isolation;
    //}
  }

  //these parameters should be defined in the cfg file:
  double maxDeltaEta_ = 0.3;
  double maxDeltaPhi_ = 0.3;

  double en1x1=0, enSum=0, dPhi=0, dEta=0;
  if( 0<=trkIso ){ // && trkIso<maxIsolation_ ){
    //----------------------------------------------------------------
    //propagate track to hcal and 
    //search for closest calo tower
    //----------------------------------------------------------------
    const GlobalPoint vtx(selTrack.vx(), selTrack.vy(), selTrack.vz());
    GlobalVector atHcal(selTrack.px(), selTrack.py(), selTrack.pz());//track at hcal
    toHcal_->propagate( vtx, atHcal, selTrack.charge(), &*field);  
    const GlobalPoint poI(atHcal.x(), atHcal.y(), atHcal.z()); //point of incidence
    CaloTowerDetId towerId = towerGeometry->getClosestCell(poI);    
    
    CaloTowerDetId id(towerId.ieta(),towerId.iphi());
    CaloTowerCollection::const_iterator selTower = towers->find(id);
    if(selTower!=towers->end()){
      dEta = selTower->eta()-selTrack.eta();
      dPhi = deltaPhi(selTower->phi(),selTrack.phi());
      
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
      }
    }
  }
*/

  //these parameters should be defined in the cfg file:
  double maxEta_ = 2.4;
  double minPt_  = 5.0;
  double maxChiSquare_ = 40.0; //just a guess!
  int    minNumOfHits_ = 10;
  reco::TrackCollection::const_iterator track = tracks->begin();
  for( ; track!=tracks->end(); ++track ){ //loop over all tracks
    if(	!(fabs(track->eta())<maxEta_) ) continue;
    if(	!(minPt_<track->pt()) )  continue;
    if( !(track->normalizedChi2()<maxChiSquare_) ) continue;
    if( !(track->found()>minNumOfHits_) ) continue;
    //check also for isolation
    //e.g. check also if track is a Pion (if running on MC)

    double dRmin2 = 99999.;
    double dEta=0, dPhi=0, dPt=0, dEtaMin, dPhiMin;
    
    //loop over all towers
    for (CaloTowerCollection::const_iterator tower = towers->begin(); tower!=towers->end(); ++tower) {

      //dEta = track->eta() - tower->p4( track.vertex() ).eta();
      dEta = track->outerEta() - tower->eta();
      dPhi = deltaPhi(track->outerPhi(),tower->phi());
      if (dEta*dEta+dPhi*dPhi<dRmin2) { //find closest tower to track
        dRmin2 = dEta*dEta+dPhi*dPhi;
	dPt = track->pt() - tower->pt();
	dEtaMin = dEta;
	dPhiMin = dPhi;
      }
    }
    
    //fill histograms
    TrackTower_dEta_->Fill( dEtaMin );
    TrackTower_dPhi_->Fill( dPhiMin );
    TrackTower_dPt_ ->Fill( dPt );
  }
}


// ------------ method called once each job just after ending the event loop  ------------
void 
FinalPlots::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FinalPlots);
