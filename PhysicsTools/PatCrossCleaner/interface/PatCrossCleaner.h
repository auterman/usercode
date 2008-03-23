// -*- C++ -*-
//
// Package:    PatCrossCleaner
// Class:      PatCrossCleaner
// 
/**\class PatCrossCleaner PatCrossCleaner.h PhysicsTools/PatCrossCleaner/interface/PatCrossCleaner.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian AUTERMANN
//         Created:  Sat Mar 22 12:58:04 CET 2008
// $Id: PatCrossCleaner.h,v 1.1.1.1 2008/03/22 19:07:55 auterman Exp $
//
//

#ifndef PatCrossCleaner_h
#define PatCrossCleaner_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/UtilAlgos/interface/ParameterAdapter.h"
#include "PhysicsTools/PatCrossCleaner/interface/ElectronJetCrossCleaner.h"
#include "PhysicsTools/PatCrossCleaner/interface/CrossCleanerResult.h"
//
// class decleration
//

class PatCrossCleaner : public edm::EDProducer {
   public:
      explicit PatCrossCleaner(const edm::ParameterSet&);
      ~PatCrossCleaner();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      /// Input: All PAT objects that are to cross-clean  or needed for that
      edm::InputTag _patJets;
      edm::InputTag _patMets;
      edm::InputTag _patMuons;
      edm::InputTag _patElectrons;
      edm::InputTag _patPhotons;
      edm::InputTag _patTaus;
      // The following are no PAT objects (yet?)
      edm::InputTag _patCaloTowers;
      edm::InputTag _patTracks;
      edm::InputTag _patVertices;
      
      ///The actual cross-cleaners:
      //Electron-Jet
      edm::ParameterSet _EJselectionCfg;
      pat::ElectronJetCrossCleaner _ElectronJetCC;
      
      //Muon-Jet
      //...
      
};

namespace reco {
  namespace modules {
    /// Helper struct to convert from ParameterSet to ElectronSelection
    template<> 
    struct ParameterAdapter<pat::ElectronJetCrossCleaner> { 
      static pat::ElectronJetCrossCleaner make(const edm::ParameterSet & cfg) {
        pat::ElectronJetCleaning config_;
        config_.value = cfg.getParameter<double>("value");
	//... read in further parameters for Electron-Jet here
	
	return pat::ElectronJetCrossCleaner( config_ );
      }
    };
  }
}


#endif
