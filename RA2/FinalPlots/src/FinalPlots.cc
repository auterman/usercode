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
// $Id: FinalPlots.cc,v 1.5 2009/10/19 15:02:33 auterman Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
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

      // ----------member data ---------------------------
      edm::InputTag weightName_;
      edm::InputTag Jet_;
      edm::InputTag Met_;
      std::string name_;
      double weight_;

      TH1F * HT_;
      TH1F * MHT_;
      TH1F * MET_;   
   
};

FinalPlots::FinalPlots(const edm::ParameterSet& iConfig):
  weightName_(  iConfig.getParameter<edm::InputTag>("weightName") ),
  Jet_(  iConfig.getParameter<edm::InputTag>("Jet") ),
  Met_(  iConfig.getParameter<edm::InputTag>("MET") ),
  name_( iConfig.getParameter<std::string>("uncertainty_name") )
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
  HT_    = fs->make<TH1F>(histname.c_str(),";HT [GeV];events",   100, 0.0, 500.0);

  histname = "MHT"+name_;
  MHT_   = fs->make<TH1F>(histname.c_str(),";MHT [GeV];events",  100, 0.0, 500.0);

  histname = "MET"+name_;
  MET_   = fs->make<TH1F>(histname.c_str(),";MET [GeV];events",  100, 0.0, 500.0);
}


FinalPlots::~FinalPlots()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
FinalPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   edm::Handle< double > event_weight;
   iEvent.getByLabel(weightName_, event_weight);
   weight_ = (event_weight.isValid() ? (*event_weight) : 1.0);
//**************************************************
// This is for debugging purposes only !!!!!
if      (name_=="_JEC_UP")    weight *= 1.08;
else if (name_=="_JEC_DN")    weight *= 0.92;
else if (name_=="_method_UP") weight *= 1.04;
else if (name_=="_method_DN") weight *= 0.96;
// This is for debugging purposes only !!!!!
//**************************************************

   edm::Handle<edm::View<reco::Candidate> > jet_hnd;
   iEvent.getByLabel(Jet_, jet_hnd);

   edm::Handle<edm::View<reco::MET> > met_hnd;
   iEvent.getByLabel(Met_, met_hnd);

   MET_->Fill( met_hnd->begin()->pt(), weight );
   
   double ht=0.0;
   math::PtEtaPhiMLorentzVector htvec(0.0, 0.0, 0.0, 0.0);
   for (edm::View<reco::Candidate>::const_iterator jet=jet_hnd->begin();
        jet!=jet_hnd->end(); ++jet){
      if ( fabs(jet->eta())>3.0 ) continue;
      if ( jet->pt()<30. ) continue;
      htvec += jet->p4();
      ht+=jet->pt();
   }	
   HT_ ->Fill( ht, weight );
   MHT_->Fill( htvec.pt(), weight );

   //other variables
   //MPT...   
   

}


// ------------ method called once each job just after ending the event loop  ------------
void 
FinalPlots::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(FinalPlots);
