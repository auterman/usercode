// -*- C++ -*-
//
// Package:    HHMuonProducer
// Class:      HHMuonProducer
// 
/**\class HHMuonProducer HHMuonProducer.cc HHAnalysis/HHMuonProducer/src/HHMuonProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Fri Apr 20 11:54:04 CEST 2007
// $Id: HHMuonProducer.cc,v 1.1.1.1 2007/05/07 11:15:25 auterman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HHAnalysis/HHMuonProducer/interface/HHMuonProducer.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HHMuonProducer::HHMuonProducer(const edm::ParameterSet& iConfig)
{

   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
*/
   //now do what ever other initialization is needed

  muontype_  = iConfig.getParameter< std::string > ("Muon_type");
  muonlabel_ = iConfig.getParameter< std::string > ("Muon_label");

  //cuts
  ptmin_ = iConfig.getUntrackedParameter< double > ("ptmin");
  eta_   = iConfig.getUntrackedParameter< double > ("eta");
  chi2_  = iConfig.getUntrackedParameter< double > ("chi2");

  produces<reco::MuonCollection>("SelectedMuons");

}


HHMuonProducer::~HHMuonProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HHMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/

  Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(muontype_, muonlabel_, muons);

  //
  // declare output stream
  //
  std::auto_ptr<reco::MuonCollection> selected(new reco::MuonCollection);
  selected->reserve(muons->size());


  //
  // loop muons
  //
  for(reco::MuonCollection::const_iterator muon = muons->begin(); 
      muon!=muons->end(); ++muon){  

    if (muon->pt()<ptmin_) continue;
    if (fabs(muon->eta())>eta_) continue;

    //
    // add muon candidate to selected
    //
    selected->push_back( *muon );
  }    
  iEvent.put(selected, "SelectedMuons");

}

// ------------ method called once each job just before starting event loop  ------------
void 
HHMuonProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HHMuonProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HHMuonProducer);
