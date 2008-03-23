#ifndef ElectronJetCrossCleaner_h
#define ElectronJetCrossCleaner_h

/**
    \class pat::ElectronJetCrossCleaner ElectronJetCrossCleaner.h "PhysicsTools/PatUtils/ElectronJetCrossCleaner.h"
    \brief cross cleans objets

    \version $Id: ElectronJetCrossCleaner.h,v 1.1.1.1 2008/03/22 19:07:55 auterman Exp $
**/

#include "PhysicsTools/PatCrossCleaner/interface/CrossCleanerResult.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include <DataFormats/Common/interface/Handle.h>

namespace pat {

  /// Structure defining the electron - jet cross cleaning
  struct ElectronJetCleaning {
    double value; ///Some exemplary cut value
  };


  class ElectronJetCrossCleaner {
  public:
    ElectronJetCrossCleaner( const ElectronJetCleaning& cfg ) : config_( cfg ) {}
    ~ElectronJetCrossCleaner() {}

    void clean(
           const edm::Handle< edm::View< pat::Electron> >& Electrons,
           const edm::Handle< edm::View< pat::Jet> >& Jets,
	   CrossCleanerMap & assMap
         ) const;

  private:
    ElectronJetCleaning config_;

  }; // class
} // namespace

#endif
