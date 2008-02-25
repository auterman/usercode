// -*- C++ -*-
//
// Package:    HHJetProducer
// Class:      HHJetProducer
// 
/**\class HHJetProducer HHJetProducer.cc HHAnalysis/HHJetProducer/src/HHJetProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Fri May  4 12:07:40 CEST 2007
// $Id: HHJetProducer.cc,v 1.1.1.3 2007/11/30 13:10:50 auterman Exp $
//
//

#include "HHAnalysis/HHJetProducer/interface/HHJetProducer.h"

//
// constructors and destructor
//
HHJetProducer::HHJetProducer(const edm::ParameterSet& iConfig)
{
   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
*/
   //now do what ever other initialization is needed

  jettype_  = iConfig.getParameter< std::string > ("Jet_type");
  jetlabel_ = iConfig.getParameter< std::string > ("Jet_label");

  //cuts
  ptmin_ = iConfig.getUntrackedParameter< double > ("ptmin");
  eta_   = iConfig.getUntrackedParameter< double > ("eta");
  chi2_  = iConfig.getUntrackedParameter< double > ("chi2");

  produces<reco::CaloJetCollection>("SelectedJets");
}


HHJetProducer::~HHJetProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HHJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
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

  Handle<reco::CaloJetCollection> jets;
  iEvent.getByLabel(jettype_, jetlabel_, jets);

  //
  // declare output stream
  //
  std::auto_ptr<reco::CaloJetCollection> selected(new reco::CaloJetCollection);
  selected->reserve(jets->size());


  //
  // loop jets
  //
  for(reco::CaloJetCollection::const_iterator jet = jets->begin(); 
      jet!=jets->end(); ++jet){  

    if (jet->pt()<ptmin_) continue;
    if (fabs(jet->eta())>eta_) continue;

    //
    // add muon candidate to selected
    //
    selected->push_back( *jet );
  }    
  iEvent.put(selected, "SelectedJets");
}

// ------------ method called once each job just before starting event loop  ------------
void 
HHJetProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HHJetProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HHJetProducer);
