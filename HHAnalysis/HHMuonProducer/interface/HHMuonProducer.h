// -*- C++ -*-
//
// Package:    HHMuonProducer
// Class:      HHMuonProducer
// 
/**\class HHMuonProducer HHMuonProducer.cc HHProducer/HHMuonProducer/src/HHMuonProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Fri Apr 20 10:28:05 CEST 2007
// $Id: HHMuonProducer.h,v 1.1.1.4 2008/02/25 15:54:04 auterman Exp $
//
//
#ifndef HHMuonProducer_h
#define HHMuonProducer_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


//
// class decleration
//

class HHMuonProducer : public edm::EDProducer {
   public:
      explicit HHMuonProducer(const edm::ParameterSet&);
      ~HHMuonProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      std::string muontype_,muonlabel_;

      // cuts:
      double ptmin_, eta_, chi2_;

};


#endif
