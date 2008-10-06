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
// $Id: HHJetProducer.h,v 1.1.1.4 2008/02/25 15:54:04 auterman Exp $
//
//
#ifndef HHJetProducer_H
#define HHJetProducer_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

//
// class decleration
//

class HHJetProducer : public edm::EDProducer {
   public:
      explicit HHJetProducer(const edm::ParameterSet&);
      ~HHJetProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      std::string jettype_,jetlabel_;

      // cuts:
      double ptmin_, eta_, chi2_;
};

//
// constants, enums and typedefs
//

#endif
