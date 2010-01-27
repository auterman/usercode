// -*- C++ -*-
//
// Package:    FinalPlots
// Class:      FinalPlots
// 
/**\class FinalPlots FinalPlots.cc RA2/FinalPlots/src/FinalPlots.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann,68/112,2115,
//         Created:  Sun Oct 18 20:00:45 CEST 2009
// $Id: FinalPlots.cc,v 1.1.1.1 2010/01/07 15:54:11 auterman Exp $
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


      typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzV;
      std::vector<double> deltaSumPt_permutations(const std::vector<LorentzV>& p4s); 
      double alphaT(const std::vector<LorentzV>& p4s); 

      // ----------member data ---------------------------
      edm::InputTag weightName_;
      edm::InputTag Jet_;
      edm::InputTag Met_;
      std::string name_;
      double weight_, jetptmin_, jetetamax_;

      TH1F * HT_;
      TH1F * MHT_;
      TH1F * MET_; 
      TH1F * AllJetsPt_;
      TH1F * Jet1Pt_, *Jet2Pt_, *Jet3Pt_, *Jet4Pt_;
      TH1F * Jet1E_,  *Jet2E_,  *Jet3E_,  *Jet4E_;
      TH1F * Jet1Phi_,*Jet2Phi_,*Jet3Phi_,*Jet4Phi_;
      TH1F * Jet1Eta_,*Jet2Eta_,*Jet3Eta_,*Jet4Eta_;
      TH1F * JetMult_; 
      TH1F * alphaT_;
      TH1F * dPhiMin_;
      TH1F * dPhiBiased_;
  
};

FinalPlots::FinalPlots(const edm::ParameterSet& iConfig):
  weightName_(iConfig.getParameter<edm::InputTag>("weightName") ),
  Jet_(       iConfig.getParameter<edm::InputTag>("Jet") ),
  Met_(       iConfig.getParameter<edm::InputTag>("MET") ),
  name_(      iConfig.getParameter<std::string>("uncertainty_name") ),
  jetptmin_ ( iConfig.getParameter<double>("JetPtMin") ),
  jetetamax_( iConfig.getParameter<double>("JetEtaMax") )
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
  HT_    = fs->make<TH1F>(histname.c_str(),";HT [GeV];events",   200, 0.0, 2000.0);
  HT_->Sumw2();  

  histname = "MHT"+name_;
  MHT_     = fs->make<TH1F>(histname.c_str(),";MHT [GeV];events",  200, 0.0, 1000.0);
  MHT_->Sumw2();  
  histname = "MET"+name_;
  MET_     = fs->make<TH1F>(histname.c_str(),";MET [GeV];events",  200, 0.0, 1000.0);
  MET_->Sumw2();  

  histname = "AllJetsPt"+name_;
  AllJetsPt_  = fs->make<TH1F>(histname.c_str(),";All Jets Pt [GeV];events",  200, 0.0, 1000.0);
  AllJetsPt_->Sumw2();  

  histname = "Jet1Pt"+name_;
  Jet1Pt_  = fs->make<TH1F>(histname.c_str(),";1. Jet Pt [GeV];events",  200, 0.0, 1000.0);
  Jet1Pt_->Sumw2();  
  histname = "Jet2Pt"+name_;
  Jet2Pt_  = fs->make<TH1F>(histname.c_str(),";2. Jet Pt [GeV];events",  200, 0.0, 1000.0);
  Jet2Pt_->Sumw2();  
  histname = "Jet3Pt"+name_;
  Jet3Pt_  = fs->make<TH1F>(histname.c_str(),";3. Jet Pt [GeV];events",  200, 0.0, 1000.0);
  Jet3Pt_->Sumw2();  
  histname = "Jet4Pt"+name_;
  Jet4Pt_  = fs->make<TH1F>(histname.c_str(),";4. Jet Pt [GeV];events",  200, 0.0, 1000.0);
  Jet4Pt_->Sumw2();  

  histname = "Jet1E"+name_;
  Jet1E_  = fs->make<TH1F>(histname.c_str(),";1. Jet E [GeV];events",  200, 0.0, 1000.0);
  Jet1E_->Sumw2();  
  histname = "Jet2E"+name_;
  Jet2E_  = fs->make<TH1F>(histname.c_str(),";2. Jet E [GeV];events",  200, 0.0, 1000.0);
  Jet2E_->Sumw2();  
  histname = "Jet3E"+name_;
  Jet3E_  = fs->make<TH1F>(histname.c_str(),";3. Jet E [GeV];events",  200, 0.0, 1000.0);
  Jet3E_->Sumw2();  
  histname = "Jet4E"+name_;
  Jet4E_  = fs->make<TH1F>(histname.c_str(),";4. Jet E [GeV];events",  200, 0.0, 1000.0);
  Jet4E_->Sumw2();  

  histname = "Jet1Phi"+name_;
  Jet1Phi_  = fs->make<TH1F>(histname.c_str(),";1. Jet #Phi;events", 200, -3.2,  3.2);
  Jet1Phi_->Sumw2();  
  histname = "Jet2Phi"+name_;
  Jet2Phi_  = fs->make<TH1F>(histname.c_str(),";2. Jet #Phi;events", 200, -3.2,  3.2);
  Jet2Phi_->Sumw2();  
  histname = "Jet3Phi"+name_;
  Jet3Phi_  = fs->make<TH1F>(histname.c_str(),";3. Jet #Phi;events", 200, -3.2,  3.2);
  Jet3Phi_->Sumw2();  
  histname = "Jet4Phi"+name_;
  Jet4Phi_  = fs->make<TH1F>(histname.c_str(),";4. Jet #Phi;events", 200, -3.2,  3.2);
  Jet4Phi_->Sumw2();  

  histname = "Jet1Eta"+name_;
  Jet1Eta_  = fs->make<TH1F>(histname.c_str(),";1. Jet #Eta;events", 200, -4.0,  4.0);
  Jet1Eta_->Sumw2();  
  histname = "Jet2Eta"+name_;
  Jet2Eta_  = fs->make<TH1F>(histname.c_str(),";2. Jet #Eta;events", 200, -4.0,  4.0);
  Jet2Eta_->Sumw2();  
  histname = "Jet3Eta"+name_;
  Jet3Eta_  = fs->make<TH1F>(histname.c_str(),";3. Jet #Eta;events", 200, -4.0,  4.0);
  Jet3Eta_->Sumw2();  
  histname = "Jet4Eta"+name_;
  Jet4Eta_  = fs->make<TH1F>(histname.c_str(),";4. Jet #Eta;events", 200, -4.0,  4.0);
  Jet4Eta_->Sumw2();  

  histname = "JetMult"+name_;
  JetMult_ = fs->make<TH1F>(histname.c_str(),";Jet multiplicity;events",  21, -0.5, 20.5);
  JetMult_->Sumw2();  
  histname = "alphaT"+name_;
  alphaT_ = fs->make<TH1F>(histname.c_str(),";alphaT;events",  200, 0.0, 2.0);
  alphaT_->Sumw2();  
  histname = "dPhiMin"+name_;
  dPhiMin_ = fs->make<TH1F>(histname.c_str(),";#Delta#phi_{min};events",  200, 0.0, 3.2);
  dPhiMin_->Sumw2();  
  histname = "dPhiBiased"+name_;
  dPhiBiased_ = fs->make<TH1F>(histname.c_str(),";#Delta#phi_{biased};events",  200, 0.0, 3.2);
  dPhiBiased_->Sumw2();  
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
  
double FinalPlots::alphaT(const std::vector<LorentzV>& p4s) 
{
    if(p4s.size()<2) return 0;
    
    std::vector<double> pTs; 
    for(unsigned i=0; i<p4s.size(); i++) pTs.push_back(p4s[i].pt());
    for(unsigned i=0; i<p4s.size(); i++) pTs.push_back(p4s[i].pt());
    const std::vector<double> DHTper( deltaSumPt_permutations(p4s) );
    
    const double mDHT = *(std::min_element( DHTper.begin(), DHTper.end() ));
    const double sumPT = accumulate( pTs.begin(), pTs.end(), double(0) );
    const LorentzV sumP4 = accumulate( p4s.begin(), p4s.end(), LorentzV() );
  
    return 0.5 * ( sumPT - mDHT ) / sqrt( sumPT*sumPT - sumP4.perp2() );
}


// ------------ method called to for each event  ------------
void
FinalPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

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
   alphaT_->Fill( alphaT(forAlphaT), weight_ );
   dPhiMin_->Fill( dphimin, weight_ );
   dPhiBiased_->Fill( dphibiased, weight_ );

   //other variables
   //MPT...   
}


// ------------ method called once each job just after ending the event loop  ------------
void 
FinalPlots::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FinalPlots);
