// -*- C++ -*-
//
// Package:    JetID
// Class:      JetID
// 
/**\class JetID JetID.cc Demo/JetID/src/JetID.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian AUTERMANN
//         Created:  Mon Apr 14 11:37:04 CEST 2008
// $Id$
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
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
//
// class decleration
//

class JetID : public edm::EDProducer {
   public:
      explicit JetID(const edm::ParameterSet&);
      ~JetID();

   typedef edm::ValueMap<double> JetIDMap;

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
   edm::InputTag _Jets;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JetID::JetID(const edm::ParameterSet& iConfig) :
  _Jets( iConfig.getParameter<edm::InputTag>( "Jets" ) )
{
   //register your products
   produces<JetIDMap>();
}


JetID::~JetID()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
JetID::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

   //Jets   
   Handle<edm::View<CaloJet> > pJets;
   iEvent.getByLabel(_Jets,pJets);
   
   //create map passing the handle to the matched collection
   std::auto_ptr<JetIDMap> jetID(new JetIDMap);
   JetIDMap::Filler filler(*jetID);
   {
     size_t n = pJets->size();
     std::vector<double> id(n);
     for(size_t i = 0; i != n; ++i) {
       id[i] = (double)i; //"ID" is exemplarily no of the jet
     }
     filler.insert(pJets, id.begin(), id.end());
   }
   // really fill the association map
   filler.fill();
   // put into the event 
   iEvent.put(jetID);
}

// ------------ method called once each job just before starting event loop  ------------
void 
JetID::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetID::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetID);
