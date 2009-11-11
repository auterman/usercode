// -*- C++ -*-
//
// Package:    WeightProducer
// Class:      WeightProducer
// 
/**\class WeightProducer WeightProducer.cc RA2/WeightProducer/src/WeightProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
//         Created:  Tue Nov 10 17:58:04 CET 2009
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


//
// class decleration
//

class WeightProducer : public edm::EDProducer {
   public:
      explicit WeightProducer(const edm::ParameterSet&);
      ~WeightProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      double _weight;
};

WeightProducer::WeightProducer(const edm::ParameterSet& iConfig):
  _weight( iConfig.getParameter<double>("weight") )
{
   //register your products
   produces< double >("weight");
}


WeightProducer::~WeightProducer()
{
}


// ------------ method called to produce the data  ------------
void
WeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   //Read 'ExampleData' from the Event
   //Handle<ExampleData> pIn;
   //iEvent.getByLabel("example",pIn);

   //---------------------------------------------------------------------------
   ///Put here code that computes a weight directly from iEvent/iSetup
   ///informations, i.e. using sample ID, pt-hat-information, etc;
   
   ///Also, here one could define look-up tables for all used samples. 
   ///An identifying 'string' for the currently used sample could be read
   ///from the config file. Perhaps this can be obtained dirtectly from crab?
   //---------------------------------------------------------------------------

   // put weight into the Event
   std::auto_ptr<double> pOut(new double(_weight));
   iEvent.put(pOut,"weight");
}

// ------------ method called once each job just before starting event loop  ------------
void 
WeightProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
WeightProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(WeightProducer);
