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
// $Id: FinalPlots.cc,v 1.7 2010/01/06 13:42:37 auterman Exp $
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
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
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
      double dPhiBiased(const edm::View<reco::Candidate>*jets,
                        const edm::View<reco::Candidate>::const_iterator jet);


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
      TH1F * Jet1Pt_, *Jet2Pt_, *Jet3Pt_, *Jet4Pt_;
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
double FinalPlots::dPhiBiased(const edm::View<reco::Candidate> * jets, const edm::View<reco::Candidate>::const_iterator excljet)
{
  math::PtEtaPhiMLorentzVector htvec(0.0, 0.0, 0.0, 0.0);
  for (edm::View<reco::Candidate>::const_iterator jet=jets->begin();
       jet!=jets->end(); ++jet){
     if ( fabs(jet->eta())>jetetamax_ ) continue;
     if ( jet->pt()<jetptmin_ ) continue;
     if ( jet==excljet ) continue;
     htvec+=jet->p4();
  }
  return deltaPhi(htvec, *excljet);
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

   edm::Handle<edm::View<reco::MET> > met_hnd;
   iEvent.getByLabel(Met_, met_hnd);

   MET_->Fill( met_hnd->front().et(), weight_ );
   
   double ht=0.0, dphimin=999., dphibiased=999.;
   int jetmult = 0;
   math::PtEtaPhiMLorentzVector htvec(0.0, 0.0, 0.0, 0.0);
   std::vector <LorentzV> forAlphaT;
   for (edm::View<reco::Candidate>::const_iterator jet=jet_hnd->begin();
        jet!=jet_hnd->end(); ++jet){
      if ( fabs(jet->eta())>jetetamax_ ) continue;
      if ( jet->pt()<jetptmin_ ) continue;
      forAlphaT.push_back(jet->p4());
      htvec+=jet->p4();
      ht+=jet->pt();
      if      (jetmult==0) Jet1Pt_->Fill( jet->pt(), weight_ );
      else if (jetmult==1) Jet2Pt_->Fill( jet->pt(), weight_ );
      else if (jetmult==2) Jet3Pt_->Fill( jet->pt(), weight_ );
      else if (jetmult==3) Jet4Pt_->Fill( jet->pt(), weight_ );
      ++jetmult;
      
      if ( deltaPhi(*jet, met_hnd->front()) < dphimin )
        dphimin = deltaPhi(*jet, met_hnd->front());
	
      double thisdphi = dPhiBiased(&(*jet_hnd), jet);
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
