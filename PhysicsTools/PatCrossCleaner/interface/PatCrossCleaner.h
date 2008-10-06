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
// $Id: PatCrossCleaner.h,v 1.3 2008/03/24 19:41:50 auterman Exp $
//
//

#ifndef PatCrossCleaner_h
#define PatCrossCleaner_h

// system include files
#include <memory>
#include <map>
#include <utility>//pair
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
      
      ///Helper function for debugging: prints out all objects that should be dropped
      void printDropped(CrossCleanerMap& conflicts);
      ///statistical information
      std::map<int,std::pair<int,int> > _statistics;
};

namespace reco {
  namespace modules {
    /// Helper struct to convert from ParameterSet to ElectronSelection
    template<> 
    struct ParameterAdapter<pat::ElectronJetCrossCleaner> { 
      static pat::ElectronJetCrossCleaner make(const edm::ParameterSet & cfg) {
        pat::ElectronJetCleaning config_;
        config_.deltaR_min = cfg.getParameter<double>("deltaR_min");
        config_.sharedByjetEratio_max = cfg.getParameter<double>("sharedByjetEratio_max");
	//... read in further parameters for Electron-Jet here
	
	return pat::ElectronJetCrossCleaner( config_ );
      }
    };
  }
}


#endif
