// -*- C++ -*-
//
// Package:    HHIsolationProducer
// Class:      HHIsolationProducer
// 
/**\class HHIsolationProducer HHIsolationProducer.cc HHAnalysis/HHIsolationProducer/src/HHIsolationProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Wed May 16 17:40:14 CEST 2007
// $Id: HHIsolationProducer.h,v 1.1.1.1 2007/05/16 17:24:46 auterman Exp $
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
template <typename S, typename I>
class HHIsolationProducer : public edm::EDProducer {
   public:
      HHIsolationProducer(const edm::ParameterSet&);
      ~HHIsolationProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag src_, iso_;
      double iso_threshold_;
      double min_pt_;
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
template <typename S, typename I>
HHIsolationProducer<S,I>::HHIsolationProducer(const edm::ParameterSet& cfg):
  src_( cfg.template getParameter<edm::InputTag>( "src" ) ),
  iso_( cfg.template getParameter<edm::InputTag>( "iso" ) ),
  iso_threshold_( cfg.template getParameter<double>( "threshold" ) ),
  min_pt_( cfg.template getParameter<double>( "MinPt" ) )
{
 
 produces<S>();
 
   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
*/
   //now do what ever other initialization is needed

}


template <typename S, typename I>
HHIsolationProducer<S,I>::~HHIsolationProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
template <typename S, typename I> void
HHIsolationProducer<S,I>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  Handle<S> source;
  Handle<I> isolation;
  iEvent.getByLabel( src_, source );
  iEvent.getByLabel( iso_, isolation );
  typename S::const_iterator src = source->begin();
  typename I::const_iterator iso = isolation->begin();
  std::auto_ptr<S> result(new S);
  
  ///drop events that don't have at least one object satisfying the isolation criteria
  for(; src != source->end() && iso!=isolation->end(); ++src, ++iso) {
     if ( (*iso)<iso_threshold_ && src->pt() > min_pt_){
        result->push_back( *src );
     }  
  }
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/
   iEvent.put(result);
}

// ------------ method called once each job just before starting event loop  ------------
template <typename S, typename I> void 
HHIsolationProducer<S,I>::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
template <typename S, typename I> void 
HHIsolationProducer<S,I>::endJob() {
}
