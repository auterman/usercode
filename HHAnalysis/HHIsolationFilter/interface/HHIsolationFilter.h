// -*- C++ -*-
//
// Package:    HHIsolationFilter
// Class:      HHIsolationFilter
// 
/**\class HHIsolationFilter HHIsolationFilter.cc HHAnalysis/HHIsolationFilter/src/HHIsolationFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Wed May 16 14:42:49 CEST 2007
// $Id: HHIsolationFilter.h,v 1.1.1.4 2008/02/25 15:54:04 auterman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//
template <typename S, typename I>
class HHIsolationFilter : public edm::EDFilter {
   public:
      explicit HHIsolationFilter(const edm::ParameterSet&);
      ~HHIsolationFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
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
HHIsolationFilter<S,I>::HHIsolationFilter( const edm::ParameterSet & cfg ) :
  src_( cfg.template getParameter<edm::InputTag>( "src" ) ),
  iso_( cfg.template getParameter<edm::InputTag>( "iso" ) ),
  iso_threshold_( cfg.template getParameter<double>( "threshold" ) ),
  min_pt_( cfg.template getParameter<double>( "MinPt" ) )
{
   //now do what ever initialization is needed

}

template <typename S, typename I>
HHIsolationFilter<S,I>::~HHIsolationFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
template <typename S, typename I> bool
HHIsolationFilter<S,I>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  Handle<S> source;
  Handle<I> isolation;
  iEvent.getByLabel( src_, source );
  iEvent.getByLabel( iso_, isolation );
  bool AtLeastOneIsolated = false;
  typename S::const_iterator src = source->begin();
  typename I::const_iterator iso = isolation->begin();
  
  ///drop events that don't have at least one object satisfying the isolation criteria
  for(; src != source->end() && iso!=isolation->end(); ++src, ++iso) {
     if ( (*iso)<iso_threshold_ && src->pt() > min_pt_){
       AtLeastOneIsolated = true;
       break;
     }  
  }
  return AtLeastOneIsolated;
}

// ------------ method called once each job just before starting event loop  ------------
template <typename S, typename I> void 
HHIsolationFilter<S,I>::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
template <typename S, typename I> void 
HHIsolationFilter<S,I>::endJob() {
}
